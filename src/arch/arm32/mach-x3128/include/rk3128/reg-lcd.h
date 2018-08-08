#ifndef __RK3128_REG_LCD_H__
#define __RK3128_REG_LCD_H__

#define RK3288_LCD_BASE				(0x1010e000)

#define LCD_SYS_CTRL                (0x00)
#define LCD_DSP_CTRL0				(0x04)
#define LCD_DSP_CTRL1				(0x08)
#define LCD_INT_SCALER              (0x0c)
#define LCD_INT_STATUS				(0x10)
#define LCD_ALPHA_CTRL				(0x14)
#define LCD_WIN0_COLOR_KEY			(0x18)
#define LCD_WIN1_COLOR_KEY			(0x1c)
#define LCD_WIN0_YRGB_MST			(0x20)
#define LCD_WIN0_CBR_MST			(0x24)
#define LCD_WIN1_VIR				(0x28)
#define LCD_WIN0_VIR				(0x30)
#define LCD_WIN0_ACT_INFO			(0x34)
#define LCD_WIN0_DSP_INFO			(0x38)
#define LCD_WIN0_DSP_ST				(0x3c)
#define LCD_WIN0_SCL_FACTOR_YRGB	(0x40)
#define LCD_WIN0_SCL_FACTOR_CBR		(0x44)
#define LCD_WIN0_SCL_OFFSET			(0x48)
#define LCD_WIN1_MST_RK312X         (0x4c)
#define LCD_WIN1_DSP_INFO_RK312X    (0x50)
#define LCD_WIN1_DSP_ST_RK312X      (0x54)
#define LCD_HWC_MST					(0x58)
#define LCD_HWC_DSP_ST				(0x5c)

#define LCD_DSP_HTOTAL_HS_END		(0x6c)
#define LCD_DSP_HACT_ST_END			(0x70)
#define LCD_DSP_VTOTAL_VS_END		(0x74)
#define LCD_DSP_VACT_ST_END			(0x78)
#define LCD_DSP_VS_ST_END_F1		(0x7c)
#define LCD_DSP_VACT_ST_END_F1		(0x80)

#define LCD_SCALER_CTRL             (0xa0)
#define LCD_SCALER_FACTOR           (0xa4)
#define LCD_SCALER_FRAME_ST         (0xa8)
#define LCD_SCALER_DSP_HOR_TIMING   (0xac)
#define LCD_SCALER_DSP_HACT_ST_END  (0xb0)
#define LCD_SCALER_DSP_VER_TIMING   (0xb4)
#define LCD_SCALER_DSP_VACT_ST_END  (0xb8)
#define LCD_SCALER_DSP_HBOR_TIMING  (0xbc)
#define LCD_SCALER_DSP_VBOR_TIMING  (0xc0)

#define LCD_BCSH_CTRL				(0xd0)
#define LCD_BCSH_COLOR_BAR 			(0xd4)
#define LCD_BCSH_BCS 				(0xd8)
#define LCD_BCSH_H 					(0xdc)
#define LCD_FRC_LOWER01_0           (0xe0)
#define LCD_FRC_LOWER01_1           (0xe4)
#define LCD_FRC_LOWER10_0           (0xe8)
#define LCD_FRC_LOWER10_1           (0xec)
#define LCD_FRC_LOWER11_0           (0xf0)
#define LCD_FRC_LOWER11_1           (0xf4)
#define LCD_AXI_BUS_CTRL			(0x2c)
#define LCD_GATHER_TRANSFER			(0x84)
#define LCD_VERSION_INFO			(0x94)
#define LCD_REG_CFG_DONE			(0x90)

#define LCD_TV_CTRL					(0x200)
#define LCD_TV_SYNC_TIMING			(0x204)
#define LCD_TV_ACT_TIMING			(0x208)
#define LCD_TV_ADJ_TIMING			(0x20c)
#define LCD_TV_FREQ_SC				(0x210)
#define LCD_TV_FILTER0				(0x214)
#define LCD_TV_FILTER1				(0x218)
#define LCD_TV_FILTER2				(0x21C)
#define LCD_TV_ACT_ST				(0x234)
#define LCD_TV_ROUTING				(0x238)
#define LCD_TV_SYNC_ADJUST			(0x250)
#define LCD_TV_STATUS				(0x254)
#define LCD_TV_RESET				(0x268)
#define LCD_TV_SATURATION			(0x278)
#define LCD_TV_BW_CTRL				(0x28C)
#define LCD_TV_BRIGHTNESS_CONTRAST	(0x290)

#define LCD_MMU_DTE_ADDR			(0x300)
#define LCD_MMU_STATUS				(0x304)
#define LCD_MMU_COMMAND				(0x308)
#define LCD_MMU_PAGE_FAULT_ADDR		(0x30c)
#define LCD_MMU_ZAP_ONE_LINE		(0x310)
#define LCD_MMU_INT_RAWSTAT			(0x314)
#define LCD_MMU_INT_CLEAR			(0x318)
#define LCD_MMU_INT_MASK			(0x31c)
#define LCD_MMU_INT_STATUS			(0x320)
#define LCD_MMU_AUTO_GATING			(0x324)

#define LCD_HWC_LUT_ADDR   			(0x800)
#define LCD_DSP_LUT_ADDR            (0xc00)

#endif /* __RK3128_REG_LCD_H__ */
