/*
 * driver/compass/compass.c
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
#include <compass/compass.h>

static ssize_t compass_read_offset(struct kobj_t * kobj, void * buf, size_t size)
{
	struct compass_t * c = (struct compass_t *)kobj->priv;
	int ox = 0, oy = 0, oz = 0;
	compass_get_offset(c, &ox, &oy, &oz);
	return sprintf(buf, "[%d %d %d]", ox, oy, oz);
}

static ssize_t compass_read_magnetic(struct kobj_t * kobj, void * buf, size_t size)
{
	struct compass_t * c = (struct compass_t *)kobj->priv;
	int x = 0, y = 0, z = 0;
	compass_get_magnetic(c, &x, &y, &z);
	return sprintf(buf, "[%d.%06d %d.%06d %d.%06d] gauss", x / 1000000, abs(x % 1000000), y / 1000000, abs(y % 1000000), z / 1000000, abs(z % 1000000));
}

static ssize_t compass_read_heading(struct kobj_t * kobj, void * buf, size_t size)
{
	struct compass_t * c = (struct compass_t *)kobj->priv;
	int heading = 0;
	compass_get_heading(c, &heading, 0);
	return sprintf(buf, "%d", heading);
}

struct compass_t * search_compass(const char * name)
{
	struct device_t * dev;

	dev = search_device(name, DEVICE_TYPE_COMPASS);
	if(!dev)
		return NULL;
	return (struct compass_t *)dev->priv;
}

struct compass_t * search_first_compass(void)
{
	struct device_t * dev;

	dev = search_first_device(DEVICE_TYPE_COMPASS);
	if(!dev)
		return NULL;
	return (struct compass_t *)dev->priv;
}

bool_t register_compass(struct device_t ** device,struct compass_t * c)
{
	struct device_t * dev;

	if(!c || !c->name)
		return FALSE;

	dev = malloc(sizeof(struct device_t));
	if(!dev)
		return FALSE;

	dev->name = strdup(c->name);
	dev->type = DEVICE_TYPE_COMPASS;
	dev->priv = c;
	dev->kobj = kobj_alloc_directory(dev->name);
	kobj_add_regular(dev->kobj, "offset", compass_read_offset, NULL, c);
	kobj_add_regular(dev->kobj, "magnetic", compass_read_magnetic, NULL, c);
	kobj_add_regular(dev->kobj, "heading", compass_read_heading, NULL, c);

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

bool_t unregister_compass(struct compass_t * c)
{
	struct device_t * dev;

	if(!c || !c->name)
		return FALSE;

	dev = search_device(c->name, DEVICE_TYPE_COMPASS);
	if(!dev)
		return FALSE;

	if(!unregister_device(dev))
		return FALSE;

	kobj_remove_self(dev->kobj);
	free(dev->name);
	free(dev);
	return TRUE;
}

bool_t compass_set_offset(struct compass_t * c, int ox, int oy, int oz)
{
	if(!c)
		return FALSE;
	c->ox = ox;
	c->oy = oy;
	c->oz = oz;
	return TRUE;
}

bool_t compass_get_offset(struct compass_t * c, int * ox, int * oy, int * oz)
{
	if(!c || !ox || !oy || !oz)
		return FALSE;
	*ox = c->ox;
	*oy = c->oy;
	*oz = c->oz;
	return TRUE;
}

bool_t compass_get_magnetic(struct compass_t * c, int * x, int * y, int * z)
{
	int tx, ty, tz;

	if(!c || !c->get || !c->get(c, &tx, &ty, &tz))
		return FALSE;
	*x = tx + c->ox;
	*y = ty + c->oy;
	*z = tz + c->oz;
	return TRUE;
}

bool_t compass_get_heading(struct compass_t * c, int * heading, int declination)
{
	int x, y, z;
	int angle;

	if(heading && compass_get_magnetic(c, &x, &y, &z))
	{
		angle = (int)(atan2((double)y, (double)x) * (180.0 / M_PI)) + declination;
		while(angle < 0)
			angle += 360;
		while(angle > 360)
			angle -= 360;
		*heading = angle;
		return TRUE;
	}
	return FALSE;
}
