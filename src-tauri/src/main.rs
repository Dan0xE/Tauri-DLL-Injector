#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use std::ffi::CString;

use tauri::{Manager, Window};

#[link(name = "inject", kind = "static")]
extern "C" {
    fn InjectDLL(process_id: i32, dll_path: *const u8) -> bool;
}

#[tauri::command]
fn inject(window: Window, pid: String, dll_path: String) -> String {
    let c_string = CString::new(dll_path).unwrap();
    let ptr = c_string.as_ptr() as *const u8;

    let pid_parsed = match pid.parse::<i32>() {
        Ok(pid) => pid,
        Err(e) => {
            window
                .emit_all("error", format!("failed to parse pid: {}", e))
                .unwrap();
            0
        }
    };
    //parsing can fail but since this is just a demo we dont care
    if unsafe { InjectDLL(pid_parsed, ptr) } {
        "Success".to_string()
    } else {
        "Failed to inject".to_string()
    }
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![inject])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
