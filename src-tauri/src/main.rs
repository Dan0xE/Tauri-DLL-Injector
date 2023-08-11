// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use std::ffi::CString;

#[link(name = "inject", kind = "static")]
extern "C" {
    fn InjectDLL(process_id: i32, dll_path: *const u8) -> bool;
}

#[tauri::command]
fn inject(pid: String, dll_path: String) -> String {
    let c_string = CString::new(dll_path).unwrap();
    let ptr = c_string.as_ptr() as *const u8;

    //parsing can fail but since this is just a demo we dont care
    if unsafe { InjectDLL(pid.parse::<i32>().unwrap(), ptr) } {
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
