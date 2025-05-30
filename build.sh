#!/bin/sh

if [[ $1 == "nob" ]]; then
	cd build
	./blah.AppImage
	exit 0
fi

if [[ -d build/ ]]; then
	clang src/c/lua-sdl3-bindings.c src/c/blah.c -o blah -llua -Llib -lSDL3 -lSDL3_ttf
	mv blah blah.AppDir/usr/bin
	cp src/lua -r blah.AppDir/usr/share
	cp src/c -r blah.AppDir/usr/share
	cd build

	runtime=(runtime-*)
	appimagetool=(appimagetool-*)
	
	if [[ ! -f $appimagetool ]]; then
		echo "Please put appimagetool in the build directory"
		exit 0
	fi

	if [[ -f $runtime ]]; then
		ARCH=x86_64 ./appimagetool-x86_64.AppImage -n --runtime-file runtime-x86_64 ../blah.AppDir blah.AppImage
	else
		ARCH=x86_64 ./appimagetool-x86_64.AppImage -n ../blah.AppDir blah.AppImage
	fi

fi

if [[ $1 == "run" ]]; then
	./blah.AppImage
fi
