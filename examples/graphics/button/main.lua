local stage = Stage.new()

local button = Widget.Button.new({x = 100, y = 100, width = 100, height = 50})
	:addEventListener("Press", function(d, e) print("Button [Press]") end)
	:addEventListener("Release", function(d, e) print("Button [Release]") end)
	:addEventListener("Click", function(d, e) print("Button [Click]") end)
stage:addChild(button)

stage:loop()