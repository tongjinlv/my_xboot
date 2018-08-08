/*
 * driver/clk-s5pv210-pll.c
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
#include <s5pv210/reg-clk.h>

struct clk_s5pv210_pll_pdata_t {
	virtual_addr_t virt;
	char * parent;
	int channel;
};

static void clk_s5pv210_pll_set_parent(struct clk_t * clk, const char * pname)
{
}

static const char * clk_s5pv210_pll_get_parent(struct clk_t * clk)
{
	struct clk_s5pv210_pll_pdata_t * pdat = (struct clk_s5pv210_pll_pdata_t *)clk->priv;
	return pdat->parent;
}

static void clk_s5pv210_pll_set_enable(struct clk_t * clk, bool_t enable)
{
}

static bool_t clk_s5pv210_pll_get_enable(struct clk_t * clk)
{
	return TRUE;
}

static void clk_s5pv210_pll_set_rate(struct clk_t * clk, u64_t prate, u64_t rate)
{
}

static u64_t clk_s5pv210_pll_get_rate(struct clk_t * clk, u64_t prate)
{
	struct clk_s5pv210_pll_pdata_t * pdat = (struct clk_s5pv210_pll_pdata_t *)clk->priv;
	u32_t r, m, p, s;
	u64_t rate = 0;

	switch(pdat->channel)
	{
	case 0:
		r = (read32(pdat->virt + CLK_MUX_STAT0) >> 0) & 0x7;
		if(r == 0x1)
		{
			rate = prate;
		}
		else if(r == 0x2)
		{
			r = read32(pdat->virt + CLK_APLL_CON0);
			m = (r >> 16) & 0x3ff;
			p = (r >> 8) & 0x3f;
			s = r & 0x7;
			s = s - 1;
			rate = (u64_t)(m * (prate / (p * (1 << s))));
		}
		break;

	case 1:
		r = (read32(pdat->virt + CLK_MUX_STAT0) >> 4) & 0x7;
		if(r == 0x1)
		{
			rate = prate;
		}
		else if(r == 0x2)
		{
			r = read32(pdat->virt + CLK_MPLL_CON);
			m = (r >> 16) & 0x3ff;
			p = (r >> 8) & 0x3f;
			s = r & 0x7;
			rate = (u64_t)(m * (prate / (p * (1 << s))));
		}
		break;

	case 2:
		r = (read32(pdat->virt + CLK_MUX_STAT0) >> 8) & 0x7;
		if(r == 0x1)
		{
			rate = prate;
		}
		else if(r == 0x2)
		{
			r = read32(pdat->virt + CLK_EPLL_CON0);
			m = (r >> 16) & 0x1ff;
			p = (r >> 8) & 0x3f;
			s = r & 0x7;
			rate = (u64_t)(m * (prate / (p * (1 << s))));
		}
		break;

	case 3:
		r = read32(pdat->virt + CLK_SRC1);
		if(r & (1 << 28))
			prate = 27 * 1000 * 1000;
		r = (read32(pdat->virt + CLK_MUX_STAT0) >> 12) & 0x7;
		if(r == 0x1)
		{
			rate = prate;
		}
		else if(r == 0x2)
		{
			r = read32(pdat->virt + CLK_VPLL_CON);
			m = (r >> 16) & 0x1ff;
			p = (r >> 8) & 0x3f;
			s = r & 0x7;
			rate = (u64_t)(m * (prate / (p * (1 << s))));
		}
		break;

	default:
		break;
	}

	return rate;
}

static struct device_t * clk_s5pv210_pll_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct clk_s5pv210_pll_pdata_t * pdat;
	struct clk_t * clk;
	struct device_t * dev;
	struct dtnode_t o;
	char * parent = dt_read_string(n, "parent", NULL);
	char * name = dt_read_string(n, "name", NULL);
	int channel = dt_read_int(n, "channel", -1);

	if(channel < 0 || channel > 8)
		return NULL;

	if(!parent || !name)
		return NULL;

	if(!search_clk(parent) || search_clk(name))
		return NULL;

	pdat = malloc(sizeof(struct clk_s5pv210_pll_pdata_t));
	if(!pdat)
		return NULL;

	clk = malloc(sizeof(struct clk_t));
	if(!clk)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = phys_to_virt(S5PV210_CLK_BASE);
	pdat->parent = strdup(parent);
	pdat->channel = channel;

	clk->name = strdup(name);
	clk->count = 0;
	clk->set_parent = clk_s5pv210_pll_set_parent;
	clk->get_parent = clk_s5pv210_pll_get_parent;
	clk->set_enable = clk_s5pv210_pll_set_enable;
	clk->get_enable = clk_s5pv210_pll_get_enable;
	clk->set_rate = clk_s5pv210_pll_set_rate;
	clk->get_rate = clk_s5pv210_pll_get_rate;
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

static void clk_s5pv210_pll_remove(struct device_t * dev)
{
	struct clk_t * clk = (struct clk_t *)dev->priv;
	struct clk_s5pv210_pll_pdata_t * pdat = (struct clk_s5pv210_pll_pdata_t *)clk->priv;

	if(clk && unregister_clk(clk))
	{
		free(pdat->parent);

		free(clk->name);
		free(clk->priv);
		free(clk);
	}
}

static void clk_s5pv210_pll_suspend(struct device_t * dev)
{
}

static void clk_s5pv210_pll_resume(struct device_t * dev)
{
}

static struct driver_t clk_s5pv210_pll = {
	.name		= "clk-s5pv210-pll",
	.probe		= clk_s5pv210_pll_probe,
	.remove		= clk_s5pv210_pll_remove,
	.suspend	= clk_s5pv210_pll_suspend,
	.resume		= clk_s5pv210_pll_resume,
};

static __init void clk_s5pv210_pll_driver_init(void)
{
	register_driver(&clk_s5pv210_pll);
}

static __exit void clk_s5pv210_pll_driver_exit(void)
{
	unregister_driver(&clk_s5pv210_pll);
}

driver_initcall(clk_s5pv210_pll_driver_init);
driver_exitcall(clk_s5pv210_pll_driver_exit);
