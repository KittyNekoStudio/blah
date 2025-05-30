local sdl = require("SDL3")

assert(sdl.init(0x00000020))
assert(sdl.ttf_init())

local width = 800
local height = 600
local line_height = 20
local visable_lines = height / line_height
-- Get env variable inside AppImage
local base_path = os.getenv("HERE")
local window = sdl.create_window("From lua", width, height, 0x0000000000000020)
local renderer = sdl.create_renderer(window)
local font = sdl.open_font(base_path .. "/usr/share/fonts/Anonymous.ttf", 16.0)

local file = assert(io.open(base_path .. "/usr/share/lua/test-lua-bindings.lua", "r"))
local file_lines = file:lines()
local lines = {}

for line in file_lines do
	if line == "" then
		line = "\0"
	end
	table.insert(lines, line)
end

local quit = false

local offsety = 0

while not quit do
	local event = sdl.poll_event()
	if event then
		if event.key == 20 then
			quit = true
		end
		if event.key == 81 then
			if offsety < #lines - (width / height) then
				offsety = offsety + 1
			end
		end
		if event.key == 82 and offsety > 0 then
			offsety = offsety - 1
		end
		if event.quit then
			quit = true
		end
	end

	sdl.set_draw_color(renderer, sdl.color(0, 0, 0, 255))
	sdl.render_clear(renderer)

	local i = 1
	while i < visable_lines do

		local line_index = i + offsety
		if line_index > #lines then break end

		local line = lines[line_index]
		local surface = sdl.blended_text_surface(font, line, line:len(), sdl.color(255, 255, 255, 255))
		local texture = sdl.create_texture_from_surface(renderer, surface)
		local dst = sdl.frect(10, i * line_height, surface.w, surface.h)
		sdl.render_texture(renderer, texture, nil, dst)

		sdl.destroy_surface(surface)
		sdl.destroy_texture(texture)

		i = i + 1
	end

	sdl.render_present(renderer)
end

file:close()
sdl.close_font(font)
sdl.destroy_renderer(renderer)
sdl.destroy_window(window)

sdl.ttf_quit()
sdl.quit()
