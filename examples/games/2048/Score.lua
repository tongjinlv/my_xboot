local M = Class(DisplayObject)

function M:init(n)
	self.super:init()
	self:set(n or 0)
end

function M:set(n)
	self.n = n or 0
	local text = string.format("%5d", self.n)
	local x = 0

	self.children = {}
	for c in string.gmatch(text, "[%z\1-\127\194-\244][\128-\191]*") do
		if c ~= ' ' then
			local char = assets:loadDisplay("assets/images/" .. c .. ".png"):setPosition(x, 0)
			self:addChild(char)
		end
		x = x + 12
	end
end

function M:get()
	return self.n
end

return M
