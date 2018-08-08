/*
 * driver/clk-rk3288-factor.c
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

struct clk_rk3288_factor_pdata_t {
	virtual_addr_t virt;
	char * parent;
};

static void clk_rk3288_factor_set_parent(struct clk_t * clk, const char * pname)
{
}

static const char * clk_rk3288_factor_get_parent(struct clk_t * clk)
{
	struct clk_rk3288_factor_pdata_t * pdat = (struct clk_rk3288_factor_pdata_t *)clk->priv;
	return pdat->parent;
}

static void clk_rk3288_factor_set_enable(struct clk_t * clk, bool_t enable)
{
}

static bool_t clk_rk3288_factor_get_enable(struct clk_t * clk)
{
	return TRUE;
}

static u64_t clk_rk3288_gcd(u64_t a, u64_t b)
{
	u64_t c;

	while(b != 0)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

static void clk_rk3288_factor_set_rate(struct clk_t * clk, u64_t prate, u64_t rate)
{
	struct clk_rk3288_factor_pdata_t * pdat = (struct clk_rk3288_factor_pdata_t *)clk->priv;
	u64_t cdiv;
	u32_t mult, div;

	if(rate == 0)
		rate = prate;

	cdiv = clk_rk3288_gcd(prate, rate);
	div = (prate / cdiv) & 0xffff;
	mult = (rate / cdiv) & 0xffff;
	write32(pdat->virt, (mult << 16) | (div << 0));
}

static u64_t clk_rk3288_factor_get_rate(struct clk_t * clk, u64_t prate)
{
	struct clk_rk3288_factor_pdata_t * pdat = (struct clk_rk3288_factor_pdata_t *)clk->priv;
	u32_t val = read32(pdat->virt);

	return (prate * ((val >> 16) & 0xffff)) / ((val >> 0) & 0xffff);
}

static struct device_t * clk_rk3288_factor_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct clk_rk3288_factor_pdata_t * pdat;
	struct clk_t * clk;
	struct device_t * dev;
	struct dtnode_t o;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * parent = dt_read_string(n, "parent", NULL);
	char * name = dt_read_string(n, "name", NULL);

	if(!parent || !name)
		return NULL;

	if(!search_clk(parent) || search_clk(name))
		return NULL;

	pdat = malloc(sizeof(struct clk_rk3288_factor_pdata_t));
	if(!pdat)
		return NULL;

	clk = malloc(sizeof(struct clk_t));
	if(!clk)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->parent = strdup(parent);

	clk->name = strdup(name);
	clk->count = 0;
	clk->set_parent = clk_rk3288_factor_set_parent;
	clk->get_parent = clk_rk3288_factor_get_parent;
	clk->set_enable = clk_rk3288_factor_set_enable;
	clk->get_enable = clk_rk3288_factor_get_enable;
	clk->set_rate = clk_rk3288_factor_set_rate;
	clk->get_rate = clk_rk3288_factor_get_rate;
	clk->priv = pdat;

	if(!register_clk(&dev, clk))
	{
		free(pdat->parent);

		free(clk->name);
		free(clk->priv);
		free(clk);
		return NULL;
	}
	dev->driver = drv;

	if(dt_read_object(n, "default", &o))
	{
		char * c = clk->name;
		char * p;
		u64_t r;
		int e;

		if((p = dt_read_string(&o, "parent", NULL)) && search_clk(p))
			clk_set_parent(c, p);
		if((r = (u64_t)dt_read_long(&o, "rate", 0)) > 0)
			clk_set_rate(c, r);
		if((e = dt_read_bool(&o, "enable", -1)) != -1)
		{
			if(e > 0)
				clk_enable(c);
			else
				clk_disable(c);
		}
	}
	return dev;
}

static void clk_rk3288_factor_remove(struct device_t * dev)
{
	struct clk_t * clk = (struct clk_t *)dev->priv;
	struct clk_rk3288_factor_pdata_t * pdat = (struct clk_rk3288_factor_pdata_t *)clk->priv;

	if(clk && unregister_clk(clk))
	{
		free(pdat->parent);

		free(clk->name);
		free(clk->priv);
		free(clk);
	}
}

static void clk_rk3288_factor_suspend(struct device_t * dev)
{
}

static void clk_rk3288_factor_resume(struct device_t * dev)
{
}

static struct driver_t clk_rk3288_factor = {
	.name		= "clk-rk3288-factor",
	.probe		= clk_rk3288_factor_probe,
	.remove		= clk_rk3288_factor_remove,
	.suspend	= clk_rk3288_factor_suspend,
	.resume		= clk_rk3288_factor_resume,
};

static __init void clk_rk3288_factor_driver_init(void)
{
	register_driver(&clk_rk3288_factor);
}

static __exit void clk_rk3288_factor_driver_exit(void)
{
	unregister_driver(&clk_rk3288_factor);
}

driver_initcall(clk_rk3288_factor_driver_init);
driver_exitcall(clk_rk3288_factor_driver_exit);
