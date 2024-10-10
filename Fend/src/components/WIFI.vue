<script setup lang="ts">
import {onMounted, ref} from 'vue';
import useApi from "@/composables/useAPI";

const {
  getSSID,
  getWifiPassword,
  getSTA,
  setWifiSettings
} = useApi();
const ssid = ref<string>('');
const password = ref<string>('');
const isAccessPoint = ref<boolean>(false);
onMounted(async () => {
  const responseSSID = await getSSID();
  const responsePassword = await getWifiPassword();
  const responseSTA = await getSTA();
  ssid.value = responseSSID.message;
  password.value = responsePassword.message;
  isAccessPoint.value = responseSTA.message === "false";
});

const handleSave = async () => {
  try {
    const response = await setWifiSettings(ssid.value, password.value, !isAccessPoint.value);
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
  <label class="form-control w-full max-w-xs mb-4">
    <div class="label">
      <span class="label-text">CLIENT/ACCESS POINT</span>
    </div>
    <input type="checkbox" class="toggle" v-model="isAccessPoint"/>
  </label>
  <input type="text" placeholder="SSID" class="input input-bordered w-full max-w-xs mb-4" v-model="ssid"
         :disabled="isAccessPoint"/>
  <input type="text" placeholder="PASSWD" class="input input-bordered w-full max-w-xs mb-4" v-model="password"
         :disabled="isAccessPoint"/>
  <button class="btn btn-primary" @click="handleSave">Salva</button>
</template>

<style scoped>
</style>