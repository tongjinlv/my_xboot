/*
 * driver/adc-rk3128.c
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
#include <adc/adc.h>

enum {
	SARADC_DATA		= 0x00,
	SARADC_STAT		= 0x04,
	SARADC_CTRL		= 0x08,
	SARADC_DELAY	= 0x0c,
};

struct adc_rk3128_pdata_t
{
	virtual_addr_t virt;
	char * clk;
};

static u32_t adc_rk3128_read(struct adc_t * adc, int channel)
{
	struct adc_rk3128_pdata_t * pdat = (struct adc_rk3128_pdata_t *)adc->priv;
	u32_t value;

	write32(pdat->virt + SARADC_CTRL, 0);
	udelay(1);
	write32(pdat->virt + SARADC_CTRL, (0x1 << 5) | (0x1 << 3) | ((channel & 0x7) << 0));
	udelay(1);
	while((read32(pdat->virt + SARADC_CTRL) & (0x1 << 6)) == 0x00);
	value = read32(pdat->virt + SARADC_DATA) & 0x3ff;
	write32(pdat->virt + SARADC_CTRL, 0);

	return value;
}

static struct device_t * adc_rk3128_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct adc_rk3128_pdata_t * pdat;
	struct adc_t * adc;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk = dt_read_string(n, "clock-name", NULL);

	if(!search_clk(clk))
		return NULL;

	pdat = malloc(sizeof(struct adc_rk3128_pdata_t));
	if(!pdat)
		return NULL;

	adc = malloc(sizeof(struct adc_t));
	if(!adc)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->clk = strdup(clk);

	adc->name = alloc_device_name(dt_read_name(n), -1);
	adc->vreference = dt_read_int(n, "reference-voltage", 1800000);
	adc->resolution = 10;
	adc->nchannel = 3;
	adc->read = adc_rk3128_read;
	adc->priv = pdat;

	clk_enable(pdat->clk);
	write32(pdat->virt + SARADC_CTRL, 0);

	if(!register_adc(&dev, adc))
	{
		write32(pdat->virt + SARADC_CTRL, 0);
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(adc->name);
		free(adc->priv);
		free(adc);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void adc_rk3128_remove(struct device_t * dev)
{
	struct adc_t * adc = (struct adc_t *)dev->priv;
	struct adc_rk3128_pdata_t * pdat = (struct adc_rk3128_pdata_t *)adc->priv;

	if(adc && unregister_adc(adc))
	{
		write32(pdat->virt + SARADC_CTRL, 0);
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(adc->name);
		free(adc->priv);
		free(adc);
	}
}

static void adc_rk3128_suspend(struct device_t * dev)
{
}

static void adc_rk3128_resume(struct device_t * dev)
{
}

static struct driver_t adc_rk3128 = {
	.name		= "adc-rk3128",
	.probe		= adc_rk3128_probe,
	.remove		= adc_rk3128_remove,
	.suspend	= adc_rk3128_suspend,
	.resume		= adc_rk3128_resume,
};

static __init void adc_rk3128_driver_init(void)
{
	register_driver(&adc_rk3128);
}

static __exit void adc_rk3128_driver_exit(void)
{
	unregister_driver(&adc_rk3128);
}

driver_initcall(adc_rk3128_driver_init);
driver_exitcall(adc_rk3128_driver_exit);
