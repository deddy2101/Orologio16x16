<script setup>

</script>

<template>
  <div class="bg-gray-100 w-full h-full flex flex-col">
    <h1 class="text-2xl font-bold text-center">Impostazioni</h1>
    <div class="bg-gray-200  p-3 flex flex-row self-center w-3/4 content-center	">
      <label class="form-control w-full max-w-xs">
        <div class="label">
          <span class="label-text">DATA E ORA DA IMPOSTARE</span>
        </div>
        <input type="datetime-local" placeholder="Type here" class="input input-bordered w-full max-w-xs" />

      </label>
      <button class="btn btn-primary">salva</button>
    </div>
    <label class="form-control w-full max-w-xs">
      <div class="label">
        <span class="label-text">AP ?</span>
      </div>
      <input type="checkbox" class="toggle" checked="checked" />
    </label>
    <input type="text" placeholder="SSID" class="input input-bordered w-full max-w-xs" />
    <input type="text" placeholder="PASSWD" class="input input-bordered w-full max-w-xs" />


  </div>
</template>

<style scoped>

</style>