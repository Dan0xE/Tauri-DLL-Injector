# Tauri DLL Injector

A DLL injector built using Tauri, with a backend written in Rust and C.

## Getting Started

### Prerequisites

- Rust
- GCC for compiling the C backend
- Node.js and npm (or any other package manager)

### Setup

1. Clone the repository:

```bash
git clone https://github.com/dan0xe/tauri-dll-injector.git
cd tauri-dll-injector
```

- src-tauri -> rust tauri backend
- src-injector -> c injector backend
- src -> typescript / svelte frontend

2. Install frontend dependencies:

```bash
npm install
```

3. Build the C backend:

You can either use the provided batch build script or copy and paste this:

```bash
gcc -shared -o inject.dll main.c -Wl,--output-def,exports.def,--out-implib,inject.lib
```

then copy and paste this lib and the dll file into src-tauri

4. Launch the Tauri application:

```bash
npm run tauri dev
```

## Usage

1. Launch the process you want to inject to
2. Type the process id into the text field
3. Click on "Select DLL" and select the DLL you want to inject

## Contributing

Contributions are welcome!

## License

This project is licensed under the MIT License

## Acknowledgments

- [Tauri](https://tauri.app/) For providing a lightweight framework to build cross-platform applications.
- [Tsoding](https://github.com/tsoding) For coding inspiration.
