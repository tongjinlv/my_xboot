/*
 * driver/light-cm32181.c
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
#include <i2c/i2c.h>
#include <light/light.h>

enum {
	CM32181_REG_CMD		= 0x00,
	CM32181_REG_ALS		= 0x04,
	CM32181_REG_STATUS	= 0x06,
	CM32181_REG_ID		= 0x07,
};

struct light_cm32181_pdata_t {
	struct i2c_device_t * dev;
};

static bool_t cm32181_read(struct i2c_device_t * dev, u8_t reg, u16_t * val)
{
	struct i2c_msg_t msgs[2];
	u8_t buf[2];

	msgs[0].addr = dev->addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &reg;

	msgs[1].addr = dev->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = 2;
	msgs[1].buf = &buf[0];

	if(i2c_transfer(dev->i2c, msgs, 2) != 2)
		return FALSE;

    if(val)
    	*val = ((u16_t)buf[1] << 8) | buf[0];
    return TRUE;
}

static bool_t cm32181_write(struct i2c_device_t * dev, u8_t reg, u16_t val)
{
	struct i2c_msg_t msg;
	u8_t buf[3];

	buf[0] = reg;
	buf[1] = (val >> 0) & 0xff;
	buf[2] = (val >> 8) & 0xff;
	msg.addr = dev->addr;
	msg.flags = 0;
	msg.len = 3;
	msg.buf = &buf[0];

    if(i2c_transfer(dev->i2c, &msg, 1) != 1)
    	return FALSE;
    return TRUE;
}

static int light_cm32181_get(struct light_t * light)
{
	struct light_cm32181_pdata_t * pdat = (struct light_cm32181_pdata_t *)light->priv;
	u16_t val;

	if(cm32181_read(pdat->dev, CM32181_REG_ALS, &val))
		return val * 42;
	return 0;
}

static struct device_t * light_cm32181_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct light_cm32181_pdata_t * pdat;
	struct light_t * light;
	struct device_t * dev;
	struct i2c_device_t * i2cdev;
	u16_t val;

	i2cdev = i2c_device_alloc(dt_read_string(n, "i2c-bus", NULL), dt_read_int(n, "slave-address", 0x10), 0);
	if(!i2cdev)
		return NULL;

	if(cm32181_read(i2cdev, CM32181_REG_ID, &val) && ((val & 0xff) == 0x81))
	{
		cm32181_write(i2cdev, CM32181_REG_CMD, (1 << 12) | (0 << 6) | (0 << 4) | (0 << 1) | (0 << 0));
	}
	else
	{
		i2c_device_free(i2cdev);
		return NULL;
	}

	pdat = malloc(sizeof(struct light_cm32181_pdata_t));
	if(!pdat)
	{
		i2c_device_free(i2cdev);
		return NULL;
	}

	light = malloc(sizeof(struct light_t));
	if(!light)
	{
		i2c_device_free(i2cdev);
		free(pdat);
		return NULL;
	}

	pdat->dev = i2cdev;

	light->name = alloc_device_name(dt_read_name(n), -1);
	light->get = light_cm32181_get;
	light->priv = pdat;

	if(!register_light(&dev, light))
	{
		i2c_device_free(pdat->dev);

		free_device_name(light->name);
		free(light->priv);
		free(light);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void light_cm32181_remove(struct device_t * dev)
{
	struct light_t * light = (struct light_t *)dev->priv;
	struct light_cm32181_pdata_t * pdat = (struct light_cm32181_pdata_t *)light->priv;

	if(light && unregister_light(light))
	{
		i2c_device_free(pdat->dev);

		free_device_name(light->name);
		free(light->priv);
		free(light);
	}
}

static void light_cm32181_suspend(struct device_t * dev)
{
}

static void light_cm32181_resume(struct device_t * dev)
{
}

static struct driver_t light_cm32181 = {
	.name		= "light-cm32181",
	.probe		= light_cm32181_probe,
	.remove		= light_cm32181_remove,
	.suspend	= light_cm32181_suspend,
	.resume		= light_cm32181_resume,
};

static __init void light_cm32181_driver_init(void)
{
	register_driver(&light_cm32181);
}

static __exit void light_cm32181_driver_exit(void)
{
	unregister_driver(&light_cm32181);
}

driver_initcall(light_cm32181_driver_init);
driver_exitcall(light_cm32181_driver_exit);
