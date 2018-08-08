local background = DisplayImage.new(Texture.new("background.png"))

local cursor = DisplayImage.new(Texture.new("cursor.png", 0, 0))
cursor:addEventListener(Event.MOUSE_DOWN, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)
cursor:addEventListener(Event.MOUSE_MOVE, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)
cursor:addEventListener(Event.MOUSE_UP, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)
cursor:addEventListener(Event.TOUCH_BEGIN, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)
cursor:addEventListener(Event.TOUCH_MOVE, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)
cursor:addEventListener(Event.TOUCH_END, function(d, e) d:setPosition(e.info.x, e.info.y) end, cursor)

stage:addChild(background)
stage:addChild(cursor)
