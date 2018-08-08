/*
 * kernel/command/cmd-dcp.c
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

enum devtype_t {
	DEVTYPE_BLOCK	= 0,
	DEVTYPE_FILE	= 1,
	DEVTYPE_MEM		= 2,
};

static void usage(void)
{
	printf("usage:\r\n");
	printf("    dcp <input@offset:size> <output@offset:size>\r\n");
}

static int do_dcp(int argc, char ** argv)
{
	enum devtype_t itype, otype;
	struct block_t * iblk = NULL, * oblk = NULL;
	int ifd, ofd;
	char * iname, * oname;
	u64_t ioff, isize;
	u64_t ooff, osize;
	s64_t n, s, l;
	char * buf;
	char * p, * offset, * size;

	if(argc != 3)
	{
		usage();
		return -1;
	}

	buf = malloc(SZ_64K);
	if(!buf)
		return -1;

	p = argv[1];
	iname = strsep(&p, "@");
	offset = strsep(&p, ":");
	size = p;
	iname = (iname && strcmp(iname, "") != 0) ? iname : NULL;
	offset = (offset && strcmp(offset, "") != 0) ? offset : NULL;
	size = (size && strcmp(size, "") != 0) ? size : NULL;
	ioff = offset ? strtoull(offset, NULL, 0) : 0;
	isize = size ? strtoull(size, NULL, 0) : ~0ULL;
	if(!iname)
	{
		itype = DEVTYPE_MEM;
	}
	else if((iblk = search_block(iname)))
	{
		l = block_capacity(iblk);
		if(ioff >= l)
		{
			printf("offset too large of input device '%s'\r\n", iname);
			free(buf);
			return -1;
		}
		isize = isize < (l - ioff) ? isize : (l - ioff);
		if(isize <= 0)
		{
			printf("don't need to copy of input device '%s'\r\n", iname);
			free(buf);
			return -1;
		}
		itype = DEVTYPE_BLOCK;
	}
	else if((ifd = open(iname, O_RDONLY, (S_IRUSR|S_IRGRP|S_IROTH))) > 0)
	{
		struct stat st;

		fstat(ifd, &st);
		l = st.st_size;
		if(ioff >= l)
		{
			printf("offset too large of input file '%s'\r\n", iname);
			free(buf);
			return -1;
		}
		isize = isize < (l - ioff) ? isize : (l - ioff);
		if(isize <= 0)
		{
			printf("don't need to copy of input file '%s'\r\n", iname);
			free(buf);
			return -1;
		}
		itype = DEVTYPE_FILE;
	}
	else
	{
		printf("can't find any input device '%s'\r\n", iname);
		free(buf);
		return -1;
	}

	p = argv[2];
	oname = strsep(&p, "@");
	offset = strsep(&p, ":");
	size = p;
	oname = (oname && strcmp(oname, "") != 0) ? oname : NULL;
	offset = (offset && strcmp(offset, "") != 0) ? offset : NULL;
	size = (size && strcmp(size, "") != 0) ? size : NULL;
	ooff = offset ? strtoull(offset, NULL, 0) : 0;
	osize = size ? strtoull(size, NULL, 0) : ~0ULL;
	if(!oname)
	{
		otype = DEVTYPE_MEM;
	}
	else if((oblk = search_block(oname)))
	{
		l = block_capacity(oblk);
		if(ooff >= l)
		{
			printf("offset too large of output device '%s'\r\n", oname);
			free(buf);
			return -1;
		}
		osize = osize < (l - ooff) ? osize : (l - ooff);
		if(osize <= 0)
		{
			printf("don't need copy to output device '%s'\r\n", oname);
			free(buf);
			return -1;
		}
		otype = DEVTYPE_BLOCK;
	}
	else if((ofd = open(oname, (O_WRONLY|O_CREAT|O_TRUNC), (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH))) > 0)
	{
		ooff = 0;
		otype = DEVTYPE_FILE;
	}
	else
	{
		printf("can't find any output device '%s'\r\n", oname);
		free(buf);
		return -1;
	}

	s = isize < osize ? isize : osize;
	l = 0;
	if(itype == DEVTYPE_BLOCK)
		block_sync(iblk);

	while(l < s)
	{
		n = (s - l) < SZ_64K ? (s - l) : SZ_64K;

		switch(itype)
		{
		case DEVTYPE_BLOCK:
			n = block_read(iblk, (u8_t *)buf, ioff + l, n);
			break;
		case DEVTYPE_FILE:
			n = read(ifd, buf, n);
			break;
		case DEVTYPE_MEM:
			memcpy(buf, (void *)((virtual_addr_t)(ioff + l)), n);
			break;
		default:
			break;
		}

		switch(otype)
		{
		case DEVTYPE_BLOCK:
			block_write(oblk, (u8_t *)buf, ooff + l, n);
			break;
		case DEVTYPE_FILE:
			write(ofd, buf, n);
			break;
		case DEVTYPE_MEM:
			memcpy((void *)((virtual_addr_t)(ooff + l)), buf, n);
			break;
		default:
			break;
		}

		l += n;
	}

	if(itype == DEVTYPE_FILE)
		close(ifd);
	if(otype == DEVTYPE_FILE)
		close(ofd);
	else if(otype == DEVTYPE_BLOCK)
		block_sync(oblk);
	free(buf);

	printf("copyed %s@0x%llx:0x%llx -> %s@0x%llx:0x%llx\r\n", iname ? iname : "", ioff, s, oname ? oname : "", ooff, s);
	return 0;
}

static struct command_t cmd_dcp = {
	.name	= "dcp",
	.desc	= "device copy for file or block or memory",
	.usage	= usage,
	.exec	= do_dcp,
};

static __init void dcp_cmd_init(void)
{
	register_command(&cmd_dcp);
}

static __exit void dcp_cmd_exit(void)
{
	unregister_command(&cmd_dcp);
}

command_initcall(dcp_cmd_init);
command_exitcall(dcp_cmd_exit);
