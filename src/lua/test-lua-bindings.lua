local sdl = require("SDL3")

assert(sdl.init(0x00000020))
assert(sdl.ttf_init())

-- Get env variable inside AppImage
local base_path = os.getenv("HERE")
local window = sdl.create_window("From lua", 1000, 500, 0x0000000000000020)
local renderer = sdl.create_renderer(window)
local font = sdl.open_font(base_path .. "/usr/share/fonts/Anonymous.ttf", 16.0)

local file = assert(io.open(base_path .. "/usr/share/lua/test-lua-bindings.lua", "r"))
local file_contents = file:read("a")
local color = sdl.color(255, 255, 255, 255)
local surface = sdl.blended_text_surface(font, file_contents, file_contents:len(), color)
local texture = sdl.create_texture_from_surface(renderer, surface)
local dstrect = sdl.frect(100, 100, surface.w, surface.h)

sdl.destroy_surface(surface)

sdl.set_draw_color(renderer, sdl.color(0, 0, 0, 255))

sdl.render_clear(renderer)

sdl.render_texture(renderer, texture, nil, dstrect)

sdl.render_present(renderer)

local quit = false

while not quit do
	local event = sdl.poll_event()
	if event then
		if event.key == 20 then
			quit = true
		end
		if event.quit then
			quit = true
		end
	end
end

file:close()
sdl.destroy_texture(texture)
sdl.close_font(font)
sdl.destroy_renderer(renderer)
sdl.destroy_window(window)

sdl.ttf_quit()
sdl.quit()
