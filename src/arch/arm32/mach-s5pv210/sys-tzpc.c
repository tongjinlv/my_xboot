/*
 * sys-tzpc.c
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

#include <xboot.h>

void sys_tzpc_init(void)
{
	virtual_addr_t addr;

	addr = 0xf1500000;
	write32(addr, 0);

	addr = 0xf1500000;
	write32(addr + 0x804, 0xff);
	write32(addr + 0x810, 0xff);
	write32(addr + 0x81c, 0xff);

	addr = 0xfad00000;
	write32(addr + 0x804, 0xff);
	write32(addr + 0x810, 0xff);
	write32(addr + 0x81c, 0xff);

	addr = 0xe0600000;
	write32(addr + 0x804, 0xff);
	write32(addr + 0x810, 0xff);
	write32(addr + 0x81c, 0xff);
	write32(addr + 0x828, 0xff);

	addr = 0xe1c00000;
	write32(addr + 0x804, 0xff);
	write32(addr + 0x810, 0xff);
	write32(addr + 0x81c, 0xff);
}
