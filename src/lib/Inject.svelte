<script lang="ts">
  import { invoke } from "@tauri-apps/api/tauri"
  import { open } from "@tauri-apps/api/dialog"
  import {emit, listen} from '@tauri-apps/api/event'
  import { onMount } from "svelte";

  let pid: number;
  let message = "";
  let dllPath = "";
  let b_message = "";

  onMount(async () => {
    async function listenErrorEvent() {
     await listen('error', function(e) {
        b_message = e.payload as string;
      })
    }
    listenErrorEvent()
    return b_message;
  })

  async function select() {
    open({
      filters: [{
        name: "",
        extensions: ['dll']
      }]
    }).then((res: string) => res ? dllPath  = res : message = "No DLL provided")
  }

  async function inject(){
    b_message = ""
    message = await invoke("inject", { pid, dllPath })
  }
</script>

<div>
    <input id="greet-input" placeholder="Enter a PID" bind:value={pid} />
    <button on:click={() => select()}>Select DLL</button>
    {#if dllPath}<button on:click={() => inject()}>Inject</button>{/if}
  <p>{message}</p>
  <p>{b_message}</p>
</div>