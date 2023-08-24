<script lang="ts">
  import { invoke } from "@tauri-apps/api/tauri"
  import { open } from "@tauri-apps/api/dialog"

  let pid: number;
  let message = "";
  let dllPath = "";

  async function select() {
    open({
      filters: [{
        name: "",
        extensions: ['dll']
      }]
    }).then((res: string) => res ? dllPath  = res : message = "No DLL provided")
  }

  async function inject(){
    message = await invoke("inject", { pid, dllPath })
  }
</script>

<div>
    <input id="greet-input" placeholder="Enter a PID" bind:value={pid} />
    <button on:click={() => select()}>Select DLL</button>
    {#if dllPath}<button on:click={() => inject()}>Inject</button>{/if}
  <p>{message}</p>
</div>