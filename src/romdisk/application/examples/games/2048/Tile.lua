local M = Class(DisplayObject)

function M:init(x, y, n)
	self.super:init()

	local assets = assets
	self.gx = x
	self.gy = y
	self.n = n
	self.o = assets:loadDisplay("games/2048/images/tile" .. self.n .. ".png")
		:setPosition((self.gx - 1) * 70 + 55, (self.gy - 1) * 70 + 171)
		:setAnchor(0.5, 0.5)
		:setScale(0.1, 0.1)
		:animate({scalex = 1, scaley = 1}, 0.3, "inOutQuint")
	self:addChild(self.o)
end

function M:move(x, y)
	if self.gx ~= x or self.gy ~= y then
		self.gx = x
		self.gy = y
		self.o:animate({x = (x - 1) * 70 + 55, y = (y - 1) * 70 + 171}, 0.3, "inOutQuint")
	end
end

function M:number()
	return self.n
end

return M
