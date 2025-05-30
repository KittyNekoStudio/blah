#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>

int luaopen_SDL3(lua_State *L); 

int main(int argc, char *argv[]) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	const char *root = getenv("HERE");
	char init_script[512];
	snprintf(init_script, sizeof(init_script), "%s/usr/share/lua/test-lua-bindings.lua", root);
	
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, luaopen_SDL3);
	lua_setfield(L, -2, "SDL3");
	lua_pop(L, 2);

	if (luaL_dofile(L, init_script) != LUA_OK) {
		fprintf(stderr, "Error in dofile init_script: %s\n",
					lua_tostring(L, -1));
		return 1;
	}

	lua_close(L);
	return 0;
}
