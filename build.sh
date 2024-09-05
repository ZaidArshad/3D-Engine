cpp_ver=c++17
compiler_options="-g -Wall"
include_dir=include
src_files=$(find "src" -name "*.cpp" -o -name "*.c")
out_file=app

if [ "$OSTYPE" == "msys" ]; then
    compiler=C:/msys64/mingw64/bin/g++.exe
    lib_dir=lib/win64
    extra_args=-lglfw3dll
elif [[ "$OSTYPE" == "darwin"* ]]; then
    compiler=/usr/bin/g++
    lib_dir=lib/mac
    extra_args="lib/mac/libglfw.3.4.dylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated"
else
    echo ERROR: Script not equipped with valid os: $OSTYPE
    exit 1
fi 

rm app
set -o xtrace

#example C:/msys64/mingw64/bin/g++.exe -std=c++17 -Wall -g "-Iinclude" "-Llib/win64" "src/main.cpp" "src/glad.c" -lglfw3dll -o "app"
$compiler -std=$cpp_ver $compiler_options "-I${include_dir}" "-L${lib_dir}" $src_files $extra_args -o $out_file
