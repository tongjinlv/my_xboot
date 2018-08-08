#ifndef __S5PV210_IRQ_H__
#define __S5PV210_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#define S5PV210_IRQ_EINT0			(0)
#define S5PV210_IRQ_EINT1			(1)
#define S5PV210_IRQ_EINT2			(2)
#define S5PV210_IRQ_EINT3			(3)
#define S5PV210_IRQ_EINT4			(4)
#define S5PV210_IRQ_EINT5			(5)
#define S5PV210_IRQ_EINT6			(6)
#define S5PV210_IRQ_EINT7			(7)
#define S5PV210_IRQ_EINT8			(8)
#define S5PV210_IRQ_EINT9			(9)
#define S5PV210_IRQ_EINT10			(10)
#define S5PV210_IRQ_EINT11			(11)
#define S5PV210_IRQ_EINT12			(12)
#define S5PV210_IRQ_EINT13			(13)
#define S5PV210_IRQ_EINT14			(14)
#define S5PV210_IRQ_EINT15			(15)
#define S5PV210_IRQ_EINT16_31		(16)
#define S5PV210_IRQ_MDMA			(18)
#define S5PV210_IRQ_PDMA0			(19)
#define S5PV210_IRQ_PDMA1			(20)
#define S5PV210_IRQ_TIMER0			(21)
#define S5PV210_IRQ_TIMER1			(22)
#define S5PV210_IRQ_TIMER2			(23)
#define S5PV210_IRQ_TIMER3			(24)
#define S5PV210_IRQ_TIMER4			(25)
#define S5PV210_IRQ_SYSTIMER		(26)
#define S5PV210_IRQ_WDT				(27)
#define S5PV210_IRQ_RTC_ALARM		(28)
#define S5PV210_IRQ_RTC_TIC			(29)
#define S5PV210_IRQ_GPIOINT			(30)
#define S5PV210_IRQ_FIMC3			(31)

#define S5PV210_IRQ_CORTEX0			(32)
#define S5PV210_IRQ_CORTEX1			(33)
#define S5PV210_IRQ_CORTEX2			(34)
#define S5PV210_IRQ_CORTEX3			(35)
#define S5PV210_IRQ_CORTEX4			(36)
#define S5PV210_IRQ_IEM_APC			(37)
#define S5PV210_IRQ_IEM_IEC			(38)
#define S5PV210_IRQ_NFC				(40)
#define S5PV210_IRQ_CFC				(41)
#define S5PV210_IRQ_UART0			(42)
#define S5PV210_IRQ_UART1			(43)
#define S5PV210_IRQ_UART2			(44)
#define S5PV210_IRQ_UART3			(45)
#define S5PV210_IRQ_I2C0			(46)
#define S5PV210_IRQ_SPI0			(47)
#define S5PV210_IRQ_SPI1			(48)
#define S5PV210_IRQ_AUDIO_SS		(50)
#define S5PV210_IRQ_I2C2			(51)
#define S5PV210_IRQ_I2C_HDMI_PHY	(52)
#define S5PV210_IRQ_UHOST			(55)
#define S5PV210_IRQ_OTG				(56)
#define S5PV210_IRQ_MODEMIF			(57)
#define S5PV210_IRQ_HSMMC0			(58)
#define S5PV210_IRQ_HSMMC1			(59)
#define S5PV210_IRQ_HSMMC2			(60)
#define S5PV210_IRQ_MIPI_DSI		(61)
#define S5PV210_IRQ_MIPI_CSI		(62)
#define S5PV210_IRQ_ONENAND			(63)

#define S5PV210_IRQ_LCD0			(64)
#define S5PV210_IRQ_LCD1			(65)
#define S5PV210_IRQ_LCD2			(66)
#define S5PV210_IRQ_ROTATOR			(68)
#define S5PV210_IRQ_FIMC0			(69)
#define S5PV210_IRQ_FIMC1			(70)
#define S5PV210_IRQ_FIMC2			(71)
#define S5PV210_IRQ_JPEG			(72)
#define S5PV210_IRQ_2D				(73)
#define S5PV210_IRQ_3D				(74)
#define S5PV210_IRQ_MIXER			(75)
#define S5PV210_IRQ_HDMI			(76)
#define S5PV210_IRQ_I2C_HDMI_DDC	(77)
#define S5PV210_IRQ_MFC				(78)
#define S5PV210_IRQ_TVENC			(79)
#define S5PV210_IRQ_I2S0			(80)
#define S5PV210_IRQ_I2S1			(81)
#define S5PV210_IRQ_AC97			(83)
#define S5PV210_IRQ_PCM0			(84)
#define S5PV210_IRQ_PCM1			(85)
#define S5PV210_IRQ_SPDIF			(86)
#define S5PV210_IRQ_ADC				(87)
#define S5PV210_IRQ_PENDN			(88)
#define S5PV210_IRQ_KEYPAD			(89)
#define S5PV210_IRQ_HASH_SSS		(91)
#define S5PV210_IRQ_FEEDCTRL		(92)
#define S5PV210_IRQ_PCM2			(93)
#define S5PV210_IRQ_SDM_IRQ			(34)
#define S5PV210_IRQ_SDM_FIQ			(95)

#define S5PV210_IRQ_MMC3			(98)
#define S5PV210_IRQ_CEC				(99)
#define S5PV210_IRQ_TSI				(100)
#define S5PV210_IRQ_ADC1			(105)
#define S5PV210_IRQ_PENDN1			(106)

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_IRQ_H__ */
