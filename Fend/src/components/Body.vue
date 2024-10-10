<script setup>

import Container from "@/components/container.vue";
import DateTime from "@/components/DateTime.vue";
import WIFI from "@/components/WIFI.vue";
</script>

<template>
  <div class="bg-gray-100 w-full h-full flex flex-col items-center justify-center space-y-6">

    <container title="Data e Ora">
      <date-time></date-time>
    </container>

    <container title="WiFi">
      <WIFI></WIFI>
    </container>
  </div>
</template>


<style scoped>

</style>