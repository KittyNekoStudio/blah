local sdl = require("SDL3")

assert(sdl.init(0x00000020))
assert(sdl.ttf_init())

local window = sdl.create_window("From lua", 1000, 500, 0)
local renderer = sdl.create_renderer(window)
local text_engine = sdl.create_text_engine(renderer)
local font = sdl.open_font("../Anonymous.ttf", 12.0)
local text = sdl.create_text(text_engine, font, "Hello, World!", 13)

sdl.render_clear(renderer)

sdl.draw_render_text(text, 100, 100)

sdl.render_present(renderer)

os.execute("sleep " .. 5)

sdl.ttf_quit()
sdl.quit()
