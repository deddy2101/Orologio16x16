<script setup lang="ts">
import {onMounted, ref} from 'vue';
import { getCurrentDateTime } from "@/composables/composable";
import useApi from "@/composables/useAPI";
const {getTimeSetting, setInternetTime} = useApi();

onMounted(async () => {
  try{
    isManualTime.value = await getTimeSetting();
  } catch (err) {
    isManualTime.value = false;
    alert(err);
  }
})

const dateTime = ref<string>("");
const isManualTime = ref<boolean>(true);

const handleGetTime = () => {
  dateTime.value = getCurrentDateTime();
  console.log(dateTime.value);
}

const handleSetDatetime = async () => {
  if(isManualTime.value === false) {
    try {
      const response = await setInternetTime(isManualTime.value);
      if (response.status === 200) {
        alert(response.message);
      } else {
        alert(response.message);
      }
    } catch (err) {
      alert(err);
    }
  }
}
</script>

<template>
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">NTP/MANUALE</span>
    </div>
    <input  type="checkbox" class="toggle" v-model="isManualTime" />
  </label>
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">DATA E ORA DA IMPOSTARE</span>
    </div>
    <input type="datetime-local" placeholder="Type here" v-model="dateTime" class="input input-bordered w-full max-w-xs" :disabled="!isManualTime" />
  </label>
  <div class="flex flex-col ">
    <button class="btn btn-accent mb-2" @click="handleGetTime" :disabled="!isManualTime">Ottieni dal tuo dispositivo</button>
    <button class="btn btn-primary ml-1" @click="handleSetDatetime">Salva</button>
  </div>
</template>

<style scoped>
</style>