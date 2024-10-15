<script setup>
import Header from "@/components/Header.vue";
import Body from "@/components/Body.vue";
</script>

<template>
  <div class="flex flex-col" style="height: 100vh; width: 100vw">
    <Header/>
    <Body class="overflow-scroll pt-3" style="place-self: self-end"/>
  </div>

</template>

<style scoped>



</style>
