<script setup>
import { ref } from 'vue';
import Header from "@/components/Header.vue";
import Body from "@/components/Body.vue";
import Menu from "@/components/Menu.vue";
import Game from "@/components/Game.vue";

const currentView = ref('menu'); // 'menu', 'settings', 'game'

const handleMenuSelect = (option) => {
  if (option === 'game') {
    currentView.value = 'game';
  } else if (option === 'settings') {
    currentView.value = 'settings';
  }
};

const goBackToMenu = () => {
  currentView.value = 'menu';
};
</script>

<template>
  <!-- Menu iniziale -->
  <Menu v-if="currentView === 'menu'" @select="handleMenuSelect" />

  <!-- Gioco Snake -->
  <div v-else-if="currentView === 'game'" class="relative">
    <button
        class="btn btn-ghost gap-2 fixed top-4 left-4 z-50 bg-base-100/90 backdrop-blur-sm shadow-lg hover:bg-base-100"
        @click="goBackToMenu"
    >
      <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-5 h-5">
        <path stroke-linecap="round" stroke-linejoin="round" d="M10.5 19.5L3 12m0 0l7.5-7.5M3 12h18" />
      </svg>
      Torna al Menu
    </button>
    <Game />
  </div>

  <!-- Impostazioni (vista originale) -->
  <div v-else-if="currentView === 'settings'" class="flex flex-col" style="height: 100vh; width: 100vw">
    <Header @logo-click="goBackToMenu" />
    <button
        class="btn btn-primary gap-2 fixed top-20 left-4 z-50 shadow-lg"
        @click="goBackToMenu"
    >
      <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-5 h-5">
        <path stroke-linecap="round" stroke-linejoin="round" d="M10.5 19.5L3 12m0 0l7.5-7.5M3 12h18" />
      </svg>
      Torna al Menu
    </button>
    <Body class="overflow-scroll pt-3" style="place-self: self-end"/>
  </div>
</template>

<style scoped>
</style>
