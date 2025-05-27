#!/bin/sh

clang -shared -fPIC -o src/SDL3.so src/lua-sdl3-bindings.c -lSDL3
