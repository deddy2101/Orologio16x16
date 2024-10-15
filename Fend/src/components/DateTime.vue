<script setup lang="ts">
import {onMounted, ref} from 'vue';
import { getCurrentDateTime } from "@/composables/composable";
import useApi from "@/composables/useAPI";
const {getTimeSetting, setInternetTime, setDateTime} = useApi();

onMounted(async () => {
  try{
   const response = await getTimeSetting();
   //the response.message is a string true or false converted to boolean
    isManualTime.value = response.message === "false";
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
      const response = await setInternetTime(!isManualTime.value);
      if (response.status === 200) {
        alert(response.message);
      } else {
        alert(response.message);
      }
    } catch (err) {
      alert(err);
    }
  } else {
    try {
      const hour = dateTime.value.split("T")[1].split(":")[0];
      const minute = dateTime.value.split("T")[1].split(":")[1];
      const second = dateTime.value.split("T")[1].split(":")[2];
      const day = dateTime.value.split("T")[0].split("-")[2];
      const month = dateTime.value.split("T")[0].split("-")[1];
      const year = dateTime.value.split("T")[0].split("-")[0];
      const response = await setDateTime(hour, minute, second, day, month, year);
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
  <label class="form-control w-full max-w-xs mb-4 flex content-center justify-center">
    <div class="label">
      <span class="label-text">AUTO/MANUALE</span>
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