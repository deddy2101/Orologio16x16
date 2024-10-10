<script setup lang="ts">
import {onMounted, ref} from "vue";
import useApi from "@/composables/useAPI";
const {getDimTime, setDimTime} = useApi();

const startDimTime = ref<string>('');
const endDimTime = ref<string>('');
onMounted(async () => {
  const response = await getDimTime();
  //in the message there is startdimtime,enddimtime
  const [startDimTimet, endDimTimet] = response.message.split(',');
  startDimTime.value = startDimTimet;
  endDimTime.value = endDimTimet;
});
const handleSave = async () => {
  try {
    const response = await setDimTime(startDimTime.value, endDimTime.value);
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
  <span class="text-xl font-bold"> Orario in formato H24 solo ore</span>
  <label class="form-control w-full max-w-xs mb-2">
    <div class="label">
      <span class="label-text">Ora Inizio</span>
    </div>
  <input type="text" placeholder="Ora Inizio" class="input input-bordered w-full max-w-xs mb-4" v-model="startDimTime"/>
  </label>
  <label class="form-control w-full max-w-xs mb-2">
    <div class="label">
      <span class="label-text">Ora Fine</span>
    </div>
  <input type="text" placeholder="OraFine" class="input input-bordered w-full max-w-xs mb-4" v-model="endDimTime"/>
  </label>
  <button class="btn btn-primary" @click="handleSave">Salva</button>
</template>

<style scoped>

</style>