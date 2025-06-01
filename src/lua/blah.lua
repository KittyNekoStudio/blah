local sdl = require("SDL3")
local blah = {
	window = {
		w = 800,
		h = 600
	},
	renderer = {},
  cursor = require("cursor"),
  root = os.getenv("HERE"),
	file = {
	  file = io.open(arg[1], "r"),
		line_height = 20,
		lines = {}
	}
}

function blah.init()
	assert(sdl.init(0x00000020))
	assert(sdl.ttf_init())
	blah.window.window = sdl.create_window("blah", blah.window.w, blah.window.h, 0x0000000000000020)
	blah.renderer.renderer = sdl.create_renderer(blah.window.window)
	blah.file.font = sdl.open_font(blah.root .. "/usr/share/assets/Anonymous.ttf", 16.0)

	for line in blah.file.file:lines() do
		if line == "" then
				line = "\0"
			end
			table.insert(blah.file.lines, line)
	end
end

function blah.quit()
	sdl.close_font(blah.file.font)
	sdl.destroy_renderer(blah.renderer.renderer)
	sdl.destroy_window(blah.window.window)
	blah.file.file:close()
end

function blah.render()
	sdl.set_draw_color(blah.renderer.renderer, sdl.color(0, 0, 0, 255))
	sdl.render_clear(blah.renderer.renderer)

	local i = 0
	local visable_lines = blah.window.h / blah.file.line_height
	while i < visable_lines do
		local line_index = i + blah.cursor.y
		if line_index > #blah.file.lines then break end

		local line = blah.file.lines[line_index]
		local surface = sdl.blended_text_surface(blah.file.font, line, line:len(), sdl.color(255, 255, 255, 255))
		local texture = sdl.create_texture_from_surface(blah.renderer.renderer, surface)
		local dst = sdl.frect(10, i * blah.file.line_height, surface.w, surface.h)
		sdl.render_texture(blah.renderer.renderer, texture, nil, dst)

		sdl.destroy_surface(surface)
		sdl.destroy_texture(texture)

		i = i + 1
	end

	sdl.set_draw_color(blah.renderer.renderer, sdl.color(255, 255, 255, 255))
	sdl.render_rect(blah.renderer.renderer, sdl.frect(blah.cursor.x * 11, 0, blah.cursor.w, blah.cursor.h))
	sdl.render_present(blah.renderer.renderer)
end

function blah.event_loop()
	local quit = false

	sdl.start_text_input(blah.window.window)
	while not quit do
		local event = sdl.poll_event()
		if event then
			if event.key == 81 then
				if blah.cursor.y < #blah.file.lines then
					blah.cursor.move_down()
				end
			end
			if event.key == 82 then
					blah.cursor.move_up()
			end
			if event.key == 79 then
				blah.cursor.move_right()
			end
			if event.key == 80 then
				blah.cursor.move_left()
			end
			if event.key == 42 then
				if blah.cursor.x > 1 then
					local updated_line = blah.file.lines[blah.cursor.y]:sub(1, blah.cursor.x - 2)
					.. blah.file.lines[blah.cursor.y]:sub(blah.cursor.x)
					blah.cursor.move_left()
					if #updated_line > 0 then
						blah.file.lines[blah.cursor.y] = updated_line
					else
						blah.file.lines[blah.cursor.y] = "\0"
					end
				else
					if blah.cursor.y ~= 1 then
						local line = blah.file.lines[blah.cursor.y]
						blah.file.lines[blah.cursor.y - 1] = blah.file.lines[blah.cursor.y - 1] .. line
						table.remove(blah.file.lines, blah.cursor.y)
						blah.cursor.move_up()
						blah.cursor.x = #blah.file.lines[blah.cursor.y] + 1
					end
				end
			end
			if event.key == 40 then
				local line = blah.file.lines[blah.cursor.y]
				local newline1 = line:sub(1, blah.cursor.x)
				local newline2 = line:sub(blah.cursor.x + 1)
				blah.file.lines[blah.cursor.y] = newline1
				if newline2 == "" then newline2 = "\0" end
				table.insert(blah.file.lines, blah.cursor.y + 1, newline2)
			end
			if event.type == 771 then
				local line = blah.file.lines[blah.cursor.y]
				line = line:sub(1, blah.cursor.x - 1) .. event.text .. line:sub(blah.cursor.x)
				blah.file.lines[blah.cursor.y] = line
				blah.cursor.move_right()
			end
			if (event.mod == 64 or event.mod == 128) and event.key == 22 then
				blah.save()
			end
			if event.quit then
				quit = true
			end
		end

		blah.cursor.check_bounds(#blah.file.lines[blah.cursor.y])
		blah.render()

	end
	sdl.stop_text_input(blah.window.window)
end

function blah.save()
	local new_file = ""
	for _, line in pairs(blah.file.lines) do
		line = line:gsub("\0", "")
		new_file = new_file .. line .. "\n"
	end
	local cool = assert(io.open(arg[1], "w"))
	cool:write(new_file)
	cool:close()
end
return blah
