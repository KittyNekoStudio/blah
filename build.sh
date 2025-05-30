#!/bin/sh

if [[ $1 == "nob" ]]; then
	cd build
	./blah.AppImage
	exit 0
fi

clang src/c/lua-sdl3-bindings.c src/c/blah.c -o blah -llua -Llib -lSDL3 -lSDL3_ttf
mv blah blah.AppDir/usr/bin
cp src/lua -r blah.AppDir/usr/share
cd build
ARCH=x86_64 ./appimagetool-x86_64.AppImage ../blah.AppDir blah.AppImage

if [[ $1 == "run" ]]; then
	./blah.AppImage
fi
