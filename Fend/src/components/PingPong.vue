<script setup>
import { ref, onMounted, onUnmounted, computed } from 'vue';

const ws = ref(null);
const isConnected = ref(false);
const player = ref(null); // null, 1, o 2
const score1 = ref(0);
const score2 = ref(0);
const message = ref('');
const messageType = ref('info');
const showMessage = ref(false);
const gameStarted = ref(false);
let reconnectInterval = null;

// Joystick state (copiato da Snake)
const isDragging = ref(false);
const joystickY = ref(0); // Solo asse Y per ping pong
const lastDirection = ref('');

// Ottiene l'URL WebSocket dall'indirizzo corrente
const getWebSocketUrl = () => {
  const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
  const host = window.location.hostname;
  const port = window.location.port ? `:${window.location.port}` : '';
  if (host === 'localhost') {
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

    if (event.data.startsWith('SCORE:P1:')) {
      score1.value = parseInt(event.data.split(':')[2]);
    } else if (event.data.startsWith('SCORE:P2:')) {
      score2.value = parseInt(event.data.split(':')[2]);
    } else if (event.data.startsWith('STATE:')) {
      const state = event.data.split(':')[1];
      if (state.includes('WIN')) {
        displayMessage(`${state}! 🎉`, 'success');
        gameStarted.value = false;
      } else if (state === 'STOPPED') {
        gameStarted.value = false;
        displayMessage('Gioco fermato', 'info');
      }
    } else if (event.data.includes('Started')) {
      gameStarted.value = true;
      displayMessage('Partita iniziata!', 'success');
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
    const command = player.value ? `P${player.value}:${cmd}` : cmd;
    ws.value.send(command);
    console.log('Sent:', command);
  } else {
    displayMessage('Non connesso!', 'error');
  }
};

const startGame = () => {
  if (ws.value && ws.value.readyState === WebSocket.OPEN) {
    ws.value.send('START_PONG');
    score1.value = 0;
    score2.value = 0;
    gameStarted.value = true;
  }
};

const stopGame = () => {
  sendCommand('STOP');
  gameStarted.value = false;
};

const pauseGame = () => {
  sendCommand('PAUSE');
};

const selectPlayer = (playerNumber) => {
  player.value = playerNumber;
  displayMessage(`Sei il Giocatore ${playerNumber}!`, 'success');
};

const displayMessage = (text, type) => {
  message.value = text;
  messageType.value = type;
  showMessage.value = true;

  setTimeout(() => {
    showMessage.value = false;
  }, 3000);
};

// Joystick handlers (copiati da Snake, adattati per solo UP/DOWN)
const handleJoystickStart = (event) => {
  if (!player.value) return;
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
  joystickY.value = 0;
  lastDirection.value = '';
};

const updateJoystickPosition = (event) => {
  const touch = event.touches ? event.touches[0] : event;
  const joystickElement = event.currentTarget;
  const rect = joystickElement.getBoundingClientRect();

  // Calcola il centro del joystick
  const centerY = rect.top + rect.height / 2;

  // Calcola la posizione relativa (-1 a 1) solo per Y
  const deltaY = touch.clientY - centerY;

  // Limita il movimento al raggio del joystick
  const maxRadius = rect.height / 2;

  if (Math.abs(deltaY) > maxRadius) {
    joystickY.value = deltaY > 0 ? 1 : -1;
  } else {
    joystickY.value = deltaY / maxRadius;
  }
};

const sendDirectionCommand = () => {
  // Solo movimento verticale
  let direction = '';

  // Movimento verticale con soglia 0.3
  direction = joystickY.value > 0.3 ? 'DOWN' : joystickY.value < -0.3 ? 'UP' : '';

  // Invia comando solo se la direzione è cambiata
  if (direction && direction !== lastDirection.value) {
    sendCommand(direction);
    lastDirection.value = direction;
  }
};

// Computed properties per lo stile dei giocatori
const playerColor = computed(() => {
  return player.value === 1 ? 'from-blue-500 to-blue-700' : 'from-green-500 to-green-700';
});

const playerBadgeColor = computed(() => {
  return player.value === 1 ? 'badge-info' : 'badge-success';
});

onMounted(() => {
  connect();
});

onUnmounted(() => {
  if (reconnectInterval) {
    clearInterval(reconnectInterval);
  }
  if (ws.value) {
    ws.value.close();
  }
});
</script>

<template>
  <div class="min-h-screen bg-gradient-to-br from-purple-500 via-purple-600 to-indigo-700 flex items-center justify-center p-4">
    <div class="card w-full max-w-lg bg-base-100 shadow-2xl">
      <div class="card-body">
        <!-- Title -->
        <h1 class="card-title text-4xl font-bold justify-center mb-2">
          🏓 Ping Pong
        </h1>

        <!-- Status Badge -->
        <div class="flex justify-center mb-4">
          <div v-if="isConnected" class="badge badge-success badge-lg gap-2">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="inline-block w-4 h-4 stroke-current">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>
            </svg>
            Connesso
          </div>
          <div v-else class="badge badge-error badge-lg gap-2">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="inline-block w-4 h-4 stroke-current">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
            </svg>
            Disconnesso
          </div>
        </div>

        <!-- Player Selection (se non ha ancora scelto) -->
        <div v-if="!player" class="mb-6">
          <h2 class="text-2xl font-bold text-center mb-4">Scegli il tuo giocatore</h2>
          <div class="grid grid-cols-2 gap-4">
            <button
                class="btn btn-lg h-auto py-8 flex-col gap-3 bg-gradient-to-br from-blue-500 to-blue-700 hover:from-blue-600 hover:to-blue-800 border-0 text-white"
                @click="selectPlayer(1)"
            >
              <span class="text-5xl">🔵</span>
              <div class="flex flex-col gap-1">
                <span class="text-2xl font-bold">Player 1</span>
                <span class="text-sm opacity-80 font-normal">Sinistra</span>
              </div>
            </button>

            <button
                class="btn btn-lg h-auto py-8 flex-col gap-3 bg-gradient-to-br from-green-500 to-green-700 hover:from-green-600 hover:to-green-800 border-0 text-white"
                @click="selectPlayer(2)"
            >
              <span class="text-5xl">🟢</span>
              <div class="flex flex-col gap-1">
                <span class="text-2xl font-bold">Player 2</span>
                <span class="text-sm opacity-80 font-normal">Destra</span>
              </div>
            </button>
          </div>
        </div>

        <!-- Game Interface (dopo aver scelto il giocatore) -->
        <div v-else>
          <!-- Player Badge -->
          <div class="flex justify-center mb-4">
            <div class="badge badge-lg gap-2" :class="playerBadgeColor">
              <span class="text-2xl">{{ player === 1 ? '🔵' : '🟢' }}</span>
              Player {{ player }}
            </div>
          </div>

          <!-- Score Board -->
          <div class="grid grid-cols-2 gap-4 mb-6">
            <div class="stat place-items-center bg-blue-500 text-white rounded-lg">
              <div class="stat-title text-blue-100">Player 1</div>
              <div class="stat-value text-5xl">{{ score1 }}</div>
            </div>
            <div class="stat place-items-center bg-green-500 text-white rounded-lg">
              <div class="stat-title text-green-100">Player 2</div>
              <div class="stat-value text-5xl">{{ score2 }}</div>
            </div>
          </div>

          <!-- Game Controls -->
          <div v-if="!gameStarted" class="flex flex-col gap-3 mb-6">
            <button class="btn btn-success btn-lg" @click="startGame">
              ▶️ Inizia Partita
            </button>
          </div>

          <div v-else class="flex flex-col gap-3 mb-6">
            <button class="btn btn-warning btn-lg" @click="pauseGame">
              ⏸️ Pausa
            </button>
            <button class="btn btn-error btn-lg" @click="stopGame">
              ⏹️ Ferma
            </button>
          </div>

          <!-- Virtual Joystick (solo verticale) -->
          <div class="mb-6">
            <div class="alert alert-info mb-4">
              <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="stroke-current shrink-0 w-6 h-6">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path>
              </svg>
              <span class="text-sm">Trascina su/giù per controllare la racchetta</span>
            </div>

            <div class="flex justify-center">
              <div
                  class="relative w-32 h-96 rounded-full shadow-2xl cursor-pointer select-none bg-gradient-to-b"
                  :class="playerColor"
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

                <!-- Center Line -->
                <div class="absolute top-1/2 left-1/2 w-16 h-1 bg-white/50 transform -translate-x-1/2 -translate-y-1/2"></div>

                <!-- Direction Indicators -->
                <div class="absolute top-8 left-1/2 transform -translate-x-1/2 text-white/60 font-bold text-2xl">▲</div>
                <div class="absolute top-1/4 left-1/2 transform -translate-x-1/2 text-white/40 text-lg">SU</div>

                <div class="absolute bottom-8 left-1/2 transform -translate-x-1/2 text-white/60 font-bold text-2xl">▼</div>
                <div class="absolute bottom-1/4 left-1/2 transform -translate-x-1/2 text-white/40 text-lg">GIÙ</div>

                <!-- Joystick Stick (solo movimento verticale) -->
                <div
                    class="absolute top-1/2 left-1/2 w-20 h-20 bg-white rounded-full shadow-lg transform -translate-x-1/2 -translate-y-1/2 transition-transform"
                    :style="{
                    transform: `translate(-50%, calc(-50% + ${joystickY * 140}px))`
                  }"
                >
                  <!-- Inner Circle -->
                  <div class="absolute inset-2 rounded-full bg-gradient-to-b" :class="playerColor"></div>

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

          <!-- Change Player Button -->
          <button class="btn btn-outline btn-sm w-full" @click="player = null">
            🔄 Cambia Giocatore
          </button>
        </div>

        <!-- Message Toast -->
        <transition name="fade">
          <div v-if="showMessage" class="toast toast-top toast-center z-50">
            <div
                class="alert shadow-lg"
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
</style>
