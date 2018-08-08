/*
 * driver/clk-rk3128-pll.c
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

#include <clk/clk.h>
#include <rk3128/reg-cru.h>

struct clk_rk3128_pll_pdata_t {
	virtual_addr_t virt;
	char * parent;
	int channel;
};

struct pll_clk_set_t {
	u64_t rate;
	u32_t pllcon0;
	u32_t pllcon1;
	u32_t pllcon2;
};

#define PLL_SET_REFDIV(val)		((0x3f << (0 + 16)) | ((val & 0x3f) << 0))
#define PLL_SET_FBDIV(val)		((0xfff << (0 + 16)) | ((val & 0xfff) << 0))
#define PLL_SET_POSTDIV1(val)	((0x7 << (12 + 16)) | ((val & 0x7) << 12))
#define PLL_SET_POSTDIV2(val)	((0x7 << (6 + 16)) | ((val & 0x7) << 6))
#define PLL_SET_FRAC(val)		((val & 0xffffff) << 0)
#define PLL_SET_DSMPD(val)		((0x1 << (12 + 16)) | ((val & 0x1) << 12))

#define PLL_SET_CLKS(_mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac) \
{ \
	.rate = _mhz, \
	.pllcon0 = PLL_SET_POSTDIV1(_postdiv1) | PLL_SET_FBDIV(_fbdiv), \
	.pllcon1 = PLL_SET_DSMPD(_dsmpd) | PLL_SET_POSTDIV2(_postdiv2) | PLL_SET_REFDIV(_refdiv), \
	.pllcon2 = PLL_SET_FRAC(_frac), \
}

static struct pll_clk_set_t pll_table[] = {
	PLL_SET_CLKS(1000000000, 3, 125, 1,  1, 1, 0),
	PLL_SET_CLKS(816000000, 1, 68, 2, 1, 1, 0),
	PLL_SET_CLKS(800000000, 1, 100, 3, 1, 1, 0),
	PLL_SET_CLKS(600000000, 1, 75, 3, 1, 1, 0),
	PLL_SET_CLKS(594000000, 2, 99, 2, 1, 1, 0),
	PLL_SET_CLKS(500000000, 1, 125, 3, 2, 1, 0),
	PLL_SET_CLKS(416000000, 1, 104, 3, 2, 1, 0),
	PLL_SET_CLKS(400000000, 3, 200, 2, 2, 1, 0),
	PLL_SET_CLKS(300000000, 1, 75, 3, 2, 1, 0),
};

static void clk_rk3128_pll_set_parent(struct clk_t * clk, const char * pname)
{
}

static const char * clk_rk3128_pll_get_parent(struct clk_t * clk)
{
	struct clk_rk3128_pll_pdata_t * pdat = (struct clk_rk3128_pll_pdata_t *)clk->priv;
	return pdat->parent;
}

static void clk_rk3128_pll_set_enable(struct clk_t * clk, bool_t enable)
{
}

static bool_t clk_rk3128_pll_get_enable(struct clk_t * clk)
{
	return TRUE;
}

static void clk_rk3128_pll_set_rate(struct clk_t * clk, u64_t prate, u64_t rate)
{
	struct clk_rk3128_pll_pdata_t * pdat = (struct clk_rk3128_pll_pdata_t *)clk->priv;
	struct pll_clk_set_t * set = NULL;
	int i;

	for(i = 0; i < ARRAY_SIZE(pll_table); i++)
	{
		if(pll_table[i].rate <= rate)
		{
			set = &pll_table[i];
			break;
		}
	}
	if(set == NULL)
		return;

	write32(pdat->virt + CRU_MODE_CON, (0x00010000 << (pdat->channel * 4)));
	write32(pdat->virt + CRU_PLL_CON(pdat->channel, 0), set->pllcon0);
	write32(pdat->virt + CRU_PLL_CON(pdat->channel, 1), set->pllcon1);
	write32(pdat->virt + CRU_PLL_CON(pdat->channel, 2), set->pllcon2);
	while(((read32(pdat->virt + CRU_PLL_CON(pdat->channel, 1)) >> 10) & 0x1) != 0x1);
	write32(pdat->virt + CRU_MODE_CON, (0x00010001 << (pdat->channel * 4)));
}

static u64_t clk_rk3128_pll_get_rate(struct clk_t * clk, u64_t prate)
{
	struct clk_rk3128_pll_pdata_t * pdat = (struct clk_rk3128_pll_pdata_t *)clk->priv;
	u32_t pll_con0, pll_con1, pll_con2, dsmp;
	u64_t rate = 0, frac_rate = 0;
	u32_t val;

	val = (read32(pdat->virt + CRU_MODE_CON) >> (pdat->channel * 4)) & 0x1;
	if(val == 0x1)
	{
		pll_con0 = read32(pdat->virt + CRU_PLL_CON(pdat->channel, 0));
		pll_con1 = read32(pdat->virt + CRU_PLL_CON(pdat->channel, 1));
		pll_con2 = read32(pdat->virt + CRU_PLL_CON(pdat->channel, 2));
		dsmp = (read32(pdat->virt + CRU_PLL_CON(pdat->channel, 1)) >> 12) & 0x1;

		rate = (u64_t)(24000000) * ((pll_con0 >> 0) & 0xfff);
		rate = rate / ((pll_con1 >> 0) & 0x3f);
		if(dsmp == 0)
		{
			frac_rate = (u64_t)(24000000) * ((pll_con2 >> 0) & 0xffffff);
			frac_rate = frac_rate / ((pll_con1 >> 0) & 0x3f);
			rate += frac_rate >> 24;
		}
		rate = rate / ((pll_con0 >> 12) & 0x7);
		rate = rate / ((pll_con1 >> 6) & 0x7);
	}
	else
	{
		rate = 24000000;
	}

	return rate;
}

static struct device_t * clk_rk3128_pll_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct clk_rk3128_pll_pdata_t * pdat;
	struct clk_t * clk;
	struct device_t * dev;
	struct dtnode_t o;
	char * parent = dt_read_string(n, "parent", NULL);
	char * name = dt_read_string(n, "name", NULL);
	int channel = dt_read_int(n, "channel", -1);

	if(channel < 0 || channel > 4)
		return NULL;

	if(!parent || !name)
		return NULL;

	if(!search_clk(parent) || search_clk(name))
		return NULL;

	pdat = malloc(sizeof(struct clk_rk3128_pll_pdata_t));
	if(!pdat)
		return NULL;

	clk = malloc(sizeof(struct clk_t));
	if(!clk)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = phys_to_virt(RK3128_CRU_BASE);
	pdat->parent = strdup(parent);
	pdat->channel = channel;

	clk->name = strdup(name);
	clk->count = 0;
	clk->set_parent = clk_rk3128_pll_set_parent;
	clk->get_parent = clk_rk3128_pll_get_parent;
	clk->set_enable = clk_rk3128_pll_set_enable;
	clk->get_enable = clk_rk3128_pll_get_enable;
	clk->set_rate = clk_rk3128_pll_set_rate;
	clk->get_rate = clk_rk3128_pll_get_rate;
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

static void clk_rk3128_pll_remove(struct device_t * dev)
{
	struct clk_t * clk = (struct clk_t *)dev->priv;
	struct clk_rk3128_pll_pdata_t * pdat = (struct clk_rk3128_pll_pdata_t *)clk->priv;

	if(clk && unregister_clk(clk))
	{
		free(pdat->parent);

		free(clk->name);
		free(clk->priv);
		free(clk);
	}
}

static void clk_rk3128_pll_suspend(struct device_t * dev)
{
}

static void clk_rk3128_pll_resume(struct device_t * dev)
{
}

static struct driver_t clk_rk3128_pll = {
	.name		= "clk-rk3128-pll",
	.probe		= clk_rk3128_pll_probe,
	.remove		= clk_rk3128_pll_remove,
	.suspend	= clk_rk3128_pll_suspend,
	.resume		= clk_rk3128_pll_resume,
};

static __init void clk_rk3128_pll_driver_init(void)
{
	register_driver(&clk_rk3128_pll);
}

static __exit void clk_rk3128_pll_driver_exit(void)
{
	unregister_driver(&clk_rk3128_pll);
}

driver_initcall(clk_rk3128_pll_driver_init);
driver_exitcall(clk_rk3128_pll_driver_exit);
