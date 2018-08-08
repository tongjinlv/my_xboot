/*
 * driver/clk-mbox.c
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
#include <bcm2836-mbox.h>

struct clk_mbox_pdata_t {
	int id;
};

static void clk_mbox_set_parent(struct clk_t * clk, const char * pname)
{
}

static const char * clk_mbox_get_parent(struct clk_t * clk)
{
	return NULL;
}

static void clk_mbox_set_enable(struct clk_t * clk, bool_t enable)
{
	struct clk_mbox_pdata_t * pdat = (struct clk_mbox_pdata_t *)clk->priv;
	bcm2836_mbox_clock_set_state(pdat->id, enable ? 0x1 : 0x0);
}

static bool_t clk_mbox_get_enable(struct clk_t * clk)
{
	struct clk_mbox_pdata_t * pdat = (struct clk_mbox_pdata_t *)clk->priv;
	return (bcm2836_mbox_clock_get_state(pdat->id) == 0x1) ? TRUE : FALSE;
}

static void clk_mbox_set_rate(struct clk_t * clk, u64_t prate, u64_t rate)
{
	struct clk_mbox_pdata_t * pdat = (struct clk_mbox_pdata_t *)clk->priv;
	bcm2836_mbox_clock_set_rate(pdat->id, rate);
}

static u64_t clk_mbox_get_rate(struct clk_t * clk, u64_t prate)
{
	struct clk_mbox_pdata_t * pdat = (struct clk_mbox_pdata_t *)clk->priv;
	return bcm2836_mbox_clock_get_rate(pdat->id);
}

static struct device_t * clk_mbox_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct clk_mbox_pdata_t * pdat;
	struct clk_t * clk;
	struct device_t * dev;
	struct dtnode_t o;
	char * name = dt_read_string(n, "name", NULL);
	int id = dt_read_int(n, "mbox-clock-id", -1);

	if(!name || id < 1 || id > 10)
		return NULL;

	if(search_clk(name))
		return NULL;

	pdat = malloc(sizeof(struct clk_mbox_pdata_t));
	if(!pdat)
		return NULL;

	clk = malloc(sizeof(struct clk_t));
	if(!clk)
	{
		free(pdat);
		return NULL;
	}

	pdat->id = id;

	clk->name = strdup(name);
	clk->count = 0;
	clk->set_parent = clk_mbox_set_parent;
	clk->get_parent = clk_mbox_get_parent;
	clk->set_enable = clk_mbox_set_enable;
	clk->get_enable = clk_mbox_get_enable;
	clk->set_rate = clk_mbox_set_rate;
	clk->get_rate = clk_mbox_get_rate;
	clk->priv = pdat;

	if(!register_clk(&dev, clk))
	{
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

static void clk_mbox_remove(struct device_t * dev)
{
	struct clk_t * clk = (struct clk_t *)dev->priv;

	if(clk && unregister_clk(clk))
	{
		free(clk->name);
		free(clk->priv);
		free(clk);
	}
}

static void clk_mbox_suspend(struct device_t * dev)
{
}

static void clk_mbox_resume(struct device_t * dev)
{
}

static struct driver_t clk_mbox = {
	.name		= "clk-mbox",
	.probe		= clk_mbox_probe,
	.remove		= clk_mbox_remove,
	.suspend	= clk_mbox_suspend,
	.resume		= clk_mbox_resume,
};

static __init void clk_mbox_driver_init(void)
{
	register_driver(&clk_mbox);
}

static __exit void clk_mbox_driver_exit(void)
{
	unregister_driver(&clk_mbox);
}

driver_initcall(clk_mbox_driver_init);
driver_exitcall(clk_mbox_driver_exit);
