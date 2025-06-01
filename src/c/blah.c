#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>

int luaopen_SDL3(lua_State *L); 

int main(int argc, char *argv[]) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	char *root = getenv("HERE");
	char init_script[512];
	snprintf(init_script, sizeof(init_script), "%s/usr/share/lua/init.lua", root);
	
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, luaopen_SDL3);
	lua_setfield(L, -2, "SDL3");
	lua_pop(L, 2);

	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	const char *current_path = luaL_checkstring(L, -1);

	char path[512];
	snprintf(path, sizeof(path), "%s/usr/share/lua/?.lua", root);

	lua_pop(L, 1);
	lua_pushstring(L, path);
	lua_setfield(L, -2, "path");

	lua_newtable(L);
	for (int i = 0; i < argc; i++) {
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
	}
	lua_setglobal(L, "arg");

	if (luaL_dofile(L, init_script) != LUA_OK) {
		fprintf(stderr, "Error in dofile init_script: %s\n",
					lua_tostring(L, -1));
		return 1;
	}

	lua_close(L);
	return 0;
}
