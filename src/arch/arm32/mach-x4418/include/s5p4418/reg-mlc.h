#ifndef __S5P4418_REG_MLC_H__
#define __S5P4418_REG_MLC_H__

#define S5P4418_MLC0_BASE			(0xC0102000)
#define S5P4418_MLC1_BASE			(0xC0102400)

#define MLC_CONTROLT				(0x000)
#define MLC_SCREENSIZE				(0x004)
#define MLC_BGCOLOR					(0x008)
#define MLC_LEFTRIGHT0				(0x00c)
#define MLC_TOPBOTTOM0				(0x010)
#define MLC_INVALIDLEFTRIGHT0_0		(0x014)
#define MLC_INVALIDTOPBOTTOM0_0		(0x018)
#define MLC_INVALIDLEFTRIGHT0_1		(0x01c)
#define MLC_INVALIDTOPBOTTOM0_1		(0x020)
#define MLC_CONTROL0				(0x024)
#define MLC_HSTRIDE0				(0x028)
#define MLC_VSTRIDE0				(0x02c)
#define MLC_TPCOLOR0				(0x030)
#define MLC_INVCOLOR0				(0x034)
#define MLC_ADDRESS0				(0x038)

#define MLC_LEFTRIGHT2				(0x074)
#define MLC_TOPBOTTOM2				(0x078)
#define MLC_CONTROL2				(0x07c)
#define MLC_VSTRIDE2				(0x080)
#define MLC_TPCOLOR2				(0x084)
#define MLC_INVCOLOR2				(0x088)
#define MLC_ADDRESS2				(0x08c)
#define MLC_ADDRESSCB				(0x090)
#define MLC_ADDRESSCR				(0x094)
#define MLC_VSTRIDECB				(0x098)
#define MLC_VSTRIDECR				(0x09c)
#define MLC_HSCALE					(0x0a0)
#define MLC_VSCALE					(0x0a4)
#define MLC_LUENH					(0x0a8)
#define MLC_CHENH0					(0x0ac)
#define MLC_CHENH1					(0x0b0)
#define MLC_CHENH2					(0x0b4)
#define MLC_CHENH3					(0x0b8)
#define MLC_CLKENB					(0x3c0)

/* Just for primary */
#define MLC_LEFTRIGHT1				(0x040)
#define MLC_TOPBOTTOM1				(0x044)
#define MLC_INVALIDLEFTRIGHT1_0		(0x048)
#define MLC_INVALIDTOPBOTTOM1_0		(0x04c)
#define MLC_INVALIDLEFTRIGHT1_1		(0x050)
#define MLC_INVALIDTOPBOTTOM1_1		(0x054)
#define MLC_CONTROL1				(0x058)
#define MLC_HSTRIDE1				(0x05c)
#define MLC_VSTRIDE1				(0x060)
#define MLC_TPCOLOR1				(0x064)
#define MLC_INVCOLOR1				(0x068)
#define MLC_ADDRESS1				(0x06c)

#endif /* __S5P4418_REG_MLC_H__ */
