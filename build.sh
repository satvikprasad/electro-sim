include="-Iraylib/src"
linker="raylib/src/libraylib.a"

gcc main.c -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -o main $include $linker
