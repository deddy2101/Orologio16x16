<script setup>
import { defineEmits } from 'vue';

const emit = defineEmits(['logo-click']);

const handleLogoClick = () => {
  emit('logo-click');
};
</script>

<template>
  <div class="navbar bg-primary text-primary-content shadow-lg">
    <div class="flex-1">
      <button
          class="btn btn-ghost normal-case text-xl hover:bg-primary-focus"
          @click="handleLogoClick"
      >
        Orologio
      </button>
    </div>
    <div class="flex-none">
      <ul class="menu menu-horizontal px-1">
        <li>
          <a @click.prevent="handleLogoClick" class="hover:bg-primary-focus">Home</a>
        </li>
      </ul>
    </div>
  </div>
</template>

<style scoped>
</style>
