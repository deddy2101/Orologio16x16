<script setup>

import Container from "@/components/container.vue";
import DateTime from "@/components/DateTime.vue";
import WIFI from "@/components/WIFI.vue";
import DimTimes from "@/components/dimTimes.vue";
import Snow from "@/components/Snow.vue";
</script>

<template>
  <div class="bg-gray-100 w-full h-full flex flex-col items-center justify-center space-y-6">

    <container title="Data e Ora">
      <date-time></date-time>
    </container>
    <container title="Periodo Buio">
      <dim-times></dim-times>
    </container>

    <container title="WiFi">
      <WIFI></WIFI>
    </container>
    <container title="SNOW">
      <Snow></Snow>
    </container>
  </div>
</template>


<style scoped>

</style>