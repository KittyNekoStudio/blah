#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <SDL3/SDL.h>
#include <string.h>
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

static int lua_sdl_destroy_window(lua_State *L) {
	SDL_Window **window = luaL_checkudata(L, 1, "SDL_Window");
	if (*window) {
		SDL_DestroyWindow(*window);
		*window = NULL;
	}
	return 0;
}

static int lua_sdl_create_renderer(lua_State *L) {
	SDL_Window **window = luaL_checkudata(L, 1, "SDL_Window");
	SDL_Renderer *renderer = SDL_CreateRenderer(*window, 0);

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

static int lua_sdl_destroy_renderer(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");
	if (*renderer) {
		SDL_DestroyRenderer(*renderer);
		*renderer = NULL;
	}
	return 0;
}

static int lua_sdl_render_clear(lua_State *L) {
	SDL_Renderer **userdata = luaL_checkudata(L, 1, "SDL_Renderer");
	SDL_Renderer *renderer = *userdata;
	SDL_RenderClear(renderer);
	return 0;
}

static int lua_sdl_render_present(lua_State *L) {
	SDL_Renderer **userdata = luaL_checkudata(L, 1, "SDL_Renderer");
	SDL_Renderer *renderer = *userdata;
	SDL_RenderPresent(renderer);
	return 0;
}

static int lua_sdl_ttf_init(lua_State *L) {
	if (!TTF_Init()) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int lua_sdl_ttf_quit(lua_State *L) {
	TTF_Quit();
	return 0;
}

static int lua_sdl_ttf_create_renderer_text_engine(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");

	TTF_TextEngine *text_engine = TTF_CreateRendererTextEngine(*renderer);

	if (!text_engine) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	TTF_TextEngine **userdata = lua_newuserdata(L, sizeof(TTF_TextEngine*));
	*userdata = text_engine;

	luaL_getmetatable(L, "TTF_TextEngine");
	lua_setmetatable(L, -2);
	return 1;
}

static int lua_sdl_ttf_destroy_renderer_text_engine(lua_State *L) {
	TTF_TextEngine **text_engine = luaL_checkudata(L, 1, "TTF_TextEngine");

	if (*text_engine) {
		TTF_DestroyRendererTextEngine(*text_engine);
		*text_engine = NULL;
	}

	return 0;
}

static int lua_sdl_ttf_open_font(lua_State *L) {
	const char *file_path = luaL_checkstring(L, 1);
	float ptsize = luaL_checknumber(L, 2);
	TTF_Font *font = TTF_OpenFont(file_path, ptsize);

	if (!font) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	TTF_Font **userdata = lua_newuserdata(L, sizeof(TTF_Font*));
	*userdata = font;

	luaL_getmetatable(L, "TTF_Font");
	lua_setmetatable(L, -2);
	return 1;
}

static int lua_sdl_ttf_close_font(lua_State *L) {
	TTF_Font **font = luaL_checkudata(L, 1, "TTF_Font");

	if (*font) {
		TTF_CloseFont(*font);
		*font = NULL;
	}

	return 0;
}

static int lua_sdl_ttf_create_text(lua_State *L) {
	TTF_TextEngine **userdata_engine = luaL_checkudata(L, 1, "TTF_TextEngine");
	TTF_TextEngine *text_engine = *userdata_engine;

	TTF_Font **userdata_font = luaL_checkudata(L, 2, "TTF_Font");
	TTF_Font *font = *userdata_font;

	const char *text = luaL_checkstring(L, 3);
	int length = luaL_checkinteger(L, 4);

	TTF_Text *ttf_text = TTF_CreateText(text_engine, font, text, length);

	if (!ttf_text) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	TTF_Text **userdata = lua_newuserdata(L, sizeof(TTF_Text*));
	*userdata = ttf_text;

	luaL_getmetatable(L, "TTF_Text");
	lua_setmetatable(L, -2);
	return 1;
}

int lua_sdl_ttf_destroy_text(lua_State *L) {
	TTF_Text **text = luaL_checkudata(L, 1, "TTF_Text");

	if (*text) {
		TTF_DestroyText(*text);
		*text = NULL;
	}

	return 0;
}

int lua_sdl_draw_render_text(lua_State *L) {
	TTF_Text **userdata = luaL_checkudata(L, 1, "TTF_Text");
	TTF_Text *text = *userdata;
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);

	if (!TTF_DrawRendererText(text, x, y)) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	return 0;
}

static const struct luaL_Reg sdl_funcs[] = {
	{"init", lua_sdl_init},
	{"quit", lua_sdl_quit},
	{"create_window", lua_sdl_create_window},
	{"destroy_window", lua_sdl_destroy_window},
	{"create_renderer", lua_sdl_create_renderer},
	{"destroy_renderer", lua_sdl_destroy_renderer},
	{"render_clear", lua_sdl_render_clear},
	{"render_present", lua_sdl_render_present},
	{"ttf_init", lua_sdl_ttf_init},
	{"ttf_quit", lua_sdl_ttf_quit},
	{"create_text_engine", lua_sdl_ttf_create_renderer_text_engine},
	{"destroy_text_engine", lua_sdl_ttf_destroy_renderer_text_engine},
	{"open_font", lua_sdl_ttf_open_font},
	{"close_font", lua_sdl_ttf_close_font},
	{"create_text", lua_sdl_ttf_create_text},
	{"destroy_text", lua_sdl_ttf_destroy_text},
	{"draw_render_text", lua_sdl_draw_render_text},
	{NULL, NULL}
};

int luaopen_SDL3(lua_State *L) {
	luaL_newmetatable(L, "SDL_Window");
	lua_pop(L, 1);

	luaL_newmetatable(L, "SDL_Renderer");
	lua_pop(L, 1);

	// TODO! we segfault and I have a feeling it is because we are not handling
	// destruction ourselves
	luaL_newmetatable(L, "TTF_TextEngine");
	lua_pop(L, 1);

	luaL_newmetatable(L, "TTF_Font");
	lua_pop(L, 1);

	luaL_newmetatable(L, "TTF_Text");
	lua_pop(L, 1);

	luaL_newlib(L, sdl_funcs);
	return 1;
}
