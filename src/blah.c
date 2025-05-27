#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int luaopen_SDL3(lua_State* L); 

int main() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	luaL_requiref(L, "sdl3", luaopen_SDL3, 1);
	lua_pop(L, 1);

	if (luaL_dofile(L, "../src/test-lua-bindings.lua") != LUA_OK) {
		fprintf(stderr, "Error in dofile tast-lua-bindings: %s\n", 
					lua_tostring(L, -1));
		return 1;
	}

	lua_close(L);
	return 0;
}
