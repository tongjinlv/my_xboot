#ifndef __RK3288_REG_VOP_H__
#define __RK3288_REG_VOP_H__

#define RK3288_VOP_BIG_BASE			(0xff930000)
#define RK3288_VOP_LIT_BASE			(0xff940000)

#define VOP_REG_CFG_DONE			(0x0000)
#define VOP_VERSION_INFO			(0x0004)
#define VOP_SYS_CTRL				(0x0008)
#define VOP_SYS_CTRL1				(0x000c)
#define VOP_DSP_CTRL0				(0x0010)
#define VOP_DSP_CTRL1				(0x0014)
#define VOP_DSP_BG					(0x0018)
#define VOP_MCU_CTRL				(0x001c)
#define VOP_INTR_CTRL0				(0x0020)
#define VOP_INTR_CTRL1				(0x0024)
#define VOP_INTR_RESERVED0			(0x0028)
#define VOP_INTR_RESERVED1			(0x002c)

#define VOP_WIN0_CTRL0				(0x0030)
#define VOP_WIN0_CTRL1				(0x0034)
#define VOP_WIN0_COLOR_KEY			(0x0038)
#define VOP_WIN0_VIR				(0x003c)
#define VOP_WIN0_YRGB_MST			(0x0040)
#define VOP_WIN0_CBR_MST			(0x0044)
#define VOP_WIN0_ACT_INFO			(0x0048)
#define VOP_WIN0_DSP_INFO			(0x004c)
#define VOP_WIN0_DSP_ST				(0x0050)
#define VOP_WIN0_SCL_FACTOR_YRGB	(0x0054)
#define VOP_WIN0_SCL_FACTOR_CBR		(0x0058)
#define VOP_WIN0_SCL_OFFSET			(0x005c)
#define VOP_WIN0_SRC_ALPHA_CTRL		(0x0060)
#define VOP_WIN0_DST_ALPHA_CTRL		(0x0064)
#define VOP_WIN0_FADING_CTRL		(0x0068)
#define VOP_WIN0_RESERVED0			(0x006c)

#define VOP_WIN1_CTRL0				(0x0070)
#define VOP_WIN1_CTRL1				(0x0074)
#define VOP_WIN1_COLOR_KEY			(0x0078)
#define VOP_WIN1_VIR				(0x007c)
#define VOP_WIN1_YRGB_MST			(0x0080)
#define VOP_WIN1_CBR_MST			(0x0084)
#define VOP_WIN1_ACT_INFO			(0x0088)
#define VOP_WIN1_DSP_INFO			(0x008c)
#define VOP_WIN1_DSP_ST				(0x0090)
#define VOP_WIN1_SCL_FACTOR_YRGB	(0x0094)
#define VOP_WIN1_SCL_FACTOR_CBR		(0x0098)
#define VOP_WIN1_SCL_OFFSET			(0x009c)
#define VOP_WIN1_SRC_ALPHA_CTRL		(0x00a0)
#define VOP_WIN1_DST_ALPHA_CTRL		(0x00a4)
#define VOP_WIN1_FADING_CTRL		(0x00a8)
#define VOP_WIN1_RESERVED0			(0x00ac)

#define VOP_WIN2_CTRL0				(0x00b0)
#define VOP_WIN2_CTRL1				(0x00b4)
#define VOP_WIN2_VIR0_1				(0x00b8)
#define VOP_WIN2_VIR2_3				(0x00bc)
#define VOP_WIN2_MST0				(0x00c0)
#define VOP_WIN2_DSP_INFO0			(0x00c4)
#define VOP_WIN2_DSP_ST0			(0x00c8)
#define VOP_WIN2_COLOR_KEY			(0x00cc)
#define VOP_WIN2_MST1				(0x00d0)
#define VOP_WIN2_DSP_INFO1			(0x00d4)
#define VOP_WIN2_DSP_ST1			(0x00d8)
#define VOP_WIN2_SRC_ALPHA_CTRL		(0x00dc)
#define VOP_WIN2_MST2				(0x00e0)
#define VOP_WIN2_DSP_INFO2			(0x00e4)
#define VOP_WIN2_DSP_ST2			(0x00e8)
#define VOP_WIN2_DST_ALPHA_CTRL		(0x00ec)
#define VOP_WIN2_MST3				(0x00f0)
#define VOP_WIN2_DSP_ST3			(0x00f8)
#define VOP_WIN2_FADING_CTRL		(0x00fc)
#define VOP_WIN2_DSP_INFO3			(0x0100)

#define VOP_WIN3_CTRL1				(0x0104)
#define VOP_WIN3_VIR0_1				(0x0108)
#define VOP_WIN3_VIR2_3				(0x010c)
#define VOP_WIN3_MST0				(0x0110)
#define VOP_WIN3_DSP_INFO0			(0x0114)
#define VOP_WIN3_DSP_ST0			(0x0118)
#define VOP_WIN3_COLOR_KEY			(0x011c)
#define VOP_WIN3_MST1				(0x0120)
#define VOP_WIN3_DSP_INFO1			(0x0124)
#define VOP_WIN3_DSP_ST1			(0x0128)
#define VOP_WIN3_SRC_ALPHA_CTRL		(0x012c)
#define VOP_WIN3_MST2				(0x0130)
#define VOP_WIN3_DSP_INFO2			(0x0134)
#define VOP_WIN3_DSP_ST2			(0x0138)
#define VOP_WIN3_DST_ALPHA_CTRL		(0x013c)
#define VOP_WIN3_MST3				(0x0140)
#define VOP_WIN3_DSP_INFO3			(0x0144)
#define VOP_WIN3_DSP_ST3			(0x0148)
#define VOP_WIN3_FADING_CTRL		(0x014c)

#define VOP_HWC_CTRL0				(0x0150)
#define VOP_HWC_CTRL1				(0x0154)
#define VOP_HWC_MST					(0x0158)
#define VOP_HWC_DSP_ST				(0x015c)
#define VOP_HWC_SRC_ALPHA_CTRL		(0x0160)
#define VOP_HWC_DST_ALPHA_CTRL		(0x0164)
#define VOP_HWC_FADING_CTRL			(0x0168)
#define VOP_HWC_RESERVED1			(0x016c)
#define VOP_POST_DSP_HACT_INFO		(0x0170)
#define VOP_POST_DSP_VACT_INFO		(0x0174)
#define VOP_POST_SCL_FACTOR_YRGB	(0x0178)
#define VOP_POST_RESERVED			(0x017c)
#define VOP_POST_SCL_CTRL			(0x0180)
#define VOP_POST_DSP_VACT_INFO_F1	(0x0184)
#define VOP_DSP_HTOTAL_HS_END		(0x0188)
#define VOP_DSP_HACT_ST_END			(0x018c)
#define VOP_DSP_VTOTAL_VS_END		(0x0190)
#define VOP_DSP_VACT_ST_END			(0x0194)
#define VOP_DSP_VS_ST_END_F1		(0x0198)
#define VOP_POST_DSP_VACT_ST_END_F1	(0x019c)

#define VOP_PWM_CTRL				(0x01a0)
#define VOP_PWM_PERIOD_HPR			(0x01a4)
#define VOP_PWM_DUTY_LPR			(0x01a8)
#define VOP_PWM_CNT					(0x01ac)

#define VOP_BCSH_COLOR_BAR			(0x01b0)
#define VOP_BCSH_BCS				(0x01b4)
#define VOP_BCSH_H					(0x01b8)
#define VOP_BCSH_RESERVED			(0x01bc)
#define VOP_CABC_CTRL0				(0x01c0)
#define VOP_CABC_CTRL1				(0x01c4)
#define VOP_CABC_GAUSS_LINE0_0		(0x01c8)
#define VOP_CABC_GAUSS_LINE0_1		(0x01cC)
#define VOP_CABC_GAUSS_LINE1_0		(0x01d0)
#define VOP_CABC_GAUSS_LINE1_1		(0x01d4)
#define VOP_CABC_GAUSS_LINE2_0		(0x01d8)
#define VOP_CABC_GAUSS_LINE2_1		(0x01dc)
#define VOP_FRC_LOWER01_0			(0x01e0)
#define VOP_FRC_LOWER01_1			(0x01e4)
#define VOP_FRC_LOWER10_0			(0x01e8)
#define VOP_FRC_LOWER10_1			(0x01ec)
#define VOP_FRC_LOWER11_0			(0x01f0)
#define VOP_FRC_LOWER11_1			(0x01f4)
#define VOP_FRC_RESERVED0			(0x01f8)
#define VOP_FRC_RESERVED1			(0x01fc)
#define VOP_MMU_DTE_ADDR			(0x0300)
#define VOP_MMU_STATUS				(0x0304)
#define VOP_MMU_COMMAND				(0x0308)
#define VOP_MMU_PAGE_FAULT_ADDR		(0x030c)
#define VOP_MMU_ZAP_ONE_LINE		(0x0310)
#define VOP_MMU_INT_RAWSTAT			(0x0314)
#define VOP_MMU_INT_CLEAR			(0x0318)
#define VOP_MMU_INT_MASK			(0x031c)
#define VOP_MMU_INT_STATUS			(0x0320)
#define VOP_MMU_AUTO_GATING			(0x0324)
#define VOP_WIN2_LUT_ADDR			(0x0400)
#define VOP_WIN3_LUT_ADDR			(0x0800)
#define VOP_HWC_LUT_ADDR			(0x0c00)
#define VOP_GAMMA_LUT_ADDR			(0x1000)
#define VOP_MCU_BYPASS_WPO			(0x2200)
#define VOP_MCU_BYPASS_RPO			(0x2300)

#endif /* __RK3288_REG_VOP_H__ */
