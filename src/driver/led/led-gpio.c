/*
 * driver/led/led-gpio.c
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
#include <gpio/gpio.h>
#include <led/led.h>

/*
 * GPIO LED - LED Driver Using Generic Purpose Input Output
 *
 * Required properties:
 * - gpio: led attached pin
 *
 * Optional properties:
 * - active-low: low level for active led
 * - default-brightness: led default brightness
 *
 * Example:
 *   "led-gpio@0": {
 *       "gpio": 0,
 *       "gpio-config": -1,
 *       "active-low": true,
 *       "default-brightness": 0
 *   }
 */

struct led_gpio_pdata_t {
	int gpio;
	int gpiocfg;
	int active_low;
	int brightness;
};

static void led_gpio_set_brightness(struct led_gpio_pdata_t * pdat, int brightness)
{
	if(brightness > 0)
		gpio_set_value(pdat->gpio, pdat->active_low ? 0 : 1);
	else
		gpio_set_value(pdat->gpio, pdat->active_low ? 1 : 0);
}

static void led_gpio_set(struct led_t * led, int brightness)
{
	struct led_gpio_pdata_t * pdat = (struct led_gpio_pdata_t *)led->priv;

	if(pdat->brightness != brightness)
	{
		led_gpio_set_brightness(pdat, brightness);
		pdat->brightness = brightness;
	}
}

static int led_gpio_get(struct led_t * led)
{
	struct led_gpio_pdata_t * pdat = (struct led_gpio_pdata_t *)led->priv;
	return pdat->brightness;
}

static struct device_t * led_gpio_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct led_gpio_pdata_t * pdat;
	struct led_t * led;
	struct device_t * dev;

	if(!gpio_is_valid(dt_read_int(n, "gpio", -1)))
		return NULL;

	pdat = malloc(sizeof(struct led_gpio_pdata_t));
	if(!pdat)
		return NULL;

	led = malloc(sizeof(struct led_t));
	if(!led)
	{
		free(pdat);
		return NULL;
	}

	pdat->gpio = dt_read_int(n, "gpio", -1);
	pdat->gpiocfg = dt_read_int(n, "gpio-config", -1);
	pdat->active_low = dt_read_bool(n, "active-low", 0);
	pdat->brightness = -1;

	led->name = alloc_device_name(dt_read_name(n), dt_read_id(n));
	led->set = led_gpio_set;
	led->get = led_gpio_get;
	led->priv = pdat;

	if(pdat->gpiocfg >= 0)
		gpio_set_cfg(pdat->gpio, pdat->gpiocfg);
	gpio_set_pull(pdat->gpio, pdat->active_low ? GPIO_PULL_UP :GPIO_PULL_DOWN);
	gpio_set_direction(pdat->gpio, GPIO_DIRECTION_OUTPUT);
	led_gpio_set(led, dt_read_int(n, "default-brightness", 0));

	if(!register_led(&dev, led))
	{
		free_device_name(led->name);
		free(led->priv);
		free(led);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void led_gpio_remove(struct device_t * dev)
{
	struct led_t * led = (struct led_t *)dev->priv;

	if(led && unregister_led(led))
	{
		free_device_name(led->name);
		free(led->priv);
		free(led);
	}
}

static void led_gpio_suspend(struct device_t * dev)
{
	struct led_t * led = (struct led_t *)dev->priv;
	struct led_gpio_pdata_t * pdat = (struct led_gpio_pdata_t *)led->priv;
	led_gpio_set_brightness(pdat, 0);
}

static void led_gpio_resume(struct device_t * dev)
{
	struct led_t * led = (struct led_t *)dev->priv;
	struct led_gpio_pdata_t * pdat = (struct led_gpio_pdata_t *)led->priv;
	led_gpio_set_brightness(pdat, pdat->brightness);
}

static struct driver_t led_gpio = {
	.name		= "led-gpio",
	.probe		= led_gpio_probe,
	.remove		= led_gpio_remove,
	.suspend	= led_gpio_suspend,
	.resume		= led_gpio_resume,
};

static __init void led_gpio_driver_init(void)
{
	register_driver(&led_gpio);
}

static __exit void led_gpio_driver_exit(void)
{
	unregister_driver(&led_gpio);
}

driver_initcall(led_gpio_driver_init);
driver_exitcall(led_gpio_driver_exit);
