<script setup lang="ts">
import {onMounted, ref} from "vue";
import useApi from "@/composables/useAPI";
const {getDimTime, setDimTime} = useApi();

const startDimTime = ref<string>('');
const endDimTime = ref<string>('');
const dayDimValue = ref<number>(150);
const nightDimValue = ref<number>(10);
onMounted(async () => {
  const response = await getDimTime();
  //in the message there is startdimtime,enddimtime
  const [startDimTimet, endDimTimet, nightDim, dayDim ] = response.message.split(',');
  startDimTime.value = startDimTimet;
  endDimTime.value = endDimTimet;
  dayDimValue.value = parseInt(dayDim);
  nightDimValue.value = parseInt(nightDim);
});
const handleSave = async () => {
  try {
    const response = await setDimTime(startDimTime.value, endDimTime.value, dayDimValue.value, nightDimValue.value);
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
  <div class="flex flex-row">
    <label class="form-control w-full max-w-xs mb-2">
      <div class="label">
        <span class="label-text">Ora Inizio</span>
      </div>
      <input type="text" placeholder="Ora Inizio" class="input input-bordered w-full max-w-xs mb-4" v-model="startDimTime"/>
    </label>
    <label class="form-control w-full max-w-xs mb-2 ml-1">
      <div class="label">
        <span class="label-text">Luminosità notte {{nightDimValue}}</span>
      </div>
      <input type="range" min="0" max="254" value="10" class="range range-info w-full " style="margin-top: 14px " v-model="nightDimValue"/>
    </label>
  </div>
  <div class="flex flex-row">
    <label class="form-control w-full max-w-xs mb-2">
      <div class="label">
        <span class="label-text">Ora Fine</span>
      </div>
      <input type="text" placeholder="OraFine" class="input input-bordered w-full max-w-xs mb-4" v-model="endDimTime"/>
    </label>
    <label class="form-control w-full max-w-xs mb-2 ml-1">
      <div class="label">
        <span class="label-text">Lumonosità giorno {{dayDimValue}}</span>
      </div>
      <input type="range" min="0" max="254" value="150" class="range range-info w-full " style="margin-top: 14px " v-model="dayDimValue"/>
    </label>

  </div>
  <button class="btn btn-primary" @click="handleSave">Salva</button>
</template>

<style scoped>

</style>