#include <SDL3/SDL_render.h>
#include <lua.h>
#include <lauxlib.h>
#include <SDL3/SDL.h>
#include <sys/types.h>

static int lua_sdl_init(lua_State *L) {
	int flags = luaL_checkinteger(L, 1);
	if (!SDL_Init(flags)) {
			lua_pushnil(L);
			lua_pushstring(L, SDL_GetError());
			return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int lua_sdl_quit(lua_State *L) {
	SDL_Quit();
	return 0;
}

static int lua_sdl_create_window(lua_State *L) {
	const char *title = luaL_checkstring(L, 1);
	int width = luaL_checkinteger(L, 2);
	int height = luaL_checkinteger(L, 3);
	uint64_t flags = luaL_checkinteger(L, 4);
	SDL_Window *window = SDL_CreateWindow(title, width, height, flags);

	if (!window) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	SDL_Window **userdata = lua_newuserdata(L, sizeof(SDL_Window*));
	*userdata = window;

	luaL_getmetatable(L, "SDL_Window");
	lua_setmetatable(L, -2);
	return 1;
}

int lua_sdl_destroy_window(lua_State *L) {
	SDL_Window **window = luaL_checkudata(L, 1, "SDL_Window");
	if (*window) {
		SDL_DestroyWindow(*window);
		*window = NULL;
	}
	return 0;
}

int lua_sdl_create_renderer(lua_State *L) {
	SDL_Window **maybe_window = luaL_checkudata(L, 1, "SDL_Window");
	SDL_Window *window = *maybe_window;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);

	if (!renderer) {
			lua_pushnil(L);
			lua_pushstring(L, SDL_GetError());
			return 2;
		}

	SDL_Renderer **userdata = lua_newuserdata(L, sizeof(SDL_Renderer*));
	*userdata = renderer;

	luaL_getmetatable(L, "SDL_Renderer");
	lua_setmetatable(L, -2);
	return 1;
}

int lua_sdl_destroy_renderer(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");
	if (*renderer) {
		SDL_DestroyRenderer(*renderer);
		*renderer = NULL;
	}
	return 0;
}

int lua_sdl_render_clear(lua_State *L) {
	SDL_Renderer **userdata = luaL_checkudata(L, 1, "SDL_Renderer");
	SDL_Renderer *renderer = *userdata;
	SDL_RenderClear(renderer);
	return 0;
}

int lua_sdl_render_present(lua_State *L) {
	SDL_Renderer **userdata = luaL_checkudata(L, 1, "SDL_Renderer");
	SDL_Renderer *renderer = *userdata;
	SDL_RenderPresent(renderer);
	return 0;
}

static const struct luaL_Reg sdl_funcs[] = {
	{"init", lua_sdl_init},
	{"quit", lua_sdl_quit},
	{"create_window", lua_sdl_create_window},
	{"create_renderer", lua_sdl_create_renderer},
	{"render_clear", lua_sdl_render_clear},
	{"render_present", lua_sdl_render_present},
	{NULL, NULL}
};

int luaopen_SDL3(lua_State *L) {
	luaL_newmetatable(L, "SDL_Window");
	lua_pushcfunction(L, lua_sdl_destroy_window);
	lua_setfield(L, -2, "__gc");
	lua_pop(L, 1);

	luaL_newmetatable(L, "SDL_Renderer");
	lua_pushcfunction(L, lua_sdl_destroy_renderer);
	lua_setfield(L, -2, "__gc");
	lua_pop(L, 1);

	luaL_newlib(L, sdl_funcs);
	return 1;
}
