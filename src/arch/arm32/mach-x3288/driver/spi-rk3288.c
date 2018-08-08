/*
 * driver/spi-rk3288.c
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
#include <spi/spi.h>

enum {
	SPI_CTRLR0	= 0x000,
	SPI_CTRLR1	= 0x004,
	SPI_ENR		= 0x008,
	SPI_SER		= 0x00c,
	SPI_BAUDR	= 0x010,
	SPI_TXFTLR	= 0x014,
	SPI_RXFTLR	= 0x018,
	SPI_TXFLR	= 0x01c,
	SPI_RXFLR	= 0x020,
	SPI_SR		= 0x024,
	SPI_IPR		= 0x028,
	SPI_IMR		= 0x02c,
	SPI_ISR		= 0x030,
	SPI_RISR	= 0x034,
	SPI_ICR		= 0x038,
	SPI_DMACR	= 0x03c,
	SPI_DMATDLR	= 0x040,
	SPI_DMARDLR	= 0x044,
	SPI_TXDR	= 0x400,
	SPI_RXDR	= 0x800,
};

struct spi_rk3288_pdata_t {
	virtual_addr_t virt;
	char * clk;
	int sclk;
	int sclkcfg;
	int mosi;
	int mosicfg;
	int miso;
	int misocfg;
	int cs;
	int cscfg;
};

static void rk3288_spi_enable_chip(struct spi_rk3288_pdata_t * pdat, int enable)
{
	write32(pdat->virt + SPI_ENR, enable ? 1 : 0);
}

static void rk3288_spi_set_rate(struct spi_rk3288_pdata_t * pdat, u64_t rate)
{
	u64_t pclk = clk_get_rate(pdat->clk);
	u32_t div = pclk / rate;

	div = (div + 1) & 0xfffe;
	write32(pdat->virt + SPI_BAUDR, div);
}

static void rk3288_spi_set_mode(struct spi_rk3288_pdata_t * pdat, int mode)
{
	u32_t val;

	val = read32(pdat->virt + SPI_CTRLR0);
	val &= ~(0x3 << 6);
	val |= (mode & 0x3) << 6;
	write32(pdat->virt + SPI_CTRLR0, val);
}

static void rk3288_spi_set_size(struct spi_rk3288_pdata_t * pdat, int size)
{
	u32_t val;

	val = read32(pdat->virt + SPI_CTRLR0);
	val &= ~((0x1 << 13) | (0x3 << 0));
	switch(size)
	{
	case 4:
		val |= (0x1 << 13) | (0x0 << 0);
		break;
	case 8:
		val |= (0x1 << 13) | (0x1 << 0);
		break;
	case 16:
		val |= (0x0 << 13) | (0x2 << 0);
		break;
	default:
		val |= (0x1 << 13) | (0x1 << 0);
		break;
	}
	write32(pdat->virt + SPI_CTRLR0, val);
}

static bool_t rk3288_spi_wait_till_not_busy(struct spi_rk3288_pdata_t * pdat)
{
	ktime_t timeout = ktime_add_ms(ktime_get(), 1);

	do {
		if(!(read32(pdat->virt + SPI_SR) & (1 << 0)))
			return TRUE;
	} while(ktime_before(ktime_get(), timeout));

	return FALSE;
}

static int rk3288_spi_xfer_8(struct spi_rk3288_pdata_t * pdat, struct spi_msg_t * msg)
{
	int count = msg->len;
	u8_t * tx = msg->txbuf;
	u8_t * rx = msg->rxbuf;
	u8_t val;

	while(count > 0)
	{
		val = 0;
		if(tx)
			val = *tx++;

		while(read32(pdat->virt + SPI_SR) & (1 << 1));
		write32(pdat->virt + SPI_TXDR, val);
		rk3288_spi_wait_till_not_busy(pdat);
		//todo
		//if(!(read32(pdat->virt + SPI_SR) & (1 << 3)))
		//val = read32(pdat->virt + SPI_RXDR);
		if(rx)
			*rx++ = val;
		count -= 1;
	}
	return msg->len - count;
}

static int rk3288_spi_xfer_16(struct spi_rk3288_pdata_t * pdat, struct spi_msg_t * msg)
{
	int count = msg->len;
	u16_t * tx = msg->txbuf;
	u16_t * rx = msg->rxbuf;
	u16_t val;

	while(count > 1)
	{
		val = 0;
		if(tx)
			val = *tx++;

		while(read32(pdat->virt + SPI_SR) & (1 << 1));
		write32(pdat->virt + SPI_TXDR, val);
		rk3288_spi_wait_till_not_busy(pdat);
		//todo
		//if(!(read32(pdat->virt + SPI_SR) & (1 << 3)))
		//val = read32(pdat->virt + SPI_RXDR);

		if(rx)
			*rx++ = val;
		count -= 2;
	}
	return msg->len - count;
}

static int rk3288_spi_xfer_32(struct spi_rk3288_pdata_t * pdat, struct spi_msg_t * msg)
{
	int count = msg->len;
	u32_t * tx = msg->txbuf;
	u32_t * rx = msg->rxbuf;
	u32_t val;

	while(count > 3)
	{
		val = 0;
		if(tx)
			val = *tx++;

		while(read32(pdat->virt + SPI_SR) & (1 << 1));
		write32(pdat->virt + SPI_TXDR, val);
		rk3288_spi_wait_till_not_busy(pdat);
		//todo
		//if(!(read32(pdat->virt + SPI_SR) & (1 << 3)))
		//val = read32(pdat->virt + SPI_RXDR);

		if(rx)
			*rx++ = val;
		count -= 4;
	}
	return msg->len - count;
}

static int spi_rk3288_transfer(struct spi_t * spi, struct spi_msg_t * msg)
{
	struct spi_rk3288_pdata_t * pdat = (struct spi_rk3288_pdata_t *)spi->priv;
	int ret = 0;

	rk3288_spi_set_mode(pdat, msg->mode);
	rk3288_spi_set_size(pdat, msg->bits);
	rk3288_spi_set_rate(pdat, (msg->speed > 0) ? msg->speed : 1000000);

	rk3288_spi_enable_chip(pdat, 1);
	if(msg->bits <= 8)
	{
		ret = rk3288_spi_xfer_8(pdat, msg);
	}
	else if(msg->bits <= 16)
	{
		ret = rk3288_spi_xfer_16(pdat, msg);
	}
	else if(msg->bits <= 32)
	{
		ret = rk3288_spi_xfer_32(pdat, msg);
	}
	rk3288_spi_enable_chip(pdat, 0);

	return ret;
}

static void spi_rk3288_select(struct spi_t * spi, int cs)
{
	struct spi_rk3288_pdata_t * pdat = (struct spi_rk3288_pdata_t *)spi->priv;
	write32(pdat->virt + SPI_SER, 1 << cs);
}

static void spi_rk3288_deselect(struct spi_t * spi, int cs)
{
	struct spi_rk3288_pdata_t * pdat = (struct spi_rk3288_pdata_t *)spi->priv;
	write32(pdat->virt + SPI_SER, 0);
}

static struct device_t * spi_rk3288_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct spi_rk3288_pdata_t * pdat;
	struct spi_t * spi;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk = dt_read_string(n, "clock-name", NULL);

	pdat = malloc(sizeof(struct spi_rk3288_pdata_t));
	if(!pdat)
		return FALSE;

	spi = malloc(sizeof(struct spi_t));
	if(!spi)
	{
		free(pdat);
		return FALSE;
	}

	pdat->virt = virt;
	pdat->clk = strdup(clk);
	pdat->sclk = dt_read_int(n, "sclk-gpio", -1);
	pdat->sclkcfg = dt_read_int(n, "sclk-gpio-config", -1);
	pdat->mosi = dt_read_int(n, "mosi-gpio", -1);;
	pdat->mosicfg = dt_read_int(n, "mosi-gpio-config", -1);
	pdat->miso = dt_read_int(n, "miso-gpio", -1);;
	pdat->misocfg = dt_read_int(n, "miso-gpio-config", -1);
	pdat->cs = dt_read_int(n, "cs-gpio", -1);
	pdat->cscfg = dt_read_int(n, "cs-gpio-config", -1);

	spi->name = alloc_device_name(dt_read_name(n), -1);
	spi->transfer = spi_rk3288_transfer;
	spi->select = spi_rk3288_select;
	spi->deselect = spi_rk3288_deselect;
	spi->priv = pdat;

	clk_enable(pdat->clk);
	if(pdat->sclk >= 0)
	{
		if(pdat->sclkcfg >= 0)
			gpio_set_cfg(pdat->sclk, pdat->sclkcfg);
		gpio_set_pull(pdat->sclk, GPIO_PULL_NONE);
	}
	if(pdat->mosi >= 0)
	{
		if(pdat->mosicfg >= 0)
			gpio_set_cfg(pdat->mosi, pdat->mosicfg);
		gpio_set_pull(pdat->mosi, GPIO_PULL_NONE);
	}
	if(pdat->miso >= 0)
	{
		if(pdat->misocfg >= 0)
			gpio_set_cfg(pdat->miso, pdat->misocfg);
		gpio_set_pull(pdat->miso, GPIO_PULL_NONE);
	}
	if(pdat->cs >= 0)
	{
		if(pdat->cscfg >= 0)
			gpio_set_cfg(pdat->cs, pdat->cscfg);
		gpio_set_pull(pdat->cs, GPIO_PULL_NONE);
	}

	rk3288_spi_enable_chip(pdat, 0);
	write32(pdat->virt + SPI_CTRLR0, (0 << 20) | (0 << 18) | (0 << 16) | (3 << 14) | (0 << 12) | (0 << 11) | (1 << 10) | (0 << 8) | (1 << 0));
	write32(pdat->virt + SPI_TXFTLR, 32 / 2 - 1);
	write32(pdat->virt + SPI_RXFTLR, 32 / 2 - 1);

	if(!register_spi(&dev, spi))
	{
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(spi->name);
		free(spi->priv);
		free(spi);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void spi_rk3288_remove(struct device_t * dev)
{
	struct spi_t * spi = (struct spi_t *)dev->priv;
	struct spi_rk3288_pdata_t * pdat = (struct spi_rk3288_pdata_t *)spi->priv;

	if(spi && unregister_spi(spi))
	{
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(spi->name);
		free(spi->priv);
		free(spi);
	}
}

static void spi_rk3288_suspend(struct device_t * dev)
{
}

static void spi_rk3288_resume(struct device_t * dev)
{
}

static struct driver_t spi_rk3288 = {
	.name		= "spi-rk3288",
	.probe		= spi_rk3288_probe,
	.remove		= spi_rk3288_remove,
	.suspend	= spi_rk3288_suspend,
	.resume		= spi_rk3288_resume,
};

static __init void spi_rk3288_driver_init(void)
{
	register_driver(&spi_rk3288);
}

static __exit void spi_rk3288_driver_exit(void)
{
	unregister_driver(&spi_rk3288);
}

driver_initcall(spi_rk3288_driver_init);
driver_exitcall(spi_rk3288_driver_exit);
