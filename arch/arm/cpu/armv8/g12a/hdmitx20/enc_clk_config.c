
/*
 * arch/arm/cpu/armv8/txl/hdmitx20/enc_clk_config.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <amlogic/enc_clk_config.h>
#include <asm/arch/io.h>
#include <asm/io.h>
#include "hw_enc_clk_config.h"
#include "mach_reg.h"
#include <amlogic/hdmi.h>

#define msleep(i) udelay(i*1000)

#define check_clk_config(para)\
	if (para == -1)\
		return;

#define check_div() \
	if (div == -1)\
		return ;\
	switch (div) {\
	case 1:\
		div = 0; break;\
	case 2:\
		div = 1; break;\
	case 4:\
		div = 2; break;\
	case 6:\
		div = 3; break;\
	case 12:\
		div = 4; break;\
	default:\
		break;\
	}

#define WAIT_FOR_PLL_LOCKED(reg)                        \
	do {                                                \
		unsigned int cnt = 10;                          \
		unsigned int time_out = 0;                      \
		while (cnt --) {                                 \
		time_out = 0;                               \
		while ((!(hd_read_reg(reg) & (1 << 31)))\
			& (time_out < 10000))               \
			time_out ++;                            \
		}                                               \
		if (cnt < 9)                                     \
			printk("pll[0x%x] reset %d times\n", reg, 9 - cnt);\
	} while(0);

// viu_channel_sel: 1 or 2
// viu_type_sel: 0: 0=ENCL, 1=ENCI, 2=ENCP, 3=ENCT.
int set_viu_path(unsigned viu_channel_sel, enum viu_type viu_type_sel)
{
	if ((viu_channel_sel > 2) || (viu_channel_sel == 0))
		return -1;
	if (viu_channel_sel == 1)
		hd_set_reg_bits(P_VPU_VIU_VENC_MUX_CTRL, viu_type_sel, 0, 2);
	else
		//viu_channel_sel ==2
		hd_set_reg_bits(P_VPU_VIU_VENC_MUX_CTRL, viu_type_sel, 2, 2);
	return 0;
}

static void set_hdmitx_sys_clk(void)
{
	hd_set_reg_bits(P_HHI_HDMI_CLK_CNTL, 0, 9, 3);
	hd_set_reg_bits(P_HHI_HDMI_CLK_CNTL, 0, 0, 7);
	hd_set_reg_bits(P_HHI_HDMI_CLK_CNTL, 1, 8, 1);
}

static void set_hpll_clk_out(unsigned clk)
{
	check_clk_config(clk);
	printk("config HPLL = %d\n", clk);

	switch (clk) {
	case 5940000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b3a04f7);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00010000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x2a29dc00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x65771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 5405400:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004e1);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00007333);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 4455000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004b9);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00014000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3712500:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00049a);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00016000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3450000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00048f);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00018000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3243240:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b000487);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x0000451f);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 2970000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00047b);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00018000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 4324320:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004b4);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	default:
		printk("error hpll clk: %d\n", clk);
		break;
	}
	printk("config HPLL done\n");
}

static void set_hpll_od1(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 16, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 16, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 16, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

static void set_hpll_od2(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 18, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 18, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 18, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

static void set_hpll_od3(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 20, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 20, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 20, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

// --------------------------------------------------
//              clocks_set_vid_clk_div
// --------------------------------------------------
// wire            clk_final_en    = control[19];
// wire            clk_div1        = control[18];
// wire    [1:0]   clk_sel         = control[17:16];
// wire            set_preset      = control[15];
// wire    [14:0]  shift_preset    = control[14:0];
static void set_hpll_od3_clk_div(int div_sel)
{
	int shift_val = 0;
	int shift_sel = 0;

	/* When div 6.25, need to reset vid_pll_div */
	if (div_sel == CLK_UTIL_VID_PLL_DIV_6p25) {
		msleep(1);
		hd_set_reg_bits(P_RESET0_REGISTER, 1, 7, 1);
	}
	// Disable the output clock
	hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	switch (div_sel) {
	case CLK_UTIL_VID_PLL_DIV_1:      shift_val = 0xFFFF; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_2:      shift_val = 0x0aaa; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_3:      shift_val = 0x0db6; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_3p5:    shift_val = 0x36cc; shift_sel = 1; break;
	case CLK_UTIL_VID_PLL_DIV_3p75:   shift_val = 0x6666; shift_sel = 2; break;
	case CLK_UTIL_VID_PLL_DIV_4:      shift_val = 0x0ccc; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_5:      shift_val = 0x739c; shift_sel = 2; break;
	case CLK_UTIL_VID_PLL_DIV_6:      shift_val = 0x0e38; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_6p25:   shift_val = 0x0000; shift_sel = 3; break;
	case CLK_UTIL_VID_PLL_DIV_7:      shift_val = 0x3c78; shift_sel = 1; break;
	case CLK_UTIL_VID_PLL_DIV_7p5:    shift_val = 0x78f0; shift_sel = 2; break;
	case CLK_UTIL_VID_PLL_DIV_12:     shift_val = 0x0fc0; shift_sel = 0; break;
	case CLK_UTIL_VID_PLL_DIV_14:     shift_val = 0x3f80; shift_sel = 1; break;
	case CLK_UTIL_VID_PLL_DIV_15:     shift_val = 0x7f80; shift_sel = 2; break;
	case CLK_UTIL_VID_PLL_DIV_2p5:    shift_val = 0x5294; shift_sel = 2; break;
	default:
		printk("Error: clocks_set_vid_clk_div:  Invalid parameter\n");
		break;
	}

	if (shift_val == 0xffff ) {      // if divide by 1
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 1, 18, 1);
	} else {
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 18, 1);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 16, 2);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 15, 1);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 0, 14);

		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, shift_sel, 16, 2);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 1, 15, 1);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, shift_val, 0, 14);
		hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	}
	// Enable the final output clock
	hd_set_reg_bits(P_HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void set_vid_clk_div(unsigned div)
{
	check_clk_config(div);
	if (div == 0)
		div = 1;
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL, 0, 16, 3);   // select vid_pll_clk
	hd_set_reg_bits(P_HHI_VID_CLK_DIV, div-1, 0, 8);
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL, 7, 0, 3);
}

static void set_hdmi_tx_pixel_div(unsigned div)
{
	check_div();
	hd_set_reg_bits(P_HHI_HDMI_CLK_CNTL, div, 16, 4);
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL2, 1, 5, 1);   //enable gate
}

static void set_encp_div(unsigned div)
{
	check_div();
	hd_set_reg_bits(P_HHI_VID_CLK_DIV, div, 24, 4);
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL2, 1, 2, 1);   //enable gate
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL, 1, 19, 1);
}

static void set_enci_div(unsigned div)
{
	check_div();
	hd_set_reg_bits(P_HHI_VID_CLK_DIV, div, 28, 4);
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL2, 1, 0, 1);   //enable gate
	hd_set_reg_bits(P_HHI_VID_CLK_CNTL, 1, 19, 1);
}

/* mode hpll_clk_out od1 od2(PHY) od3
 * vid_pll_div vid_clk_div hdmi_tx_pixel_div encp_div enci_div
 */
/* For colordepth 8bits */
static struct hw_enc_clk_val_group setting_enc_clk_val_24[] = {
	{
		{
			HDMI_720x480i60_16x9, HDMI_720x576i50_16x9, GROUP_END
		},
		1, VIU_ENCI, 4324320, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, -1, 2
	},
	{
		{
			HDMI_720x576p50_16x9, HDMI_720x480p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 4324320, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	{
		{
			HDMI_720x576p100_16x9, HDMI_720x480p120_16x9, GROUP_END
		},
		1, VIU_ENCP, 4324320, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1280x720p50_16x9, HDMI_1280x720p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 2970000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i60_16x9, HDMI_1920x1080i50_16x9, GROUP_END
		},
		1, VIU_ENCP, 2970000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i100_16x9, HDMI_1920x1080i120_16x9,
			HDMI_1280x720p100_16x9, HDMI_1280x720p120_16x9,
			GROUP_END
		},
		1, VIU_ENCP, 2970000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080p60_16x9, HDMI_1920x1080p50_16x9, GROUP_END
		},
		1, VIU_ENCP, 2970000, 1, 2, 2, CLK_UTIL_VID_PLL_DIV_5, 1, 1, 1, -1
	},
	{
		{
			HDMI_1920x1080p30_16x9, HDMI_1920x1080p24_16x9,
			HDMI_1920x1080p25_16x9, GROUP_END
		},
		1, VIU_ENCP, 2970000, 2, 2, 2, CLK_UTIL_VID_PLL_DIV_5, 1, 1, 1, -1
	},
	{
		{
			HDMI_3840x2160p30_16x9, HDMI_3840x2160p25_16x9,
			HDMI_3840x2160p24_16x9, HDMI_4096x2160p24_256x135,
			HDMI_4096x2160p25_256x135, HDMI_4096x2160p30_256x135,
			HDMI_1920x1080p100_16x9, HDMI_1920x1080p120_16x9,
			GROUP_END
		},
		1, VIU_ENCP, 2970000, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_2160x1200p90hz, GROUP_END
		},
		1, VIU_ENCP, 5371100, 1, 2, 2, CLK_UTIL_VID_PLL_DIV_5, 1, 1, 1, -1
	},
	{
		{
			HDMI_3840x2160p60_16x9, HDMI_3840x2160p50_16x9,
			HDMI_4096x2160p60_256x135, HDMI_4096x2160p50_256x135,
			GROUP_END
		},
		1, VIU_ENCP, 5940000, 1, 1, 2, CLK_UTIL_VID_PLL_DIV_5, 1, 1, 1, -1
	},
	{
		{
			HDMI_4096x2160p60_256x135_Y420,
			HDMI_4096x2160p50_256x135_Y420,
			HDMI_3840x2160p60_16x9_Y420,
			HDMI_3840x2160p50_16x9_Y420,
			GROUP_END
		},
		1, VIU_ENCP, 5940000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 1, 2, 1, -1
	},
	/* pll setting for VESA modes */
	{
		{
			HDMIV_640x480p60hz, /* 4.028G / 16 = 251.75M */
			GROUP_END
		},
		1, VIU_ENCP, 4028000, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_800x480p60hz, GROUP_END
		},
		1, VIU_ENCP, 4761600, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_800x600p60hz, GROUP_END
		},
		1, VIU_ENCP, 3200000, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_852x480p60hz, HDMIV_854x480p60hz, GROUP_END
		},
		1, VIU_ENCP, 4838400, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1024x600p60hz, GROUP_END
		},
		1, VIU_ENCP, 4115866, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1024x768p60hz, GROUP_END
		},
		1, VIU_ENCP, 5200000, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1280x768p60hz, GROUP_END
		},
		1, VIU_ENCP, 3180000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1280x800p60hz,GROUP_END
		},
		1, VIU_ENCP, 5680000, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1152x864p75hz, HDMIV_1280x960p60hz,
			HDMIV_1280x1024p60hz, HDMIV_1600x900p60hz,
			GROUP_END
		},
		1, VIU_ENCP, 4320000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1600x1200p60hz, GROUP_END
		},
		1, VIU_ENCP, 3240000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1360x768p60hz, HDMIV_1366x768p60hz, GROUP_END
		},
		1, VIU_ENCP, 3420000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1400x1050p60hz, GROUP_END
		},
		1, VIU_ENCP, 4870000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1440x900p60hz, GROUP_END
		},
		1, VIU_ENCP, 4260000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1440x2560p60hz, GROUP_END
		},
		1, VIU_ENCP, 4897000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1440x2560p70hz, GROUP_END
		},
		1, VIU_ENCP, 5600000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1680x1050p60hz, GROUP_END
		},
		1, VIU_ENCP, 5850000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_1920x1200p60hz, GROUP_END
		},
		1, VIU_ENCP, 3865000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
	{
		{
			HDMIV_2560x1600p60hz, GROUP_END
		},
		1, VIU_ENCP, 3485000, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_5, 2, 1, 1, -1
	},
};

/* For colordepth 10bits */
static struct hw_enc_clk_val_group setting_enc_clk_val_30[] = {
	{
		{
			HDMI_720x480i60_16x9, HDMI_720x576i50_16x9, GROUP_END
		},
		1, VIU_ENCI, 5405400, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, -1, 2
	},
	{
		{
			HDMI_720x576p50_16x9, HDMI_720x480p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 5405400, 4, 4, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_720x576p100_16x9, HDMI_720x480p120_16x9, GROUP_END
		},
		1, VIU_ENCP, 5405400, 4, 2, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_1280x720p50_16x9, HDMI_1280x720p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i60_16x9, HDMI_1920x1080i50_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i120_16x9, HDMI_1920x1080i100_16x9,
			HDMI_1280x720p100_16x9, HDMI_1280x720p120_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080p60_16x9, HDMI_1920x1080p50_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 1, 2, 2, CLK_UTIL_VID_PLL_DIV_6p25, 1, 1, 1, -1
	},
	{
		{
			HDMI_1920x1080p120_16x9, HDMI_1920x1080p100_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 2, -1
	},
	{
		{
			HDMI_1920x1080p30_16x9, HDMI_1920x1080p24_16x9,
			HDMI_1920x1080p25_16x9, GROUP_END
		},
		1, VIU_ENCP, 3712500, 2, 2, 2, CLK_UTIL_VID_PLL_DIV_6p25, 1, 1, 1, -1
	},
	{
		{
			HDMI_4096x2160p60_256x135_Y420, HDMI_4096x2160p50_256x135_Y420,
			HDMI_3840x2160p60_16x9_Y420, HDMI_3840x2160p50_16x9_Y420, GROUP_END
		},
		1, VIU_ENCP, 3712500, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 1, -1
	},
	{
		{
			HDMI_3840x2160p24_16x9, HDMI_3840x2160p25_16x9,
			HDMI_3840x2160p30_16x9, HDMI_4096x2160p24_256x135,
			HDMI_4096x2160p25_256x135, HDMI_4096x2160p30_256x135, GROUP_END
		},
		1, VIU_ENCP, 3712500, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_6p25, 1, 2, 2, -1
	},
};


/* For colordepth 12bits */
static struct hw_enc_clk_val_group setting_enc_clk_val_36[] = {
	{
		{
			HDMI_720x480i60_16x9, HDMI_720x576i50_16x9, GROUP_END
		},
		1, VIU_ENCI, 3243240, 2, 4, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, -1, 2
	},
	{
		{
			HDMI_720x576p50_16x9, HDMI_720x480p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 3243240, 2, 4, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_720x576p100_16x9, HDMI_720x480p120_16x9, GROUP_END
		},
		1, VIU_ENCP, 3243240, 2, 2, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1280x720p50_16x9, HDMI_1280x720p60_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i60_16x9, HDMI_1920x1080i50_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 4, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080i120_16x9, HDMI_1920x1080i100_16x9,
			HDMI_1280x720p100_16x9, HDMI_1280x720p120_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 2, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_1920x1080p60_16x9, HDMI_1920x1080p50_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 1, 2, 2, CLK_UTIL_VID_PLL_DIV_7p5, 1, 1, 1, -1
	},
	{
		{
			HDMI_1920x1080p120_16x9, HDMI_1920x1080p100_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 2, -1
	},
	{
		{
			HDMI_1920x1080p30_16x9, HDMI_1920x1080p24_16x9,
			HDMI_1920x1080p25_16x9, GROUP_END
		},
		1, VIU_ENCP, 4455000, 2, 2, 2, CLK_UTIL_VID_PLL_DIV_7p5, 1, 1, 1, -1
	},
	{
		{
			HDMI_4096x2160p60_256x135_Y420,
			HDMI_4096x2160p50_256x135_Y420,
			HDMI_3840x2160p60_16x9_Y420,
			HDMI_3840x2160p50_16x9_Y420,
			GROUP_END
		},
		1, VIU_ENCP, 4455000, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 1, -1
	},
	{
		{
			HDMI_3840x2160p24_16x9,
			HDMI_3840x2160p25_16x9,
			HDMI_3840x2160p30_16x9,
			HDMI_4096x2160p24_256x135,
			HDMI_4096x2160p25_256x135,
			HDMI_4096x2160p30_256x135,
			GROUP_END
		},
		1, VIU_ENCP, 4455000, 1, 1, 1, CLK_UTIL_VID_PLL_DIV_7p5, 1, 2, 2, -1
	},
};

void hdmitx_set_clk_(struct hdmitx_dev *hdev)
{
	int i = 0;
	int j = 0;
	struct hw_enc_clk_val_group *p_enc =NULL;
	enum hdmi_vic vic = hdev->vic;
	enum hdmi_color_format cs = hdev->para->cs;
	enum hdmi_color_depth cd = hdev->para->cd;

	/* YUV 422 always use 24B mode */
	if (cs == HDMI_COLOR_FORMAT_422)
		cd = HDMI_COLOR_DEPTH_24B;

	if (cd == HDMI_COLOR_DEPTH_24B) {
		p_enc = &setting_enc_clk_val_24[0];
		for (j = 0; j < sizeof(setting_enc_clk_val_24)
			/ sizeof(struct hw_enc_clk_val_group); j++) {
			for (i = 0; ((i < GROUP_MAX) && (p_enc[j].group[i]
				!= GROUP_END)); i++) {
				if (vic == p_enc[j].group[i])
					goto next;
			}
		}
		if (j == sizeof(setting_enc_clk_val_24)
			/ sizeof(struct hw_enc_clk_val_group)) {
			pr_info("Not find VIC = %d for hpll setting\n", vic);
			return;
		}
	} else if (cd == HDMI_COLOR_DEPTH_30B) {
		p_enc = &setting_enc_clk_val_30[0];
		for (j = 0; j < sizeof(setting_enc_clk_val_30)
			/ sizeof(struct hw_enc_clk_val_group); j++) {
			for (i = 0; ((i < GROUP_MAX) && (p_enc[j].group[i]
				!= GROUP_END)); i++) {
				if (vic == p_enc[j].group[i])
					goto next;
			}
		}
		if (j == sizeof(setting_enc_clk_val_30) /
			sizeof(struct hw_enc_clk_val_group)) {
			pr_info("Not find VIC = %d for hpll setting\n", vic);
			return;
		}
	} else if (cd == HDMI_COLOR_DEPTH_36B) {
		p_enc = &setting_enc_clk_val_36[0];
		for (j = 0; j < sizeof(setting_enc_clk_val_36)
			/ sizeof(struct hw_enc_clk_val_group); j++) {
			for (i = 0; ((i < GROUP_MAX) && (p_enc[j].group[i]
				!= GROUP_END)); i++) {
				if (vic == p_enc[j].group[i])
					goto next;
			}
		}
		if (j == sizeof(setting_enc_clk_val_36) /
			sizeof(struct hw_enc_clk_val_group)) {
			pr_info("Not find VIC = %d for hpll setting\n", vic);
			return;
		}
	} else {
		pr_info("not support colordepth 48bits\n");
		return;
	}

next:
	set_viu_path(p_enc[j].viu_path, p_enc[j].viu_type);
	set_hdmitx_sys_clk();
	set_hpll_clk_out(p_enc[j].hpll_clk_out);
	set_hpll_od1(p_enc[j].od1);
	set_hpll_od2(p_enc[j].od2);
	set_hpll_od3(p_enc[j].od3);
	set_hpll_od3_clk_div(p_enc[j].vid_pll_div);
	printk("j = %d  vid_clk_div = %d\n", j, p_enc[j].vid_clk_div);
	set_vid_clk_div(p_enc[j].vid_clk_div);
	set_hdmi_tx_pixel_div(p_enc[j].hdmi_tx_pixel_div);
	set_encp_div(p_enc[j].encp_div);
	set_enci_div(p_enc[j].enci_div);
}

void hdmitx_set_clk(struct hdmitx_dev *hdev)
{
	hdmitx_set_clk_(hdev);
}
