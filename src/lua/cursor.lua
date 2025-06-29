local cursor = {
	x = 1,
	y = 1,
	w = 12,
	h = 20,
}

function cursor.move_right()
	cursor.x = cursor.x + 1
end

function cursor.move_left()
	if cursor.x > 1 then cursor.x = cursor.x - 1 end
end

function cursor.move_up()
	if cursor.y > 1 then cursor.y = cursor.y - 1 end
end

function cursor.move_down()
	cursor.y = cursor.y + 1
end

function cursor.check_bounds(xbound)
	if cursor.x > xbound then cursor.x = xbound + 1 end
end

return cursor
