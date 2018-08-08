#ifndef __BCM2836_IRQ_H__
#define __BCM2836_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * GPU interrupts
 */
#define BCM2836_IRQ_TIMER0			(0)
#define BCM2836_IRQ_TIMER1			(1)
#define BCM2836_IRQ_TIMER2			(2)
#define BCM2836_IRQ_TIMER3			(3)
#define BCM2836_IRQ_CODEC0			(4)
#define BCM2836_IRQ_CODEC1			(5)
#define BCM2836_IRQ_CODEC2			(6)
#define BCM2836_IRQ_JPEG			(7)
#define BCM2836_IRQ_ISP				(8)
#define BCM2836_IRQ_USB				(9)
#define BCM2836_IRQ_3D				(10)
#define BCM2836_IRQ_TRANSPOSER		(11)
#define BCM2836_IRQ_MULTICORESYNC0	(12)
#define BCM2836_IRQ_MULTICORESYNC1	(13)
#define BCM2836_IRQ_MULTICORESYNC2	(14)
#define BCM2836_IRQ_MULTICORESYNC3	(15)
#define BCM2836_IRQ_DMA0			(16)
#define BCM2836_IRQ_DMA1			(17)
#define BCM2836_IRQ_DMA2			(18)
#define BCM2836_IRQ_DMA3			(19)
#define BCM2836_IRQ_DMA4			(20)
#define BCM2836_IRQ_DMA5			(21)
#define BCM2836_IRQ_DMA6			(22)
#define BCM2836_IRQ_DMA7			(23)
#define BCM2836_IRQ_DMA8			(24)
#define BCM2836_IRQ_DMA9			(25)
#define BCM2836_IRQ_DMA10			(26)
#define BCM2836_IRQ_DMA11			(27)
#define BCM2836_IRQ_DMA12			(28)
#define BCM2836_IRQ_AUX				(29)
#define BCM2836_IRQ_ARM				(30)
#define BCM2836_IRQ_VPUDMA			(31)

#define BCM2836_IRQ_HOSTPORT		(32)
#define BCM2836_IRQ_VIDEOSCALER		(33)
#define BCM2836_IRQ_CCP2TX			(34)
#define BCM2836_IRQ_SDC				(35)
#define BCM2836_IRQ_DSI0			(36)
#define BCM2836_IRQ_AVE				(37)
#define BCM2836_IRQ_CAM0			(38)
#define BCM2836_IRQ_CAM1			(39)
#define BCM2836_IRQ_HDMI0			(40)
#define BCM2836_IRQ_HDMI1			(41)
#define BCM2836_IRQ_PIXELVALVE1		(42)
#define BCM2836_IRQ_I2CSPISLV		(43)
#define BCM2836_IRQ_DSI1			(44)
#define BCM2836_IRQ_PWA0			(45)
#define BCM2836_IRQ_PWA1			(46)
#define BCM2836_IRQ_CPR				(47)
#define BCM2836_IRQ_SMI				(48)
#define BCM2836_IRQ_GPIOP0			(49)
#define BCM2836_IRQ_GPIOP1			(50)
#define BCM2836_IRQ_GPIOP2			(51)
#define BCM2836_IRQ_GPIOP3			(52)
#define BCM2836_IRQ_I2C				(53)
#define BCM2836_IRQ_SPI				(54)
#define BCM2836_IRQ_I2SPCM			(55)
#define BCM2836_IRQ_SDIO			(56)
#define BCM2836_IRQ_UART			(57)
#define BCM2836_IRQ_SLIMBUS			(58)
#define BCM2836_IRQ_VEC				(59)
#define BCM2836_IRQ_CPG				(60)
#define BCM2836_IRQ_RNG				(61)
#define BCM2836_IRQ_ARASANSDIO		(62)
#define BCM2836_IRQ_AVSPMON			(63)

/*
 * ARM interrupts
 */
#define BCM2836_IRQ_ARM_TIMER		(64)
#define BCM2836_IRQ_ARM_MAILBOX		(65)
#define BCM2836_IRQ_ARM_DOORBELL_0	(66)
#define BCM2836_IRQ_ARM_DOORBELL_1	(67)
#define BCM2836_IRQ_VPU0_HALTED		(68)
#define BCM2836_IRQ_VPU1_HALTED		(69)
#define BCM2836_IRQ_ILLEGAL_TYPE0	(70)
#define BCM2836_IRQ_ILLEGAL_TYPE1	(71)

/*
 * GPIO interrupts
 */
#define BCM2836_IRQ_GPIO0			(96)
#define BCM2836_IRQ_GPIO1			(97)
#define BCM2836_IRQ_GPIO2			(98)
#define BCM2836_IRQ_GPIO3			(99)
#define BCM2836_IRQ_GPIO4			(100)
#define BCM2836_IRQ_GPIO5			(101)
#define BCM2836_IRQ_GPIO6			(102)
#define BCM2836_IRQ_GPIO7			(103)
#define BCM2836_IRQ_GPIO8			(104)
#define BCM2836_IRQ_GPIO9			(105)
#define BCM2836_IRQ_GPIO10			(106)
#define BCM2836_IRQ_GPIO11			(107)
#define BCM2836_IRQ_GPIO12			(108)
#define BCM2836_IRQ_GPIO13			(109)
#define BCM2836_IRQ_GPIO14			(110)
#define BCM2836_IRQ_GPIO15			(111)
#define BCM2836_IRQ_GPIO16			(112)
#define BCM2836_IRQ_GPIO17			(113)
#define BCM2836_IRQ_GPIO18			(114)
#define BCM2836_IRQ_GPIO19			(115)
#define BCM2836_IRQ_GPIO20			(116)
#define BCM2836_IRQ_GPIO21			(117)
#define BCM2836_IRQ_GPIO22			(118)
#define BCM2836_IRQ_GPIO23			(119)
#define BCM2836_IRQ_GPIO24			(120)
#define BCM2836_IRQ_GPIO25			(121)
#define BCM2836_IRQ_GPIO26			(122)
#define BCM2836_IRQ_GPIO27			(123)
#define BCM2836_IRQ_GPIO28			(124)
#define BCM2836_IRQ_GPIO29			(125)
#define BCM2836_IRQ_GPIO30			(126)
#define BCM2836_IRQ_GPIO31			(127)

#define BCM2836_IRQ_GPIO32			(128)
#define BCM2836_IRQ_GPIO33			(129)
#define BCM2836_IRQ_GPIO34			(130)
#define BCM2836_IRQ_GPIO35			(131)
#define BCM2836_IRQ_GPIO36			(132)
#define BCM2836_IRQ_GPIO37			(133)
#define BCM2836_IRQ_GPIO38			(134)
#define BCM2836_IRQ_GPIO39			(135)
#define BCM2836_IRQ_GPIO40			(136)
#define BCM2836_IRQ_GPIO41			(137)
#define BCM2836_IRQ_GPIO42			(138)
#define BCM2836_IRQ_GPIO43			(139)
#define BCM2836_IRQ_GPIO44			(140)
#define BCM2836_IRQ_GPIO45			(141)
#define BCM2836_IRQ_GPIO46			(142)
#define BCM2836_IRQ_GPIO47			(143)
#define BCM2836_IRQ_GPIO48			(144)
#define BCM2836_IRQ_GPIO49			(145)
#define BCM2836_IRQ_GPIO50			(146)
#define BCM2836_IRQ_GPIO51			(147)
#define BCM2836_IRQ_GPIO52			(148)
#define BCM2836_IRQ_GPIO53			(149)

#ifdef __cplusplus
}
#endif

#endif /* __BCM2836_IRQ_H__ */
