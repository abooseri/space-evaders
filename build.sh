#!/bin/bash

pushd raylib/src
export MACOSX_DEPLOYMENT_TARGET=10.9
make PLATFORM=PLATFORM_DESKTOP
popd

mkdir bin
pushd bin
clang++ -g -std=c++11 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL ../src/engine.cpp -o SpaceEvaders -I ../src/ -I ../src/header/ -I ../raylib/src -L ../raylib/src/  -lraylib
popd