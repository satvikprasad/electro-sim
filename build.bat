@echo off

SET include=-Iraylib\src
SET linker=raylib\src\libraylib.a -lgdi32 -lole32 -loleaut32 -limm32 -lwinmm

gcc main.c -o main.exe %include% %linker%
