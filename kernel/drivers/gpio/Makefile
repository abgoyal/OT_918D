# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


# generic gpio support: dedicated expander chips, etc
#
# NOTE: platform-specific GPIO drivers don't belong in the
# drivers/gpio directory; put them with other platform setup
# code, IRQ controllers, board init, etc.

ccflags-$(CONFIG_DEBUG_GPIO)	+= -DDEBUG

obj-$(CONFIG_GPIOLIB)		+= gpiolib.o

obj-$(CONFIG_GPIO_ADP5520)	+= adp5520-gpio.o
obj-$(CONFIG_GPIO_ADP5588)	+= adp5588-gpio.o
obj-$(CONFIG_GPIO_LANGWELL)	+= langwell_gpio.o
obj-$(CONFIG_GPIO_MAX730X)	+= max730x.o
obj-$(CONFIG_GPIO_MAX7300)	+= max7300.o
obj-$(CONFIG_GPIO_MAX7301)	+= max7301.o
obj-$(CONFIG_GPIO_MAX732X)	+= max732x.o
obj-$(CONFIG_GPIO_MC33880)	+= mc33880.o
obj-$(CONFIG_GPIO_MCP23S08)	+= mcp23s08.o
obj-$(CONFIG_GPIO_PCA953X)	+= pca953x.o
obj-$(CONFIG_GPIO_PCF857X)	+= pcf857x.o
obj-$(CONFIG_GPIO_PL061)	+= pl061.o
obj-$(CONFIG_GPIO_TC35892)	+= tc35892-gpio.o
obj-$(CONFIG_GPIO_TIMBERDALE)	+= timbgpio.o
obj-$(CONFIG_GPIO_TWL4030)	+= twl4030-gpio.o
obj-$(CONFIG_GPIO_UCB1400)	+= ucb1400_gpio.o
obj-$(CONFIG_GPIO_XILINX)	+= xilinx_gpio.o
obj-$(CONFIG_GPIO_CS5535)	+= cs5535-gpio.o
obj-$(CONFIG_GPIO_BT8XX)	+= bt8xxgpio.o
obj-$(CONFIG_GPIO_IT8761E)	+= it8761e_gpio.o
obj-$(CONFIG_GPIO_VR41XX)	+= vr41xx_giu.o
obj-$(CONFIG_GPIO_WM831X)	+= wm831x-gpio.o
obj-$(CONFIG_GPIO_WM8350)	+= wm8350-gpiolib.o
obj-$(CONFIG_GPIO_WM8994)	+= wm8994-gpio.o
obj-$(CONFIG_GPIO_SCH)		+= sch_gpio.o
obj-$(CONFIG_GPIO_RDC321X)	+= rdc321x-gpio.o
obj-$(CONFIG_GPIO_JANZ_TTL)	+= janz-ttl.o
