local sdl = require("SDL3")

assert(sdl.init(0x00000020))
assert(sdl.ttf_init())

local window = sdl.create_window("From lua", 1000, 500, 0)
local renderer = sdl.create_renderer(window)
local text_engine = sdl.create_text_engine(renderer)
local font = sdl.open_font("../Anonymous.ttf", 12.0)
local text_literal = "Hello, World! It is so cool man."
local text = sdl.create_text(text_engine, font, text_literal, string.len(text_literal))

sdl.render_clear(renderer)

sdl.draw_render_text(text, 100, 100)

sdl.render_present(renderer)

os.execute("sleep " .. 1)

sdl.destroy_text_engine(text_engine)
sdl.close_font(font)
sdl.destroy_text(text)
sdl.destroy_renderer(renderer)
sdl.destroy_window(window)

sdl.ttf_quit()
sdl.quit()
