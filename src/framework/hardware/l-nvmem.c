/*
 * framework/hardware/l-nvmem.c
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

#include <nvmem/nvmem.h>
#include <framework/hardware/l-hardware.h>

static int l_nvmem_new(lua_State * L)
{
	const char * name = luaL_optstring(L, 1, NULL);
	struct nvmem_t * m = name ? search_nvmem(name) : search_first_nvmem();
	if(!m)
		return 0;
	lua_pushlightuserdata(L, m);
	luaL_setmetatable(L, MT_HARDWARE_NVMEM);
	return 1;
}

static int l_nvmem_list(lua_State * L)
{
	struct device_t * pos, * n;
	struct nvmem_t * m;

	lua_newtable(L);
	list_for_each_entry_safe(pos, n, &__device_head[DEVICE_TYPE_NVMEM], head)
	{
		m = (struct nvmem_t *)(pos->priv);
		if(!m)
			continue;
		lua_pushlightuserdata(L, m);
		luaL_setmetatable(L, MT_HARDWARE_NVMEM);
		lua_setfield(L, -2, pos->name);
	}
	return 1;
}

static const luaL_Reg l_nvmem[] = {
	{"new",		l_nvmem_new},
	{"list",	l_nvmem_list},
	{NULL,	NULL}
};

static int m_nvmem_tostring(lua_State * L)
{
	struct nvmem_t * m = luaL_checkudata(L, 1, MT_HARDWARE_NVMEM);
	lua_pushstring(L, m->name);
	return 1;
}

static int m_nvmem_set(lua_State * L)
{
	struct nvmem_t * m = luaL_checkudata(L, 1, MT_HARDWARE_NVMEM);
	const char * key = luaL_checkstring(L, 2);
	const char * value = luaL_optstring(L, 3, NULL);
	nvmem_set(m, key, value);
	lua_settop(L, 1);
	return 1;
}

static int m_nvmem_get(lua_State * L)
{
	struct nvmem_t * m = luaL_checkudata(L, 1, MT_HARDWARE_NVMEM);
	const char * key = luaL_checkstring(L, 2);
	const char * def = luaL_optstring(L, 3, NULL);
	lua_pushstring(L, nvmem_get(m, key, def));
	return 1;
}

static int m_nvmem_clear(lua_State * L)
{
	struct nvmem_t * m = luaL_checkudata(L, 1, MT_HARDWARE_NVMEM);
	nvmem_clear(m);
	lua_settop(L, 1);
	return 1;
}

static int m_nvmem_sync(lua_State * L)
{
	struct nvmem_t * m = luaL_checkudata(L, 1, MT_HARDWARE_NVMEM);
	nvmem_sync(m);
	lua_settop(L, 1);
	return 1;
}

static const luaL_Reg m_nvmem[] = {
	{"__tostring",	m_nvmem_tostring},
	{"set",			m_nvmem_set},
	{"get",			m_nvmem_get},
	{"clear",		m_nvmem_clear},
	{"sync",		m_nvmem_sync},
	{NULL,	NULL}
};

int luaopen_hardware_nvmem(lua_State * L)
{
	luaL_newlib(L, l_nvmem);
	luahelper_create_metatable(L, MT_HARDWARE_NVMEM, m_nvmem);
	return 1;
}
