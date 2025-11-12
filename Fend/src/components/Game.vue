<script setup>
import { ref, onMounted, onUnmounted } from 'vue';

const ws = ref(null);
const isConnected = ref(false);
const score = ref(0);
const message = ref('');
const messageType = ref('info');
const showMessage = ref(false);
let reconnectInterval = null;

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
    displayMessage(`Command: ${cmd}`, 'info');
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

        <!-- D-Pad -->
        <div class="grid grid-cols-3 gap-2 w-fit mx-auto mb-6">
          <div></div>
          <button class="btn btn-primary btn-square btn-lg text-2xl" @click="sendCommand('UP')">
            ▲
          </button>
          <div></div>

          <button class="btn btn-primary btn-square btn-lg text-2xl" @click="sendCommand('LEFT')">
            ◄
          </button>
          <div></div>
          <button class="btn btn-primary btn-square btn-lg text-2xl" @click="sendCommand('RIGHT')">
            ►
          </button>

          <div></div>
          <button class="btn btn-primary btn-square btn-lg text-2xl" @click="sendCommand('DOWN')">
            ▼
          </button>
          <div></div>
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
</style>
