/*
 * framework/hardware/l-gpio.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <gpio/gpio.h>
#include <framework/hardware/l-hardware.h>

struct lgpio_t {
	struct gpiochip_t * chip;
	int offset;
};

static int l_gpio_new(lua_State * L)
{
	int offset = luaL_checkinteger(L, 1);
	struct gpiochip_t * chip = search_gpiochip(offset);
	if(chip && (offset >= 0) && (offset < chip->ngpio))
	{
		struct lgpio_t * gpio = lua_newuserdata(L, sizeof(struct lgpio_t));
		gpio->chip = chip;
		gpio->offset = offset;
		luaL_setmetatable(L, MT_HARDWARE_GPIO);
		return 1;
	}
	return 0;
}

static const luaL_Reg l_gpio[] = {
	{"new",	l_gpio_new},
	{NULL,	NULL}
};

static int m_gpio_set_cfg(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	int cfg = luaL_checkinteger(L, 2);
	gpio->chip->set_cfg(gpio->chip, gpio->offset, cfg);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_cfg(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	int cfg = gpio->chip->get_cfg(gpio->chip, gpio->offset);
	lua_pushinteger(L, cfg);
	return 1;
}

static int m_gpio_set_pull(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_pull_t pull = (enum gpio_pull_t)luaL_checkinteger(L, 2);
	gpio->chip->set_pull(gpio->chip, gpio->offset, pull);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_pull(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_pull_t pull = gpio->chip->get_pull(gpio->chip, gpio->offset);
	lua_pushinteger(L, pull);
	return 1;
}

static int m_gpio_set_drv(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_drv_t drv = (enum gpio_pull_t)luaL_checkinteger(L, 2);
	gpio->chip->set_drv(gpio->chip, gpio->offset, drv);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_drv(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_drv_t drv = gpio->chip->get_drv(gpio->chip, gpio->offset);
	lua_pushinteger(L, drv);
	return 1;
}

static int m_gpio_set_rate(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_rate_t rate = (enum gpio_pull_t)luaL_checkinteger(L, 2);
	gpio->chip->set_rate(gpio->chip, gpio->offset, rate);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_rate(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_rate_t rate = gpio->chip->get_rate(gpio->chip, gpio->offset);
	lua_pushinteger(L, rate);
	return 1;
}

static int m_gpio_set_dir(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_direction_t dir = (enum gpio_pull_t)luaL_checkinteger(L, 2);
	gpio->chip->set_dir(gpio->chip, gpio->offset, dir);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_dir(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	enum gpio_direction_t dir = gpio->chip->get_dir(gpio->chip, gpio->offset);
	lua_pushinteger(L, dir);
	return 1;
}

static int m_gpio_set_value(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	int value = luaL_checkinteger(L, 2);
	gpio->chip->set_value(gpio->chip, gpio->offset, value);
	lua_settop(L, 1);
	return 1;
}

static int m_gpio_get_value(lua_State * L)
{
	struct lgpio_t * gpio = luaL_checkudata(L, 1, MT_HARDWARE_GPIO);
	int value = gpio->chip->get_value(gpio->chip, gpio->offset);
	lua_pushinteger(L, value);
	return 1;
}

static const luaL_Reg m_gpio[] = {
	{"setCfg",		m_gpio_set_cfg},
	{"getCfg",		m_gpio_get_cfg},
	{"setPull",		m_gpio_set_pull},
	{"getPull",		m_gpio_get_pull},
	{"setDrv",		m_gpio_set_drv},
	{"getDrv",		m_gpio_get_drv},
	{"setRate",		m_gpio_set_rate},
	{"getRate", 	m_gpio_get_rate},
	{"setDir",		m_gpio_set_dir},
	{"getDir",		m_gpio_get_dir},
	{"setValue",	m_gpio_set_value},
	{"getValue",	m_gpio_get_value},
	{NULL,	NULL}
};

int luaopen_hardware_gpio(lua_State * L)
{
	luaL_newlib(L, l_gpio);
    /* gpio_pull_t */
	luahelper_set_intfield(L, "PULL_UP",		GPIO_PULL_UP);
	luahelper_set_intfield(L, "PULL_DOWN",		GPIO_PULL_DOWN);
	luahelper_set_intfield(L, "PULL_NONE",		GPIO_PULL_NONE);
    /* gpio_drv_t */
	luahelper_set_intfield(L, "DRV_WEAK)",		GPIO_DRV_WEAK);
	luahelper_set_intfield(L, "DRV_WEAKER)",	GPIO_DRV_WEAKER);
	luahelper_set_intfield(L, "DRV_STRONGER)",	GPIO_DRV_STRONGER);
	luahelper_set_intfield(L, "DRV_STRONG)",	GPIO_DRV_STRONG);
    /* gpio_rate_t */
	luahelper_set_intfield(L, "RATE_SLOW",		GPIO_RATE_SLOW);
	luahelper_set_intfield(L, "RATE_FAST",		GPIO_RATE_FAST);
    /* gpio_direction_t */
	luahelper_set_intfield(L, "DIR_INPUT",		GPIO_DIRECTION_INPUT);
	luahelper_set_intfield(L, "DIR_OUTPUT",		GPIO_DIRECTION_OUTPUT);
	luahelper_create_metatable(L, MT_HARDWARE_GPIO, m_gpio);
	return 1;
}
