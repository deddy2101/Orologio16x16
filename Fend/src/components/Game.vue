<script setup>
import { ref, onMounted, onUnmounted } from 'vue';

const ws = ref(null);
const isConnected = ref(false);
const score = ref(0);
const message = ref('');
const messageType = ref('info');
const showMessage = ref(false);
let reconnectInterval = null;

// Joystick state
const isDragging = ref(false);
const joystickX = ref(0); // -1 a 1
const joystickY = ref(0); // -1 a 1
const lastDirection = ref('');

// Ottiene l'URL WebSocket dall'indirizzo corrente
const getWebSocketUrl = () => {
  const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
  const host = window.location.hostname;
  const port = window.location.port ? `:${window.location.port}` : '';
  if ( host === 'localhost' ) {
    console.log("siamo in localhost")
    return `ws://192.168.1.1/ws`;
  }
  return `${protocol}//${host}${port}/ws`;
};

const connect = () => {
  const wsUrl = getWebSocketUrl();
  console.log('Connecting to:', wsUrl);

  ws.value = new WebSocket(wsUrl);

  ws.value.onopen = () => {
    console.log('WebSocket connected');
    isConnected.value = true;
    displayMessage('Connesso al controller!', 'success');

    if (reconnectInterval) {
      clearInterval(reconnectInterval);
      reconnectInterval = null;
    }
  };

  ws.value.onmessage = (event) => {
    console.log('Received:', event.data);

    if (event.data.startsWith('SCORE:')) {
      score.value = parseInt(event.data.split(':')[1]);
    } else if (event.data.startsWith('STATE:')) {
      const state = event.data.split(':')[1];
      displayMessage(`Game State: ${state}`, 'info');
    } else {
      displayMessage(event.data, 'info');
    }
  };

  ws.value.onerror = (error) => {
    console.error('WebSocket error:', error);
    displayMessage('Errore di connessione', 'error');
  };

  ws.value.onclose = () => {
    console.log('WebSocket disconnected');
    isConnected.value = false;

    if (!reconnectInterval) {
      reconnectInterval = setInterval(connect, 3000);
    }
  };
};

const sendCommand = (cmd) => {
  if (ws.value && ws.value.readyState === WebSocket.OPEN) {
    ws.value.send(cmd);
    console.log('Sent:', cmd);
  } else {
    displayMessage('Non connesso!', 'error');
  }
};

const displayMessage = (text, type) => {
  message.value = text;
  messageType.value = type;
  showMessage.value = true;

  setTimeout(() => {
    showMessage.value = false;
  }, 3000);
};

// Joystick handlers
const handleJoystickStart = (event) => {
  isDragging.value = true;
  updateJoystickPosition(event);
};

const handleJoystickMove = (event) => {
  if (!isDragging.value) return;
  event.preventDefault();
  updateJoystickPosition(event);
  sendDirectionCommand();
};

const handleJoystickEnd = () => {
  isDragging.value = false;
  joystickX.value = 0;
  joystickY.value = 0;
  lastDirection.value = '';
};

const updateJoystickPosition = (event) => {
  const touch = event.touches ? event.touches[0] : event;
  const joystickElement = event.currentTarget;
  const rect = joystickElement.getBoundingClientRect();

  // Calcola il centro del joystick
  const centerX = rect.left + rect.width / 2;
  const centerY = rect.top + rect.height / 2;

  // Calcola la posizione relativa (-1 a 1)
  const deltaX = touch.clientX - centerX;
  const deltaY = touch.clientY - centerY;

  // Limita il movimento al raggio del joystick
  const maxRadius = rect.width / 2;
  const distance = Math.sqrt(deltaX * deltaX + deltaY * deltaY);

  if (distance > maxRadius) {
    const angle = Math.atan2(deltaY, deltaX);
    joystickX.value = Math.cos(angle);
    joystickY.value = Math.sin(angle);
  } else {
    joystickX.value = deltaX / maxRadius;
    joystickY.value = deltaY / maxRadius;
  }
};

const sendDirectionCommand = () => {
  // Determina la direzione principale
  let direction = '';

  if (Math.abs(joystickX.value) > Math.abs(joystickY.value)) {
    // Movimento orizzontale predominante
    direction = joystickX.value > 0.3 ? 'RIGHT' : joystickX.value < -0.3 ? 'LEFT' : '';
  } else {
    // Movimento verticale predominante
    direction = joystickY.value > 0.3 ? 'DOWN' : joystickY.value < -0.3 ? 'UP' : '';
  }

  // Invia comando solo se la direzione è cambiata
  if (direction && direction !== lastDirection.value) {
    sendCommand(direction);
    lastDirection.value = direction;
  }
};

const handleKeydown = (event) => {
  const key = event.key.toLowerCase();

  if (key === 'w' || key === 'arrowup') {
    sendCommand('UP');
    event.preventDefault();
  } else if (key === 's' || key === 'arrowdown') {
    sendCommand('DOWN');
    event.preventDefault();
  } else if (key === 'a' || key === 'arrowleft') {
    sendCommand('LEFT');
    event.preventDefault();
  } else if (key === 'd' || key === 'arrowright') {
    sendCommand('RIGHT');
    event.preventDefault();
  } else if (key === ' ') {
    sendCommand('PAUSE');
    event.preventDefault();
  } else if (key === 'enter') {
    sendCommand('START');
    event.preventDefault();
  }
};

onMounted(() => {
  connect();
  document.addEventListener('keydown', handleKeydown);
});

onUnmounted(() => {
  if (reconnectInterval) {
    clearInterval(reconnectInterval);
  }
  if (ws.value) {
    ws.value.close();
  }
  document.removeEventListener('keydown', handleKeydown);
});
</script>

<template>
  <div class="min-h-screen bg-gradient-to-br from-purple-500 via-purple-600 to-indigo-700 flex items-center justify-center p-4">
    <div class="card w-full max-w-lg bg-base-100 shadow-2xl">
      <div class="card-body">
        <!-- Title -->
        <h1 class="card-title text-4xl font-bold justify-center mb-2">
          🐍 Snake Game
        </h1>

        <!-- Status Badge -->
        <div class="flex justify-center mb-4">
          <div v-if="isConnected" class="badge badge-success badge-lg gap-2">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="inline-block w-4 h-4 stroke-current">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>
            </svg>
            Connected
          </div>
          <div v-else class="badge badge-error badge-lg gap-2">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="inline-block w-4 h-4 stroke-current">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
            </svg>
            Disconnected - Reconnecting...
          </div>
        </div>

        <!-- Score -->
        <div class="text-center mb-6">
          <div class="stat place-items-center bg-primary text-primary-content rounded-lg">
            <div class="stat-title text-base-100">Score</div>
            <div class="stat-value">{{ score }}</div>
          </div>
        </div>

        <!-- Control Buttons -->
        <div class="flex flex-col gap-3 mb-6">
          <button class="btn btn-success btn-lg" @click="sendCommand('START')">
            ▶️ Start Game
          </button>
          <button class="btn btn-warning btn-lg" @click="sendCommand('PAUSE')">
            ⏸️ Pause
          </button>
          <button class="btn btn-error btn-lg" @click="sendCommand('STOP')">
            ⏹️ Stop
          </button>
        </div>

        <!-- Virtual Joystick -->
        <div class="mb-6">
          <div class="alert alert-info mb-4">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="stroke-current shrink-0 w-6 h-6">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path>
            </svg>
            <span class="text-sm">Trascina il joystick per controllare il serpente</span>
          </div>

          <div class="flex justify-center">
            <div
                class="relative w-64 h-64 rounded-full bg-gradient-to-br from-purple-400 to-indigo-600 shadow-2xl cursor-pointer select-none"
                @touchstart="handleJoystickStart"
                @touchmove="handleJoystickMove"
                @touchend="handleJoystickEnd"
                @mousedown="handleJoystickStart"
                @mousemove="handleJoystickMove"
                @mouseup="handleJoystickEnd"
                @mouseleave="handleJoystickEnd"
            >
              <!-- Outer Ring -->
              <div class="absolute inset-4 rounded-full border-4 border-white/30"></div>

              <!-- Center Dot -->
              <div class="absolute top-1/2 left-1/2 w-4 h-4 bg-white/50 rounded-full transform -translate-x-1/2 -translate-y-1/2"></div>

              <!-- Direction Indicators -->
              <div class="absolute top-4 left-1/2 transform -translate-x-1/2 text-white/60 font-bold text-lg">▲</div>
              <div class="absolute bottom-4 left-1/2 transform -translate-x-1/2 text-white/60 font-bold text-lg">▼</div>
              <div class="absolute top-1/2 left-4 transform -translate-y-1/2 text-white/60 font-bold text-lg">◄</div>
              <div class="absolute top-1/2 right-4 transform -translate-y-1/2 text-white/60 font-bold text-lg">►</div>

              <!-- Joystick Stick -->
              <div
                  class="absolute top-1/2 left-1/2 w-20 h-20 bg-white rounded-full shadow-lg transform -translate-x-1/2 -translate-y-1/2 transition-transform"
                  :style="{
                  transform: `translate(calc(-50% + ${joystickX * 72}px), calc(-50% + ${joystickY * 72}px))`
                }"
              >
                <!-- Inner Circle -->
                <div class="absolute inset-2 rounded-full bg-gradient-to-br from-purple-500 to-indigo-600"></div>

                <!-- Highlight -->
                <div class="absolute top-2 left-2 w-6 h-6 bg-white/40 rounded-full blur-sm"></div>
              </div>

              <!-- Active Indicator -->
              <div
                  v-if="isDragging"
                  class="absolute inset-0 rounded-full bg-white/10 animate-pulse"
              ></div>
            </div>
          </div>
        </div>

        <!-- Keyboard Hint -->
        <div class="alert alert-info">
          <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="stroke-current shrink-0 w-6 h-6">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path>
          </svg>
          <span class="text-sm">Puoi anche usare W/A/S/D o le frecce direzionali</span>
        </div>

        <!-- Message Toast -->
        <transition name="fade">
          <div v-if="showMessage" class="toast toast-top toast-center">
            <div
                class="alert"
                :class="{
                'alert-info': messageType === 'info',
                'alert-success': messageType === 'success',
                'alert-error': messageType === 'error'
              }"
            >
              <span>{{ message }}</span>
            </div>
          </div>
        </transition>
      </div>
    </div>
  </div>
</template>

<style scoped>
.fade-enter-active, .fade-leave-active {
  transition: opacity 0.3s;
}
.fade-enter-from, .fade-leave-to {
  opacity: 0;
}

.select-none {
  -webkit-user-select: none;
  user-select: none;
  -webkit-touch-callout: none;
}

/* Previeni il drag dell'immagine */
img {
  pointer-events: none;
}
</style>
