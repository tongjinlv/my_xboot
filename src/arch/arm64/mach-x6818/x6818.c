/*
 * x6818.c
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
#include <s5p6818-rstcon.h>
#include <s5p6818/reg-sys.h>
#include <s5p6818/reg-id.h>

static const struct mmap_t mach_map[] = {
	{ 0 },
};

static int mach_detect(struct machine_t * mach)
{
	/*
	 * VIC to core, pass through GIC.
	 */
	write32(phys_to_virt(0xf0000100), 0);

	/*
	 * Reset some IP modules.
	 */
	s5p6818_ip_reset(RESET_ID_TIMER, 0);
	s5p6818_ip_reset(RESET_ID_PWM, 0);
	s5p6818_ip_reset(RESET_ID_UART0, 0);
	s5p6818_ip_reset(RESET_ID_UART1, 0);
	s5p6818_ip_reset(RESET_ID_UART2, 0);
	s5p6818_ip_reset(RESET_ID_UART3, 0);
	s5p6818_ip_reset(RESET_ID_UART4, 0);
	s5p6818_ip_reset(RESET_ID_UART5, 0);

	return 1;
}

static void mach_memmap(struct machine_t * mach)
{
}

static void mach_shutdown(struct machine_t * mach)
{
}

static void mach_reboot(struct machine_t * mach)
{
	write32(phys_to_virt(S5P6818_SYS_PWRCONT), (read32(phys_to_virt(S5P6818_SYS_PWRCONT)) & ~(0x1<<3)) | (0x1<<3));
	write32(phys_to_virt(S5P6818_SYS_PWRMODE), (read32(phys_to_virt(S5P6818_SYS_PWRMODE)) & ~(0x1<<12)) | (0x1<<12));
}

static void mach_sleep(struct machine_t * mach)
{
}

static void mach_cleanup(struct machine_t * mach)
{
}

static void mach_logger(struct machine_t * mach, const char * buf, int count)
{
	virtual_addr_t virt = phys_to_virt(0xc00a1000);
	int i;

	if((read32(virt + 0x8) & (1 << 0)))
	{
		for(i = 0; i < count; i++)
		{
			while((read32(virt + 0x18) & (1 << 24)));
			write8(virt + 0x20, buf[i]);
		}
	}
	else
	{
		for(i = 0; i < count; i++)
		{
			while(!(read32(virt + 0x10) & (1 << 1)));
			write8(virt + 0x20, buf[i]);
		}
	}
}

static const char * mach_uniqueid(struct machine_t * mach)
{
	static char uniqueid[16 + 1] = { 0 };
	virtual_addr_t virt = phys_to_virt(S5P6818_ID_BASE);
	u32_t ecid0, ecid1;

	s5p6818_ip_reset(RESET_ID_ECID, 0);

	ecid0 = read32(virt + ID_ECID0);
	ecid1 = read32(virt + ID_ECID1);
	sprintf(uniqueid, "%02x%02x%02x%02x%02x%02x%02x%02x",
		(ecid0 >> 24) & 0xff, (ecid0 >> 16) & 0xff, (ecid0 >> 8) & 0xff, (ecid0 >> 0) & 0xff,
		(ecid1 >> 24) & 0xff, (ecid1 >> 16) & 0xff, (ecid1 >> 8) & 0xff, (ecid1 >> 0) & 0xff);
	return uniqueid;
}

static int mach_keygen(struct machine_t * mach, const char * msg, void * key)
{
	return 0;
}

static struct machine_t x6818 = {
	.name 		= "x6818",
	.desc 		= "X6818 Based On S5P6818 SOC",
	.map		= mach_map,
	.detect 	= mach_detect,
	.memmap		= mach_memmap,
	.shutdown	= mach_shutdown,
	.reboot		= mach_reboot,
	.sleep		= mach_sleep,
	.cleanup	= mach_cleanup,
	.logger		= mach_logger,
	.uniqueid	= mach_uniqueid,
	.keygen		= mach_keygen,
};

static __init void x6818_machine_init(void)
{
	register_machine(&x6818);
}

static __exit void x6818_machine_exit(void)
{
	unregister_machine(&x6818);
}

machine_initcall(x6818_machine_init);
machine_exitcall(x6818_machine_exit);
