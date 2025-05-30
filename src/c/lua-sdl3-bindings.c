#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>

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

static int lua_sdl_create_texture_from_surface(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");
	if (!*renderer) {
		lua_pushnil(L);
		lua_pushstring(L, "Not a renderer");
		return 2;
	}

	SDL_Surface **surface = luaL_checkudata(L, 2, "SDL_Surface");
	if (!*renderer) {
			lua_pushnil(L);
			lua_pushstring(L, "Not a surface");
			return 2;
		}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(*renderer, *surface);
	if (!texture) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	SDL_Texture **userdata = lua_newuserdata(L, sizeof(SDL_Texture*));
	*userdata = texture;

	luaL_getmetatable(L, "SDL_Texture");
	lua_setmetatable(L, -2);
	return 1;
}

static int lua_sdl_destroy_texture(lua_State *L) {
	SDL_Texture **texture = luaL_checkudata(L, 1, "SDL_Texture");
	if (*texture) {
		SDL_DestroyTexture(*texture);
		*texture = NULL;
	}

	return 0;
}

static int lua_sdl_create_color(lua_State *L) {
	uint8_t red = luaL_checknumber(L, 1);
	uint8_t green = luaL_checknumber(L, 2);
	uint8_t blue = luaL_checknumber(L, 3);
	uint8_t alpha = luaL_checknumber(L, 4);

	SDL_Color color = {red, green, blue, alpha};

	SDL_Color *userdata = lua_newuserdata(L, sizeof(SDL_Color));
	*userdata = color;

	luaL_getmetatable(L, "SDL_Color");
	lua_setmetatable(L, -2);
	return 1;
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
	if (!SDL_RenderPresent(renderer)) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}
	return 0;
}

static int lua_sdl_set_render_draw_color(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");
	if (!*renderer) {
		lua_pushnil(L);
		lua_pushstring(L, "Not a renderer");
		return 2;
	}

	SDL_Color *color = luaL_checkudata(L, 2, "SDL_Color");

	if (!SDL_SetRenderDrawColor(*renderer, color->r, color->g, color->b, color->a)) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}
	return 0;
}

static int lua_sdl_render_texture(lua_State *L) {
	SDL_Renderer **renderer = luaL_checkudata(L, 1, "SDL_Renderer");
	if (!*renderer) {
		lua_pushnil(L);
		lua_pushstring(L, "Not a renderer");
		return 2;
	}

	SDL_Texture **texture = luaL_checkudata(L, 2, "SDL_Texture");
	if (!*texture) {
		lua_pushnil(L);
		lua_pushstring(L, "Not a texture");
		return 2;
	}
	

	SDL_FRect **srcrect;
	if (lua_isnil(L, 3)) {
		*srcrect = NULL;
	} else {
		srcrect = luaL_checkudata(L, 3, "SDL_FRect");
	}

	SDL_FRect *dstrect = luaL_checkudata(L, 4, "SDL_FRect");
	if (!SDL_RenderTexture(*renderer, *texture, *srcrect, dstrect)) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}
	return 0;
}

static int lua_sdl_create_frect(lua_State *L) {
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float w = luaL_checknumber(L, 3);
	float h = luaL_checknumber(L, 4);

	SDL_FRect frect = {x, y, w, h};
	
	SDL_FRect *userdata = lua_newuserdata(L, sizeof(SDL_FRect));
	*userdata = frect;

	luaL_getmetatable(L, "SDL_FRect");
	lua_setmetatable(L, -2);
	return 1;
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


static int lua_sdl_ttf_render_text_blended(lua_State *L) {
	TTF_Font **font = luaL_checkudata(L, 1, "TTF_Font");
	if (!font) {
			lua_pushnil(L);
			lua_pushstring(L, SDL_GetError());
			return 2;
	}
	const char *text = luaL_checkstring(L, 2);
	int text_length = luaL_checkinteger(L, 3);
	SDL_Color *color = luaL_checkudata(L, 4, "SDL_Color");
	SDL_Surface *surface = TTF_RenderText_Blended(*font, text, text_length, *color);

	if (!surface) {
			lua_pushnil(L);
			lua_pushstring(L, SDL_GetError());
			return 2;
	}


	SDL_Surface **userdata = lua_newuserdata(L, sizeof(SDL_Surface*));
	*userdata = surface;	

	luaL_getmetatable(L, "SDL_Surface");
	lua_setmetatable(L, -2);
	return 1;
}

static int lua_sdl_destroy_surface(lua_State *L) {
	SDL_Surface **surface = luaL_checkudata(L, 1, "SDL_Surface");

	if (*surface) {
		SDL_DestroySurface(*surface);
		*surface = NULL;
	}
	return 0;
}

static int lua_sdl_surface_index(lua_State *L) {
	SDL_Surface **userdata = luaL_checkudata(L, 1, "SDL_Surface");
	if (!*userdata) {
		lua_pushnil(L);
		lua_pushstring(L, "Not a surface");
		return 2;
	}

	SDL_Surface *surface = *userdata;
	const char *key = luaL_checkstring(L, 2);

	if (strcmp(key, "w") == 0) {
			lua_pushinteger(L, surface->w);
			return 1;
	} else if (strcmp(key, "h") == 0) {
			lua_pushinteger(L, surface->h);
			return 1;
	}

	return 0;
}

static int lua_sdl_poll_event(lua_State *L) {
	SDL_Event event;
	
	if (SDL_PollEvent(&event)) {
		lua_newtable(L);
		
		lua_pushstring(L, "type");
		lua_pushinteger(L, event.type);
		lua_settable(L, -3);

		if (event.type == SDL_EVENT_KEY_DOWN) {
			lua_pushstring(L, "key");
			lua_pushinteger(L, event.key.scancode);
			lua_settable(L, -3);
		}

		if (event.type == SDL_EVENT_QUIT) {
			lua_pushstring(L, "quit");
			lua_pushboolean(L, 1);
			lua_settable(L, -3);
		}

		return 1;
	}
	
	lua_pushnil(L);
	return 1;
}

static const struct luaL_Reg sdl_funcs[] = {
	{"init", lua_sdl_init},
	{"quit", lua_sdl_quit},
	{"create_window", lua_sdl_create_window},
	{"destroy_window", lua_sdl_destroy_window},
	{"create_renderer", lua_sdl_create_renderer},
	{"destroy_renderer", lua_sdl_destroy_renderer},
	{"create_texture_from_surface", lua_sdl_create_texture_from_surface},
	{"destroy_texture", lua_sdl_destroy_texture},
	{"blended_text_surface", lua_sdl_ttf_render_text_blended},
	{"destroy_surface", lua_sdl_destroy_surface},
	{"render_clear", lua_sdl_render_clear},
	{"render_present", lua_sdl_render_present},
	{"render_texture", lua_sdl_render_texture},
	{"color", lua_sdl_create_color},
	{"set_draw_color", lua_sdl_set_render_draw_color},
	{"ttf_init", lua_sdl_ttf_init},
	{"ttf_quit", lua_sdl_ttf_quit},
	{"open_font", lua_sdl_ttf_open_font},
	{"close_font", lua_sdl_ttf_close_font},
	{"poll_event", lua_sdl_poll_event},
	{"frect", lua_sdl_create_frect},
	{NULL, NULL}
};

int luaopen_SDL3(lua_State *L) {
	luaL_newmetatable(L, "SDL_Window");
	lua_pop(L, 1);

	luaL_newmetatable(L, "SDL_Renderer");
	lua_pop(L, 1);

	luaL_newmetatable(L, "SDL_Surface");
	lua_pushcfunction(L, lua_sdl_surface_index);
	lua_setfield(L, -2, "__index");
	lua_pop(L ,1);

	luaL_newmetatable(L, "SDL_Texture");
	lua_pop(L ,1);

	luaL_newmetatable(L, "SDL_Color");
	lua_pop(L, 1);

	luaL_newmetatable(L, "SDL_FRect");
	lua_pop(L, 1);

	luaL_newmetatable(L, "TTF_Font");
	lua_pop(L, 1);

	luaL_newlib(L, sdl_funcs);

	return 1;
}
