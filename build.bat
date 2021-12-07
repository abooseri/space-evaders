@echo off

pushd raylib\src
mingw32-make PLATFORM=PLATFORM_DESKTOP
popd

mkdir bin
pushd bin
g++ ../src/engine.cpp -o SpaceEvaders.exe -I ../src/ -I ../src/header/ -I ../raylib/src/ -ggdb -L ../raylib/src/ -lraylib -lopengl32 -lgdi32 -lwinmm  
popd