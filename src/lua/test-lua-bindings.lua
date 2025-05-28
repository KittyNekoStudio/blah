local sdl = require("SDL3")


assert(sdl.init(0x00000020))
assert(sdl.ttf_init())

-- Get env variable inside AppImage
local base_path = os.getenv("HERE")
local window = sdl.create_window("From lua", 1000, 500, 0)
local renderer = sdl.create_renderer(window)
local text_engine = sdl.create_text_engine(renderer)
local font = sdl.open_font(base_path .. "/usr/share/fonts/Anonymous.ttf", 12.0)

local file = assert(io.open(base_path .. "/usr/share/lua/test-lua-bindings.lua", "r"))
local file_contents = file:read("a")
local text = sdl.create_text(text_engine, font, file_contents, string.len(file_contents))

sdl.render_clear(renderer)

sdl.draw_render_text(text, 100, 100)

sdl.render_present(renderer)

local running = true

while running do
	local event = sdl.poll_event()
	if event then
		if event.key == 20 then
			running = false
		end
	end
end

file:close()
sdl.destroy_text_engine(text_engine)
sdl.close_font(font)
sdl.destroy_text(text)
sdl.destroy_renderer(renderer)
sdl.destroy_window(window)

sdl.ttf_quit()
sdl.quit()
