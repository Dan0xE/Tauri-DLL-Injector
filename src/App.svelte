<script lang="ts">
  import Inject from './lib/Inject.svelte'
</script>

<main class="container">
  <h1>Welcome to the Tauri DLL Injector!</h1>

  <p>
  Enter a process PID, select a DLL and click Inject
  </p>

  <div class="row">
    <Inject />
  </div>
</main>

