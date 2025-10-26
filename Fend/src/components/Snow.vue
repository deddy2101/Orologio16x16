<script setup lang="ts">
import {onMounted, ref} from 'vue';
import { getCurrentDateTime } from "@/composables/composable";
import useApi from "@/composables/useAPI";
const {setSnow, getSnow} = useApi();

onMounted(async () => {
  try{
   const response = await getSnow();
   //the response.message is a string true or false converted to boolean
    snowState.value = response.message === "true";
  } catch (err) {
    snowState.value = false;
    alert(err);
  }
})

const dateTime = ref<string>("");
const snowState = ref<boolean>(true);

const handleGetTime = () => {
  dateTime.value = getCurrentDateTime();
  console.log(dateTime.value);
}

const handleSetSnow = async () => {

    try {
      const response = await setSnow(snowState.value);
      if (response.status === 200) {
        alert(response.message);
      } else {
        alert(response.message);
      }
    } catch (err) {
      alert(err);
    }

}
</script>

<template>
  <label class="form-control w-full max-w-xs mb-4 flex content-center justify-center">
    <div class="label">
      <span class="label-text">ON/OFF</span>
    </div>
    <input  type="checkbox" class="toggle" v-model="snowState" />
  </label>
  <div class="flex flex-col ">
    <button class="btn btn-primary ml-1" @click="handleSetSnow">Salva</button>
  </div>
</template>

<style scoped>
</style>