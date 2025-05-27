#!/bin/sh

clang -shared -fPIC -o build/SDL3.so src/lua-sdl3-bindings.c -lSDL3 -lSDL3_ttf
clang src/lua-sdl3-bindings.c src/blah.c -o build/blah -llua -Llib -lSDL3 -lSDL3_ttf
