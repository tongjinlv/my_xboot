/*
 * driver/gmeter/gmeter.c
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
#include <gmeter/gmeter.h>

static ssize_t gmeter_read_acceleration(struct kobj_t * kobj, void * buf, size_t size)
{
	struct gmeter_t * g = (struct gmeter_t *)kobj->priv;
	int x = 0, y = 0, z = 0;
	gmeter_get_acceleration(g, &x, &y, &z);
	return sprintf(buf, "[%d.%06d %d.%06d %d.%06d] m/s^2", x / 1000000, abs(x % 1000000), y / 1000000, abs(y % 1000000), z / 1000000, abs(z % 1000000));
}

struct gmeter_t * search_gmeter(const char * name)
{
	struct device_t * dev;

	dev = search_device(name, DEVICE_TYPE_GMETER);
	if(!dev)
		return NULL;
	return (struct gmeter_t *)dev->priv;
}

struct gmeter_t * search_first_gmeter(void)
{
	struct device_t * dev;

	dev = search_first_device(DEVICE_TYPE_GMETER);
	if(!dev)
		return NULL;
	return (struct gmeter_t *)dev->priv;
}

bool_t register_gmeter(struct device_t ** device,struct gmeter_t * g)
{
	struct device_t * dev;

	if(!g || !g->name)
		return FALSE;

	dev = malloc(sizeof(struct device_t));
	if(!dev)
		return FALSE;

	dev->name = strdup(g->name);
	dev->type = DEVICE_TYPE_GMETER;
	dev->priv = g;
	dev->kobj = kobj_alloc_directory(dev->name);
	kobj_add_regular(dev->kobj, "acceleration", gmeter_read_acceleration, NULL, g);

	if(!register_device(dev))
	{
		kobj_remove_self(dev->kobj);
		free(dev->name);
		free(dev);
		return FALSE;
	}

	if(device)
		*device = dev;
	return TRUE;
}

bool_t unregister_gmeter(struct gmeter_t * g)
{
	struct device_t * dev;

	if(!g || !g->name)
		return FALSE;

	dev = search_device(g->name, DEVICE_TYPE_GMETER);
	if(!dev)
		return FALSE;

	if(!unregister_device(dev))
		return FALSE;

	kobj_remove_self(dev->kobj);
	free(dev->name);
	free(dev);
	return TRUE;
}

bool_t gmeter_get_acceleration(struct gmeter_t * g, int * x, int * y, int * z)
{
	if(g && g->get)
		return g->get(g, x, y, z);
	return FALSE;
}
