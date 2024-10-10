<script setup>
const props = defineProps({
  title: {
    type: String,
    required: true
  }
})

</script>

<template>
  <div class="bg-gray-200 rounded-2xl p-6 flex flex-col items-center w-3/4">
    <h1 class="text-2xl font-bold text-center mb-4">{{ props.title }}</h1>
    <slot></slot>
  </div>
</template>

<style scoped>

</style>