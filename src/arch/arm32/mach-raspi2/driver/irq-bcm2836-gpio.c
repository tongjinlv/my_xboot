/*
 * driver/irq-bcm2836-gpio.c
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
#include <interrupt/interrupt.h>

enum {
	GPIO_FSEL	= 0x00,
	GPIO_SET	= 0x1c,
	GPIO_CLR	= 0x28,
	GPIO_LEV	= 0x34,
	GPIO_EDS	= 0x40,
	GPIO_REN	= 0x4c,
	GPIO_FEN	= 0x58,
	GPIO_HEN	= 0x64,
	GPIO_LEN	= 0x70,
	GPIO_ARE	= 0x7c,
	GPIO_AFE	= 0x88,
	GPIO_UD		= 0x94,
	GPIO_UDCLK	= 0x98,
};

struct irq_bcm2836_gpio_pdata_t
{
	virtual_addr_t virt;
	int base;
	int nirq;
	int parent;
};

static void irq_bcm2836_gpio_enable(struct irqchip_t * chip, int offset)
{
}

static void irq_bcm2836_gpio_disable(struct irqchip_t * chip, int offset)
{
	struct irq_bcm2836_gpio_pdata_t * pdat = (struct irq_bcm2836_gpio_pdata_t *)chip->priv;

	write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) & ~(1 << offset));
	write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) & ~(1 << offset));
	write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
	write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
	write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
	write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
}

static void irq_bcm2836_gpio_settype(struct irqchip_t * chip, int offset, enum irq_type_t type)
{
	struct irq_bcm2836_gpio_pdata_t * pdat = (struct irq_bcm2836_gpio_pdata_t *)chip->priv;

	switch(type)
	{
	case IRQ_TYPE_NONE:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) & ~(1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	case IRQ_TYPE_LEVEL_LOW:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) & ~(1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) |  (1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	case IRQ_TYPE_LEVEL_HIGH:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) & ~(1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) |  (1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	case IRQ_TYPE_EDGE_FALLING:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) & ~(1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) |  (1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	case IRQ_TYPE_EDGE_RISING:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) |  (1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	case IRQ_TYPE_EDGE_BOTH:
		write32(pdat->virt + GPIO_REN, read32(pdat->virt + GPIO_REN) |  (1 << offset));
		write32(pdat->virt + GPIO_FEN, read32(pdat->virt + GPIO_FEN) |  (1 << offset));
		write32(pdat->virt + GPIO_HEN, read32(pdat->virt + GPIO_HEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_LEN, read32(pdat->virt + GPIO_LEN) & ~(1 << offset));
		write32(pdat->virt + GPIO_ARE, read32(pdat->virt + GPIO_ARE) & ~(1 << offset));
		write32(pdat->virt + GPIO_AFE, read32(pdat->virt + GPIO_AFE) & ~(1 << offset));
		break;

	default:
		break;
	}
}

static void irq_bcm2836_gpio_dispatch(struct irqchip_t * chip)
{
	struct irq_bcm2836_gpio_pdata_t * pdat = (struct irq_bcm2836_gpio_pdata_t *)chip->priv;
	u32_t pend = read32(pdat->virt + GPIO_EDS);

	if(pend != 0)
	{
		u32_t offset = __ffs(pend);
		if((offset >= 0) && (offset < chip->nirq))
		{
			(chip->handler[offset].func)(chip->handler[offset].data);
			write32(pdat->virt + GPIO_EDS, (0x1 << offset));
		}
	}
}

static struct device_t * irq_bcm2836_gpio_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct irq_bcm2836_gpio_pdata_t * pdat;
	struct irqchip_t * chip;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	int base = dt_read_int(n, "interrupt-base", -1);
	int nirq = dt_read_int(n, "interrupt-count", -1);
	int parent = dt_read_int(n, "interrupt-parent", -1);

	if((base < 0) || (nirq <= 0) || !irq_is_valid(parent))
		return NULL;

	pdat = malloc(sizeof(struct irq_bcm2836_gpio_pdata_t));
	if(!pdat)
		return NULL;

	chip = malloc(sizeof(struct irqchip_t));
	if(!chip)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->base = base;
	pdat->nirq = nirq;
	pdat->parent = parent;

	chip->name = alloc_device_name(dt_read_name(n), -1);
	chip->base = pdat->base;
	chip->nirq = pdat->nirq;
	chip->handler = malloc(sizeof(struct irq_handler_t) * pdat->nirq);
	chip->enable = irq_bcm2836_gpio_enable;
	chip->disable = irq_bcm2836_gpio_disable;
	chip->settype = irq_bcm2836_gpio_settype;
	chip->dispatch = irq_bcm2836_gpio_dispatch;
	chip->priv = pdat;

	if(!register_sub_irqchip(&dev, pdat->parent, chip))
	{
		free_device_name(chip->name);
		free(chip->handler);
		free(chip->priv);
		free(chip);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void irq_bcm2836_gpio_remove(struct device_t * dev)
{
	struct irqchip_t * chip = (struct irqchip_t *)dev->priv;
	struct irq_bcm2836_gpio_pdata_t * pdat = (struct irq_bcm2836_gpio_pdata_t *)chip->priv;

	if(chip && unregister_sub_irqchip(pdat->parent, chip))
	{
		free_device_name(chip->name);
		free(chip->handler);
		free(chip->priv);
		free(chip);
	}
}

static void irq_bcm2836_gpio_suspend(struct device_t * dev)
{
}

static void irq_bcm2836_gpio_resume(struct device_t * dev)
{
}

static struct driver_t irq_bcm2836_gpio = {
	.name		= "irq-bcm2836-gpio",
	.probe		= irq_bcm2836_gpio_probe,
	.remove		= irq_bcm2836_gpio_remove,
	.suspend	= irq_bcm2836_gpio_suspend,
	.resume		= irq_bcm2836_gpio_resume,
};

static __init void irq_bcm2836_gpio_driver_init(void)
{
	register_driver(&irq_bcm2836_gpio);
}

static __exit void irq_bcm2836_gpio_driver_exit(void)
{
	unregister_driver(&irq_bcm2836_gpio);
}

driver_initcall(irq_bcm2836_gpio_driver_init);
driver_exitcall(irq_bcm2836_gpio_driver_exit);
