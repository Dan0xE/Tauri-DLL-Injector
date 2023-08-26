@echo off

echo running build script...
cd src-injector
cmd.exe /c gcc -shared -o inject.dll main.c -Wl,--output-def,exports.def,--out-implib,inject.lib
cd ..
cmd.exe /c yarn tauri build
cd src-injector
cmd.exe /c xcopy inject.dll "%cd%\..\src-tauri\target\release" /y
cd..
pause