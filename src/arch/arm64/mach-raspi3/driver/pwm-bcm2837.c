/*
 * driver/pwm-bcm2837.c
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
#include <gpio/gpio.h>
#include <pwm/pwm.h>

#define PWM_CTRL			(0x00)
#define PWM_CTRL_MASK		(0xff)
#define PWM_CTRL_SHIFT(x)	((x) * 8)
#define PWM_STA				(0x04)
#define PWM_DMAC			(0x08)
#define PWM_FIFO			(0x18)
#define PWM_PERIOD(x)		(((x) * 0x10) + 0x10)
#define PWM_DUTY(x)			(((x) * 0x10) + 0x14)

struct pwm_bcm2837_pdata_t
{
	virtual_addr_t virt;
	char * clk;
	int channel;
	int pwm;
	int pwmcfg;
};

static void pwm_bcm2837_config(struct pwm_t * pwm, int duty, int period, int polarity)
{
	struct pwm_bcm2837_pdata_t * pdat = (struct pwm_bcm2837_pdata_t *)pwm->priv;
	unsigned long scaler = (unsigned long)(1000000000ULL / clk_get_rate(pdat->clk));
	u32_t ctrl;

	if(pwm->__duty != duty)
	{
		write32(pdat->virt + PWM_DUTY(pdat->channel), duty / scaler);
	}

	if(pwm->__period != period)
	{
		write32(pdat->virt + PWM_PERIOD(pdat->channel), period /scaler);
	}

	if(pwm->__polarity != polarity)
	{
		ctrl = read32(pdat->virt + PWM_CTRL);
		if(polarity)
			ctrl |= (0x10 << PWM_CTRL_SHIFT(pdat->channel));
		else
			ctrl &= ~(0x10 << PWM_CTRL_SHIFT(pdat->channel));
		write32(pdat->virt + PWM_CTRL, ctrl);
	}
}

static void pwm_bcm2837_enable(struct pwm_t * pwm)
{
	struct pwm_bcm2837_pdata_t * pdat = (struct pwm_bcm2837_pdata_t *)pwm->priv;
	u32_t ctrl;

	gpio_set_cfg(pdat->pwm, pdat->pwmcfg);
	clk_enable(pdat->clk);

	ctrl = read32(pdat->virt + PWM_CTRL);
	ctrl &= ~(PWM_CTRL_MASK << PWM_CTRL_SHIFT(pdat->channel));
	ctrl |= (0x81 << PWM_CTRL_SHIFT(pdat->channel));
	write32(pdat->virt + PWM_CTRL, ctrl);
}

static void pwm_bcm2837_disable(struct pwm_t * pwm)
{
	struct pwm_bcm2837_pdata_t * pdat = (struct pwm_bcm2837_pdata_t *)pwm->priv;
	u32_t ctrl;

	ctrl = read32(pdat->virt + PWM_CTRL);
	ctrl &= ~(PWM_CTRL_MASK << PWM_CTRL_SHIFT(pdat->channel));
	ctrl |= (0x00 << PWM_CTRL_SHIFT(pdat->channel));
	write32(pdat->virt + PWM_CTRL, ctrl);
	clk_disable(pdat->clk);
}

static struct device_t * pwm_bcm2837_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct pwm_bcm2837_pdata_t * pdat;
	struct pwm_t * pwm;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk = dt_read_string(n, "clock-name", NULL);
	int channel = dt_read_int(n, "channel", -1);

	if(channel < 0 || channel > 1)
		return NULL;

	if(!search_clk(clk))
		return NULL;

	pdat = malloc(sizeof(struct pwm_bcm2837_pdata_t));
	if(!pdat)
		return NULL;

	pwm = malloc(sizeof(struct pwm_t));
	if(!pwm)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->clk = strdup(clk);
	pdat->channel = channel;
	pdat->pwm = dt_read_int(n, "pwm-gpio", -1);
	pdat->pwmcfg = dt_read_int(n, "pwm-gpio-config", -1);

	pwm->name = alloc_device_name(dt_read_name(n), -1);
	pwm->config = pwm_bcm2837_config;
	pwm->enable = pwm_bcm2837_enable;
	pwm->disable = pwm_bcm2837_disable;
	pwm->priv = pdat;

	if(!register_pwm(&dev, pwm))
	{
		free(pdat->clk);

		free_device_name(pwm->name);
		free(pwm->priv);
		free(pwm);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void pwm_bcm2837_remove(struct device_t * dev)
{
	struct pwm_t * pwm = (struct pwm_t *)dev->priv;
	struct pwm_bcm2837_pdata_t * pdat = (struct pwm_bcm2837_pdata_t *)pwm->priv;

	if(pwm && unregister_pwm(pwm))
	{
		free(pdat->clk);

		free_device_name(pwm->name);
		free(pwm->priv);
		free(pwm);
	}
}

static void pwm_bcm2837_suspend(struct device_t * dev)
{
}

static void pwm_bcm2837_resume(struct device_t * dev)
{
}

static struct driver_t pwm_bcm2837 = {
	.name		= "pwm-bcm2837",
	.probe		= pwm_bcm2837_probe,
	.remove		= pwm_bcm2837_remove,
	.suspend	= pwm_bcm2837_suspend,
	.resume		= pwm_bcm2837_resume,
};

static __init void pwm_bcm2837_driver_init(void)
{
	register_driver(&pwm_bcm2837);
}

static __exit void pwm_bcm2837_driver_exit(void)
{
	unregister_driver(&pwm_bcm2837);
}

driver_initcall(pwm_bcm2837_driver_init);
driver_exitcall(pwm_bcm2837_driver_exit);
