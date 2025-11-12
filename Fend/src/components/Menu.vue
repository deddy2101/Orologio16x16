<script setup>
import { defineEmits } from 'vue';

const emit = defineEmits(['select']);

const selectOption = (option) => {
  emit('select', option);
};
</script>

<template>
  <div class="min-h-screen bg-gradient-to-br from-purple-500 via-purple-600 to-indigo-700 flex items-center justify-center p-4">
    <div class="card w-full max-w-lg bg-base-100 shadow-2xl">
      <div class="card-body items-center text-center">
        <!-- Title -->
        <h1 class="text-5xl font-bold mb-2">🕐 Orologio Smart</h1>
        <p class="text-xl text-base-content/70 mb-8">Cosa vuoi fare?</p>

        <!-- Menu Options -->
        <div class="w-full space-y-4">
          <!-- Play Button -->
          <button
              class="btn btn-lg w-full h-auto py-6 flex-col gap-3 bg-gradient-to-r from-purple-500 to-indigo-600 hover:from-purple-600 hover:to-indigo-700 border-0 text-white"
              @click="selectOption('game')"
          >
            <span class="text-5xl">🎮</span>
            <div class="flex flex-col gap-1">
              <span class="text-2xl font-bold">Gioca a Snake</span>
              <span class="text-sm opacity-80 font-normal">Controlla il gioco Snake</span>
            </div>
          </button>

          <!-- Settings Button -->
          <button
              class="btn btn-lg w-full h-auto py-6 flex-col gap-3 bg-gradient-to-r from-pink-500 to-rose-600 hover:from-pink-600 hover:to-rose-700 border-0 text-white"
              @click="selectOption('settings')"
          >
            <span class="text-5xl">⚙️</span>
            <div class="flex flex-col gap-1">
              <span class="text-2xl font-bold">Impostazioni</span>
              <span class="text-sm opacity-80 font-normal">Configura data, ora, WiFi e altro</span>
            </div>
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
