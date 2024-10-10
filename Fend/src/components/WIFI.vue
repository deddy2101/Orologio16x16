<script setup lang="ts">
import { ref } from 'vue';

const ssid = ref<string>('');
const password = ref<string>('');
const isAccessPoint = ref<boolean>(false);
</script>

<template>
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">Abilita Acces Point</span>
    </div>
    <input type="checkbox" class="toggle" v-model="isAccessPoint" />
  </label>
  <input type="text" placeholder="SSID" class="input input-bordered w-full max-w-xs mb-4" v-model="ssid" :disabled="isAccessPoint" />
  <input type="text" placeholder="PASSWD" class="input input-bordered w-full max-w-xs mb-4" v-model="password" :disabled="isAccessPoint" />
  <button class="btn btn-primary">Salva</button>
</template>

<style scoped>
</style>