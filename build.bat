@echo off

SET include=-Ilib\raylib\src
SET linker=lib\raylib\src\libraylib.a -lgdi32 -lole32 -loleaut32 -limm32 -lwinmm

gcc src\*.c -o main.exe %include% %linker%
