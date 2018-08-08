/*
 * driver/wdg-sp805.c
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
#include <clk/clk.h>
#include <watchdog/watchdog.h>

enum {
	WDG_LOAD	= 0x000,
	WDG_VALUE	= 0x004,
	WDG_CTRL	= 0x008,
	WDG_INTCLR	= 0x00c,
	WDG_RIS		= 0x010,
	WDG_MIS		= 0x014,
	WDG_LOCK	= 0xc00,
};

struct wdg_sp805_pdata_t {
	virtual_addr_t virt;
	char * clk;
	u32_t load;
};

static void wdg_sp805_set(struct watchdog_t * watchdog, int timeout)
{
	struct wdg_sp805_pdata_t * pdat = (struct wdg_sp805_pdata_t *)watchdog->priv;
	u64_t rate = clk_get_rate(pdat->clk);

	if(timeout > 0)
	{
		pdat->load = timeout * rate / 2 - 1;
		if(pdat->load > 0xffffffff)
			pdat->load = 0xffffffff;
		if(pdat->load < 1)
			pdat->load = 1;
		write32(pdat->virt + WDG_LOCK, 0x1acce551);
		write32(pdat->virt + WDG_LOAD, pdat->load);
		write32(pdat->virt + WDG_INTCLR, 0);
		write32(pdat->virt + WDG_CTRL, 3);
		write32(pdat->virt + WDG_LOCK, 0);
	}
	else
	{
		pdat->load = 0;
		write32(pdat->virt + WDG_LOCK, 0x1acce551);
		write32(pdat->virt + WDG_INTCLR, 0);
		write32(pdat->virt + WDG_CTRL, 0);
		write32(pdat->virt + WDG_LOCK, 0);
	}
}

static int wdg_sp805_get(struct watchdog_t * watchdog)
{
	struct wdg_sp805_pdata_t * pdat = (struct wdg_sp805_pdata_t *)watchdog->priv;
	u64_t rate = clk_get_rate(pdat->clk);
	u32_t val = read32(pdat->virt + WDG_VALUE);
	if(rate == 0)
		return 0;
	if (!(read32(pdat->virt + WDG_RIS) & 0x1))
		val += pdat->load + 1;
	return (int)(val / rate);
}

static struct device_t * wdg_sp805_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct wdg_sp805_pdata_t * pdat;
	struct watchdog_t * wdg;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk = dt_read_string(n, "clock-name", NULL);
	u32_t id = (((read32(virt + 0xfec) & 0xff) << 24) |
				((read32(virt + 0xfe8) & 0xff) << 16) |
				((read32(virt + 0xfe4) & 0xff) <<  8) |
				((read32(virt + 0xfe0) & 0xff) <<  0));

	if(((id >> 12) & 0xff) != 0x41 || (id & 0xfff) != 0x805)
		return NULL;

	if(!search_clk(clk))
		return NULL;

	pdat = malloc(sizeof(struct wdg_sp805_pdata_t));
	if(!pdat)
		return NULL;

	wdg = malloc(sizeof(struct watchdog_t));
	if(!wdg)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->clk = strdup(clk);
	pdat->load = 0;

	wdg->name = alloc_device_name(dt_read_name(n), -1);
	wdg->set = wdg_sp805_set;
	wdg->get = wdg_sp805_get;
	wdg->priv = pdat;

	clk_enable(pdat->clk);
	write32(pdat->virt + WDG_LOCK, 0x1acce551);
	write32(pdat->virt + WDG_INTCLR, 0);
	write32(pdat->virt + WDG_CTRL, 0);
	write32(pdat->virt + WDG_LOCK, 0);

	if(!register_watchdog(&dev, wdg))
	{
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(wdg->name);
		free(wdg->priv);
		free(wdg);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void wdg_sp805_remove(struct device_t * dev)
{
	struct watchdog_t * wdg = (struct watchdog_t *)dev->priv;
	struct wdg_sp805_pdata_t * pdat = (struct wdg_sp805_pdata_t *)wdg->priv;

	if(wdg && unregister_watchdog(wdg))
	{
		write32(pdat->virt + WDG_LOCK, 0x1acce551);
		write32(pdat->virt + WDG_INTCLR, 0);
		write32(pdat->virt + WDG_CTRL, 0);
		write32(pdat->virt + WDG_LOCK, 0);
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(wdg->name);
		free(wdg->priv);
		free(wdg);
	}
}

static void wdg_sp805_suspend(struct device_t * dev)
{
}

static void wdg_sp805_resume(struct device_t * dev)
{
}

static struct driver_t wdg_sp805 = {
	.name		= "wdg-sp805",
	.probe		= wdg_sp805_probe,
	.remove		= wdg_sp805_remove,
	.suspend	= wdg_sp805_suspend,
	.resume		= wdg_sp805_resume,
};

static __init void wdg_sp805_driver_init(void)
{
	register_driver(&wdg_sp805);
}

static __exit void wdg_sp805_driver_exit(void)
{
	unregister_driver(&wdg_sp805);
}

driver_initcall(wdg_sp805_driver_init);
driver_exitcall(wdg_sp805_driver_exit);
