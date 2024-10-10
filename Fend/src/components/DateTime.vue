<script setup lang="ts">
import { ref } from 'vue';
import { getCurrentDateTime } from "@/composables/composable";

const dateTime = ref<string>("");
const isManualTime = ref<boolean>(true);

const handleGetTime = () => {
  dateTime.value = getCurrentDateTime();
  console.log(dateTime.value);
}
</script>

<template>
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">Carica ora manualmente</span>
    </div>
    <input type="checkbox" class="toggle" v-model="isManualTime" />
  </label>
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">DATA E ORA DA IMPOSTARE</span>
    </div>
    <input type="datetime-local" placeholder="Type here" v-model="dateTime" class="input input-bordered w-full max-w-xs" :disabled="!isManualTime" />
  </label>
  <div>
    <button class="btn btn-accent" @click="handleGetTime" :disabled="!isManualTime">Ottieni dal tuo dispositivo</button>
    <button class="btn btn-primary ml-1">Salva</button>
  </div>
</template>

<style scoped>
</style>