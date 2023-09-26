set -xe

cc="clang"
cflags="-Wall -Wextra -Wpedantic -Iinclude -Isrc -D_CRT_SECURE_NO_WARNINGS -DFUDE_SHAREDLIB"
ldflags="-Llibs -lglfw3_mt -lgdi32 -luser32 -lshell32 -lopengl32"

if [ ! -d "./build" ]; then 
    mkdir "./build"
fi
if [ ! -d "./build/bin" ]; then 
    mkdir "./build/bin"
fi
if [ ! -d "./build/bin-int" ]; then 
    mkdir "./build/bin-int"
fi

$cc $cflags -DFUDE_EXPORT -c -o "./build/bin-int/fude_core.c.o"           "./src/fude_core.c"
$cc $cflags -DFUDE_EXPORT -c -o "./build/bin-int/fude_utils.c.o"          "./src/fude_utils.c"
$cc $cflags -DFUDE_EXPORT -c -o "./build/bin-int/fude_graphics.c.o"       "./src/fude_graphics.c"
$cc $cflags -DFUDE_EXPORT -c -o "./build/bin-int/fude_glfw.c.o"           "./src/fude_glfw.c"
$cc $cflags -DFUDE_EXPORT -c -o "./build/bin-int/glad.c.o"                "./src/glad/glad.c"

$cc $ldflags -shared -o "./build/bin/fude.dll" \
    ./build/bin-int/fude_core.c.o ./build/bin-int/fude_utils.c.o \
    ./build/bin-int/fude_glfw.c.o ./build/bin-int/fude_graphics.c.o \
    ./build/bin-int/glad.c.o

$cc $cflags -o ./build/bin/example.exe ./example/main.c $ldflags -Lbuild/bin -lfude
