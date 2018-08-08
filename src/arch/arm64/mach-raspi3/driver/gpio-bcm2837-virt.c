/*
 * driver/gpio-bcm2837-virt.c
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
#include <bcm2837-mbox.h>

struct gpio_bcm2837_virt_pdata_t
{
	int base;
	int ngpio;
	int oirq;
	uint32_t virtbuf;
	uint32_t * status;
};

static void gpio_bcm2837_virt_set_cfg(struct gpiochip_t * chip, int offset, int cfg)
{
}

static int gpio_bcm2837_virt_get_cfg(struct gpiochip_t * chip, int offset)
{
	return 0;
}

static void gpio_bcm2837_virt_set_pull(struct gpiochip_t * chip, int offset, enum gpio_pull_t pull)
{
}

static enum gpio_pull_t gpio_bcm2837_virt_get_pull(struct gpiochip_t * chip, int offset)
{
	return GPIO_PULL_NONE;
}

static void gpio_bcm2837_virt_set_drv(struct gpiochip_t * chip, int offset, enum gpio_drv_t drv)
{
}

static enum gpio_drv_t gpio_bcm2837_virt_get_drv(struct gpiochip_t * chip, int offset)
{
	return GPIO_DRV_WEAK;
}

static void gpio_bcm2837_virt_set_rate(struct gpiochip_t * chip, int offset, enum gpio_rate_t rate)
{
}

static enum gpio_rate_t gpio_bcm2837_virt_get_rate(struct gpiochip_t * chip, int offset)
{
	return GPIO_RATE_SLOW;
}

static void gpio_bcm2837_virt_set_dir(struct gpiochip_t * chip, int offset, enum gpio_direction_t dir)
{
}

static enum gpio_direction_t gpio_bcm2837_virt_get_dir(struct gpiochip_t * chip, int offset)
{
	return GPIO_DIRECTION_OUTPUT;
}

static void gpio_bcm2837_virt_set_value(struct gpiochip_t * chip, int offset, int value)
{
	struct gpio_bcm2837_virt_pdata_t * pdat = (struct gpio_bcm2837_virt_pdata_t *)chip->priv;
	uint16_t enables, disables;
	int16_t diff, lit;

	enables = pdat->status[offset] >> 16;
	disables = pdat->status[offset] >> 0;
	diff = (int16_t)(enables - disables);
	lit = (diff > 0) ? 1 : 0;

	if((value && lit) || (!value && !lit))
		return;
	if(value)
		enables++;
	else
		disables++;

	pdat->status[offset] = (enables << 16) | (disables << 0);
	mb(); write32(pdat->virtbuf + offset * 4, pdat->status[offset]);
}

static int gpio_bcm2837_virt_get_value(struct gpiochip_t * chip, int offset)
{
	struct gpio_bcm2837_virt_pdata_t * pdat = (struct gpio_bcm2837_virt_pdata_t *)chip->priv;
	return (read32(pdat->virtbuf + offset * 4) >> offset) & 0x1;
}

static int gpio_bcm2837_virt_to_irq(struct gpiochip_t * chip, int offset)
{
	struct gpio_bcm2837_virt_pdata_t * pdat = (struct gpio_bcm2837_virt_pdata_t *)chip->priv;

	if((offset >= chip->ngpio) || (pdat->oirq < 0))
		return -1;
	return pdat->oirq + offset;
}

static struct device_t * gpio_bcm2837_virt_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct gpio_bcm2837_virt_pdata_t * pdat;
	struct gpiochip_t * chip;
	struct device_t * dev;
	int base = dt_read_int(n, "gpio-base", -1);
	int ngpio = dt_read_int(n, "gpio-count", -1);

	if((base < 0) || (ngpio <= 0))
		return NULL;

	pdat = malloc(sizeof(struct gpio_bcm2837_virt_pdata_t));
	if(!pdat)
		return NULL;

	chip = malloc(sizeof(struct gpiochip_t));
	if(!chip)
	{
		free(pdat);
		return NULL;
	}

	pdat->base = base;
	pdat->ngpio = ngpio;
	pdat->oirq = dt_read_int(n, "interrupt-offset", -1);
	pdat->virtbuf = bcm2837_mbox_fb_get_gpiovirt();
	pdat->status = malloc(sizeof(uint32_t) * pdat->ngpio);
	memset(pdat->status, 0, sizeof(uint32_t) * pdat->ngpio);

	chip->name = alloc_device_name(dt_read_name(n), -1);
	chip->base = pdat->base;
	chip->ngpio = pdat->ngpio;
	chip->set_cfg = gpio_bcm2837_virt_set_cfg;
	chip->get_cfg = gpio_bcm2837_virt_get_cfg;
	chip->set_pull = gpio_bcm2837_virt_set_pull;
	chip->get_pull = gpio_bcm2837_virt_get_pull;
	chip->set_drv = gpio_bcm2837_virt_set_drv;
	chip->get_drv = gpio_bcm2837_virt_get_drv;
	chip->set_rate = gpio_bcm2837_virt_set_rate;
	chip->get_rate = gpio_bcm2837_virt_get_rate;
	chip->set_dir = gpio_bcm2837_virt_set_dir;
	chip->get_dir = gpio_bcm2837_virt_get_dir;
	chip->set_value = gpio_bcm2837_virt_set_value;
	chip->get_value = gpio_bcm2837_virt_get_value;
	chip->to_irq = gpio_bcm2837_virt_to_irq;
	chip->priv = pdat;

	if(!register_gpiochip(&dev, chip))
	{
		free(pdat->status);

		free_device_name(chip->name);
		free(chip->priv);
		free(chip);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void gpio_bcm2837_virt_remove(struct device_t * dev)
{
	struct gpiochip_t * chip = (struct gpiochip_t *)dev->priv;
	struct gpio_bcm2837_virt_pdata_t * pdat = (struct gpio_bcm2837_virt_pdata_t *)chip->priv;

	if(chip && unregister_gpiochip(chip))
	{
		free(pdat->status);

		free_device_name(chip->name);
		free(chip->priv);
		free(chip);
	}
}

static void gpio_bcm2837_virt_suspend(struct device_t * dev)
{
}

static void gpio_bcm2837_virt_resume(struct device_t * dev)
{
}

static struct driver_t gpio_bcm2837_virt = {
	.name		= "gpio-bcm2837-virt",
	.probe		= gpio_bcm2837_virt_probe,
	.remove		= gpio_bcm2837_virt_remove,
	.suspend	= gpio_bcm2837_virt_suspend,
	.resume		= gpio_bcm2837_virt_resume,
};

static __init void gpio_bcm2837_virt_driver_init(void)
{
	register_driver(&gpio_bcm2837_virt);
}

static __exit void gpio_bcm2837_virt_driver_exit(void)
{
	unregister_driver(&gpio_bcm2837_virt);
}

driver_initcall(gpio_bcm2837_virt_driver_init);
driver_exitcall(gpio_bcm2837_virt_driver_exit);
