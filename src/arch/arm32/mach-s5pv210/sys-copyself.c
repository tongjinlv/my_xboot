/*
 * sys-copyself.c
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

extern unsigned char __image_start;
extern unsigned char __image_end;

/*
 * Global block size.
 */
#define irom_global_block_size \
	(*((volatile u32_t *)(0xd0037480)))

/*
 * Global sdhc information bit.
 *
 * [31:16] RCA address
 * [2] sd card.
 * [1] mmc card.
 * [0] high capacity enable.
 */
#define irom_global_sdhc_info_bit \
	(*((volatile u32_t *)(0xd0037484)))

/*
 * Sdmmc base, current boot channel.
 */
#define irom_v210_sdmmc_base \
	(*((volatile u32_t *)(0xd0037488)))

/*
 * This function copies a block of page to destination memory (8-bit ecc only)
 * 2048, 4096 page 8bits-bus nand Only.
 *
 * @param u32_t block : source block address number to copy.
 * @param u32_t page  : source page address number to copy.
 * @param u8_t * mem  : target memory pointer.
 * @return s32_t - success or failure.
*/
#define	irom_nf8_readpage_adv(block, page, mem) \
	(((s32_t(*)(u32_t, u32_t, u8_t *))(*((u32_t *)(0xd0037f90))))(block, page, mem))

/*
 * This function copies a block of page to destination memory (4-bit ecc only)
 * 2048 page size, 5 cycle address, 16bits-bus nand Only
 *
 * @param u32_t block : source block address number to copy.
 * @param u32_t page  : source page address number to copy.
 * @param u8_t * mem  : target memory pointer.
 * @return s32_t - success or failure.
*/
#define	irom_nf16_readpage_adv(block, page, mem) \
	(((s32_t(*)(u32_t, u32_t, u8_t *))(*((u32_t *)(0xd0037f94))))(block, page, mem))

/*
 * This function copies SD/MMC card data to memory.
 * always use EPLL source clock. this function works at 20Mhz.
 *
 * @param ch 	 : HSMMC controller channel number
 * @param sector : source card(SD/MMC) address (it must block address).
 * @param count  : number of blocks to copy.
 * @param mem    : memory to copy to.
 * @param init 	 : reinitialize or not.
 * @return bool(u8_t) - success or failure.
 */
#define irom_sdmmc_to_mem(ch, sector, count, mem, init) \
	(((u8_t(*)(s32_t, u32_t, u16_t, u32_t *, s32_t))(*((u32_t *)(0xd0037f98))))(ch, sector, count, mem, init))

void sys_copyself(void)
{
	virtual_addr_t addr = 0xe0000004;
	u32_t page, block, size;
	u32_t * mem;
	u8_t om;

	/*
	 * Read om register, om[4..1]
	 */
	om = (u8_t)((read32(addr) >> 1) & 0x0f);

	/* Essd */
	if(om == 0x0)
	{
	}
	/* Nand 2KB, 5-cycle, 8-bit ecc */
	else if(om == 0x1)
	{
		mem = (u32_t *)&__image_start;
		size = (&__image_end - &__image_start + 0x00020000) >> 17;
		for(block = 0; block < size; block++)
		{
			for(page = 0; page < 64; page++)
			{
				irom_nf8_readpage_adv(block, page, (u8_t *)mem);
				mem += 512;
			}
		}
	}
	/* Nand 4KB, 5-cycle, 8-bit ecc */
	else if(om == 0x2)
	{
	}
	/* Nand 4KB, 5-cycle, 16-bit ecc */
	else if(om == 0x3)
	{
	}
	/* Onenand mux */
	else if(om == 0x4)
	{
	}
	/* Onenand demux */
	else if(om == 0x5)
	{
	}
	/* Sd / mmc */
	else if(om == 0x6)
	{
		mem = (u32_t *)&__image_start;
		size = (&__image_end - &__image_start + 0x00040000) >> 18;
		size = size << 9;
		if(irom_v210_sdmmc_base == 0xeb000000)
			irom_sdmmc_to_mem(0, 1, size, mem, 0);
		else if(irom_v210_sdmmc_base == 0xeb200000)
			irom_sdmmc_to_mem(2, 1, size, mem, 0);
	}
	/* Emmc, 4-bit */
	else if(om == 0x7)
	{
	}
	/* Reserved */
	else if(om == 0x8)
	{
	}
	/* Nand 2KB, 4-cycle, 8-bit ecc */
	else if(om == 0x9)
	{
	}
	/* Nor flash */
	else if(om == 0xa)
	{
	}
	/* Emmc, 8-bit */
	else if(om == 0xa)
	{
	}
}
