/*
 * driver/clk/clk-setting.c
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

static void clk_setting_set_parent(struct clk_t * clk, const char * pname)
{
}

static const char * clk_setting_get_parent(struct clk_t * clk)
{
	return NULL;
}

static void clk_setting_set_enable(struct clk_t * clk, bool_t enable)
{
}

static bool_t clk_setting_get_enable(struct clk_t * clk)
{
	return TRUE;
}

static void clk_setting_set_rate(struct clk_t * clk, u64_t prate, u64_t rate)
{
}

static u64_t clk_setting_get_rate(struct clk_t * clk, u64_t prate)
{
	return prate;
}

static struct device_t * clk_setting_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct dtnode_t o;
	struct clk_t * clk;
	struct device_t * dev;
	char * name;
	char * parent;
	u64_t rate;
	int nclocks = dt_read_array_length(n, "clocks");
	int i;

	for(i = 0; i < nclocks; i++)
	{
		dt_read_array_object(n, "clocks", i, &o);
		name = dt_read_string(&o, "name", NULL);

		if(name && search_clk(name))
		{
			if((parent = dt_read_string(&o, "parent", NULL)) && search_clk(parent))
				clk_set_parent(name, parent);

			if((rate = (u64_t)dt_read_long(&o, "rate", 0)) > 0)
				clk_set_rate(name, rate);

			if(dt_read_bool(&o, "enable", 0))
				clk_enable(name);
			else
				clk_disable(name);
		}
	}

	clk = malloc(sizeof(struct clk_t));
	if(!clk)
		return NULL;

	clk->name = alloc_device_name(dt_read_name(n), dt_read_id(n));
	clk->count = 0;
	clk->set_parent = clk_setting_set_parent;
	clk->get_parent = clk_setting_get_parent;
	clk->set_enable = clk_setting_set_enable;
	clk->get_enable = clk_setting_get_enable;
	clk->set_rate = clk_setting_set_rate;
	clk->get_rate = clk_setting_get_rate;
	clk->priv = 0;

	if(!register_clk(&dev, clk))
	{
		free_device_name(clk->name);
		free(clk->priv);
		free(clk);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void clk_setting_remove(struct device_t * dev)
{
	struct clk_t * clk = (struct clk_t *)dev->priv;

	if(clk && unregister_clk(clk))
	{
		free_device_name(clk->name);
		free(clk->priv);
		free(clk);
	}
}

static void clk_setting_suspend(struct device_t * dev)
{
}

static void clk_setting_resume(struct device_t * dev)
{
}

static struct driver_t clk_setting = {
	.name		= "clk-setting",
	.probe		= clk_setting_probe,
	.remove		= clk_setting_remove,
	.suspend	= clk_setting_suspend,
	.resume		= clk_setting_resume,
};

static __init void clk_setting_driver_init(void)
{
	register_driver(&clk_setting);
}

static __exit void clk_setting_driver_exit(void)
{
	unregister_driver(&clk_setting);
}

driver_initcall(clk_setting_driver_init);
driver_exitcall(clk_setting_driver_exit);
