local sdl = require("SDL3")
local cursor = require("cursor")

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
local font = sdl.open_font(base_path .. "/usr/share/assets/Anonymous.ttf", 16.0)

local input = tostring(io.read())
local file
if input == "" then
	print("File not found. Opening init script.")
	file = assert(io.open(base_path .. "/usr/share/lua/test-lua-bindings.lua", "r"))
else
	file = io.open(input, "r")
end
if not file then
	print("File not found. Opening init script.")
	file = assert(io.open(base_path .. "/usr/share/lua/test-lua-bindings.lua", "r"))
end
local file_lines = file:lines()
local lines = {}

for line in file_lines do
	if line == "" then
		line = "\0"
	end
	table.insert(lines, line)
end

local offsety = 1

local function insert_char(char)
	local line = lines[offsety]
	line = line:sub(1, cursor.x - 1) .. char .. line:sub(cursor.x)
	lines[offsety] = line
	cursor.move_right()
end

local quit = false

sdl.start_text_input(window)
while not quit do
	local event = sdl.poll_event()
	if event then
		if event.key == 81 then
			if offsety < #lines - (width / height) then
				offsety = offsety + 1
			end
		end
		if event.key == 82 and offsety > 1 then
			offsety = offsety - 1
		end
		if event.key == 79 then
			cursor.move_right()
		end
		if event.key == 80 then
			cursor.move_left()
		end
		if event.key == 42 then
			if cursor.x > 1 then
				local updated_line = lines[offsety]:sub(1, cursor.x - 2) .. lines[offsety]:sub(cursor.x)
				cursor.move_left()
				if #updated_line > 0 then
					lines[offsety] = updated_line
				else
					lines[offsety] = "\0"
				end
			else
				if offsety ~= 1 then
					local line = lines[offsety]
					lines[offsety - 1] = lines[offsety - 1] .. line
					table.remove(lines, offsety)
					offsety = offsety - 1
					cursor.x = #lines[offsety] + 1
				end
			end
		end
		if event.key == 40 then
			local line = lines[offsety]
			local newline1 = line:sub(1, cursor.x)
			local newline2 = line:sub(cursor.x + 1)
			lines[offsety] = newline1
			if newline2 == "" then newline2 = "\0" end
			table.insert(lines, offsety + 1, newline2)
		end
		if event.type == 771 then
			insert_char(event.text)
		end
		if event.quit then
			quit = true
		end
	end

	cursor.check_bounds(#lines[offsety])

	sdl.set_draw_color(renderer, sdl.color(0, 0, 0, 255))
	sdl.render_clear(renderer)

	local i = 0
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

sdl.stop_text_input(window)

file:close()
sdl.close_font(font)
sdl.destroy_renderer(renderer)
sdl.destroy_window(window)

sdl.ttf_quit()
sdl.quit()

local new_file = ""

for _, line in pairs(lines) do
	if line == "\0" then
		new_file = new_file .. "\n"
	else
		new_file = new_file .. line .. "\n"
	end
end

local cool = assert(io.open("newfile.txt", "w"))
cool:write(new_file)
cool:close()
