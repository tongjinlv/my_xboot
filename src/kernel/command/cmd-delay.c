/*
 * kernel/command/cmd-delay.c
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

#include <command/command.h>

static void usage(void)
{
	printf("usage:\r\n");
	printf("    delay [millisecond]\r\n");
}

static int do_delay(int argc, char ** argv)
{
	u32_t ms = 1000;

	if(argc > 1)
		ms = strtoul(argv[1], NULL, 0);
	mdelay(ms);

	return 0;
}

static struct command_t cmd_delay = {
	.name	= "delay",
	.desc	= "delay for a specified time",
	.usage	= usage,
	.exec	= do_delay,
};

static __init void delay_cmd_init(void)
{
	register_command(&cmd_delay);
}

static __exit void delay_cmd_exit(void)
{
	unregister_command(&cmd_delay);
}

command_initcall(delay_cmd_init);
command_exitcall(delay_cmd_exit);
