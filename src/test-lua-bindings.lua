local sdl = require("SDL3")

assert(sdl.init(0x00000020))

local window = assert(sdl.create_window("From lua", 1000, 500, 0))
local renderer = sdl.create_renderer(window)

sdl.render_clear(renderer)
sdl.render_present(renderer)
os.execute("sleep " .. 5)

sdl.quit()
