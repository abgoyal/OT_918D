/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *  linux/arch/arm/mach-integrator/integrator_ap.c
 *
 *  Copyright (C) 2000-2003 Deep Blue Solutions Ltd
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysdev.h>
#include <linux/amba/bus.h>
#include <linux/amba/kmi.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/platform.h>
#include <asm/hardware/arm_timer.h>
#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/param.h>		/* HZ */
#include <asm/mach-types.h>

#include <mach/lm.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>

/* 
 * All IO addresses are mapped onto VA 0xFFFx.xxxx, where x.xxxx
 * is the (PA >> 12).
 *
 * Setup a VA for the Integrator interrupt controller (for header #0,
 * just for now).
 */
#define VA_IC_BASE	IO_ADDRESS(INTEGRATOR_IC_BASE) 
#define VA_SC_BASE	IO_ADDRESS(INTEGRATOR_SC_BASE)
#define VA_EBI_BASE	IO_ADDRESS(INTEGRATOR_EBI_BASE)
#define VA_CMIC_BASE	IO_ADDRESS(INTEGRATOR_HDR_IC)

/*
 * Logical      Physical
 * e8000000	40000000	PCI memory		PHYS_PCI_MEM_BASE	(max 512M)
 * ec000000	61000000	PCI config space	PHYS_PCI_CONFIG_BASE	(max 16M)
 * ed000000	62000000	PCI V3 regs		PHYS_PCI_V3_BASE	(max 64k)
 * ee000000	60000000	PCI IO			PHYS_PCI_IO_BASE	(max 16M)
 * ef000000			Cache flush
 * f1000000	10000000	Core module registers
 * f1100000	11000000	System controller registers
 * f1200000	12000000	EBI registers
 * f1300000	13000000	Counter/Timer
 * f1400000	14000000	Interrupt controller
 * f1600000	16000000	UART 0
 * f1700000	17000000	UART 1
 * f1a00000	1a000000	Debug LEDs
 * f1b00000	1b000000	GPIO
 */

static struct map_desc ap_io_desc[] __initdata = {
	{
		.virtual	= IO_ADDRESS(INTEGRATOR_HDR_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_HDR_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_SC_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_SC_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_EBI_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_EBI_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_CT_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_CT_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_IC_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_IC_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_UART0_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_UART0_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_UART1_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_UART1_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_DBG_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_DBG_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= IO_ADDRESS(INTEGRATOR_AP_GPIO_BASE),
		.pfn		= __phys_to_pfn(INTEGRATOR_AP_GPIO_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE
	}, {
		.virtual	= PCI_MEMORY_VADDR,
		.pfn		= __phys_to_pfn(PHYS_PCI_MEM_BASE),
		.length		= SZ_16M,
		.type		= MT_DEVICE
	}, {
		.virtual	= PCI_CONFIG_VADDR,
		.pfn		= __phys_to_pfn(PHYS_PCI_CONFIG_BASE),
		.length		= SZ_16M,
		.type		= MT_DEVICE
	}, {
		.virtual	= PCI_V3_VADDR,
		.pfn		= __phys_to_pfn(PHYS_PCI_V3_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE
	}, {
		.virtual	= PCI_IO_VADDR,
		.pfn		= __phys_to_pfn(PHYS_PCI_IO_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE
	}
};

static void __init ap_map_io(void)
{
	iotable_init(ap_io_desc, ARRAY_SIZE(ap_io_desc));
}

#define INTEGRATOR_SC_VALID_INT	0x003fffff

static void sc_mask_irq(unsigned int irq)
{
	writel(1 << irq, VA_IC_BASE + IRQ_ENABLE_CLEAR);
}

static void sc_unmask_irq(unsigned int irq)
{
	writel(1 << irq, VA_IC_BASE + IRQ_ENABLE_SET);
}

static struct irq_chip sc_chip = {
	.name	= "SC",
	.ack	= sc_mask_irq,
	.mask	= sc_mask_irq,
	.unmask = sc_unmask_irq,
};

static void __init ap_init_irq(void)
{
	unsigned int i;

	/* Disable all interrupts initially. */
	/* Do the core module ones */
	writel(-1, VA_CMIC_BASE + IRQ_ENABLE_CLEAR);

	/* do the header card stuff next */
	writel(-1, VA_IC_BASE + IRQ_ENABLE_CLEAR);
	writel(-1, VA_IC_BASE + FIQ_ENABLE_CLEAR);

	for (i = 0; i < NR_IRQS; i++) {
		if (((1 << i) & INTEGRATOR_SC_VALID_INT) != 0) {
			set_irq_chip(i, &sc_chip);
			set_irq_handler(i, handle_level_irq);
			set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
		}
	}
}

#ifdef CONFIG_PM
static unsigned long ic_irq_enable;

static int irq_suspend(struct sys_device *dev, pm_message_t state)
{
	ic_irq_enable = readl(VA_IC_BASE + IRQ_ENABLE);
	return 0;
}

static int irq_resume(struct sys_device *dev)
{
	/* disable all irq sources */
	writel(-1, VA_CMIC_BASE + IRQ_ENABLE_CLEAR);
	writel(-1, VA_IC_BASE + IRQ_ENABLE_CLEAR);
	writel(-1, VA_IC_BASE + FIQ_ENABLE_CLEAR);

	writel(ic_irq_enable, VA_IC_BASE + IRQ_ENABLE_SET);
	return 0;
}
#else
#define irq_suspend NULL
#define irq_resume NULL
#endif

static struct sysdev_class irq_class = {
	.name		= "irq",
	.suspend	= irq_suspend,
	.resume		= irq_resume,
};

static struct sys_device irq_device = {
	.id	= 0,
	.cls	= &irq_class,
};

static int __init irq_init_sysfs(void)
{
	int ret = sysdev_class_register(&irq_class);
	if (ret == 0)
		ret = sysdev_register(&irq_device);
	return ret;
}

device_initcall(irq_init_sysfs);

/*
 * Flash handling.
 */
#define SC_CTRLC (VA_SC_BASE + INTEGRATOR_SC_CTRLC_OFFSET)
#define SC_CTRLS (VA_SC_BASE + INTEGRATOR_SC_CTRLS_OFFSET)
#define EBI_CSR1 (VA_EBI_BASE + INTEGRATOR_EBI_CSR1_OFFSET)
#define EBI_LOCK (VA_EBI_BASE + INTEGRATOR_EBI_LOCK_OFFSET)

static int ap_flash_init(void)
{
	u32 tmp;

	writel(INTEGRATOR_SC_CTRL_nFLVPPEN | INTEGRATOR_SC_CTRL_nFLWP, SC_CTRLC);

	tmp = readl(EBI_CSR1) | INTEGRATOR_EBI_WRITE_ENABLE;
	writel(tmp, EBI_CSR1);

	if (!(readl(EBI_CSR1) & INTEGRATOR_EBI_WRITE_ENABLE)) {
		writel(0xa05f, EBI_LOCK);
		writel(tmp, EBI_CSR1);
		writel(0, EBI_LOCK);
	}
	return 0;
}

static void ap_flash_exit(void)
{
	u32 tmp;

	writel(INTEGRATOR_SC_CTRL_nFLVPPEN | INTEGRATOR_SC_CTRL_nFLWP, SC_CTRLC);

	tmp = readl(EBI_CSR1) & ~INTEGRATOR_EBI_WRITE_ENABLE;
	writel(tmp, EBI_CSR1);

	if (readl(EBI_CSR1) & INTEGRATOR_EBI_WRITE_ENABLE) {
		writel(0xa05f, EBI_LOCK);
		writel(tmp, EBI_CSR1);
		writel(0, EBI_LOCK);
	}
}

static void ap_flash_set_vpp(int on)
{
	unsigned long reg = on ? SC_CTRLS : SC_CTRLC;

	writel(INTEGRATOR_SC_CTRL_nFLVPPEN, reg);
}

static struct flash_platform_data ap_flash_data = {
	.map_name	= "cfi_probe",
	.width		= 4,
	.init		= ap_flash_init,
	.exit		= ap_flash_exit,
	.set_vpp	= ap_flash_set_vpp,
};

static struct resource cfi_flash_resource = {
	.start		= INTEGRATOR_FLASH_BASE,
	.end		= INTEGRATOR_FLASH_BASE + INTEGRATOR_FLASH_SIZE - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device cfi_flash_device = {
	.name		= "armflash",
	.id		= 0,
	.dev		= {
		.platform_data	= &ap_flash_data,
	},
	.num_resources	= 1,
	.resource	= &cfi_flash_resource,
};

static void __init ap_init(void)
{
	unsigned long sc_dec;
	int i;

	platform_device_register(&cfi_flash_device);

	sc_dec = readl(VA_SC_BASE + INTEGRATOR_SC_DEC_OFFSET);
	for (i = 0; i < 4; i++) {
		struct lm_device *lmdev;

		if ((sc_dec & (16 << i)) == 0)
			continue;

		lmdev = kzalloc(sizeof(struct lm_device), GFP_KERNEL);
		if (!lmdev)
			continue;

		lmdev->resource.start = 0xc0000000 + 0x10000000 * i;
		lmdev->resource.end = lmdev->resource.start + 0x0fffffff;
		lmdev->resource.flags = IORESOURCE_MEM;
		lmdev->irq = IRQ_AP_EXPINT0 + i;
		lmdev->id = i;

		lm_device_register(lmdev);
	}
}

/*
 * Where is the timer (VA)?
 */
#define TIMER0_VA_BASE IO_ADDRESS(INTEGRATOR_TIMER0_BASE)
#define TIMER1_VA_BASE IO_ADDRESS(INTEGRATOR_TIMER1_BASE)
#define TIMER2_VA_BASE IO_ADDRESS(INTEGRATOR_TIMER2_BASE)

/*
 * How long is the timer interval?
 */
#define TIMER_INTERVAL	(TICKS_PER_uSEC * mSEC_10)
#if TIMER_INTERVAL >= 0x100000
#define TICKS2USECS(x)	(256 * (x) / TICKS_PER_uSEC)
#elif TIMER_INTERVAL >= 0x10000
#define TICKS2USECS(x)	(16 * (x) / TICKS_PER_uSEC)
#else
#define TICKS2USECS(x)	((x) / TICKS_PER_uSEC)
#endif

static unsigned long timer_reload;

static void __iomem * const clksrc_base = (void __iomem *)TIMER2_VA_BASE;

static cycle_t timersp_read(struct clocksource *cs)
{
	return ~(readl(clksrc_base + TIMER_VALUE) & 0xffff);
}

static struct clocksource clocksource_timersp = {
	.name		= "timer2",
	.rating		= 200,
	.read		= timersp_read,
	.mask		= CLOCKSOURCE_MASK(16),
	.shift		= 16,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static void integrator_clocksource_init(u32 khz)
{
	struct clocksource *cs = &clocksource_timersp;
	void __iomem *base = clksrc_base;
	u32 ctrl = TIMER_CTRL_ENABLE;

	if (khz >= 1500) {
		khz /= 16;
		ctrl = TIMER_CTRL_DIV16;
	}

	writel(ctrl, base + TIMER_CTRL);
	writel(0xffff, base + TIMER_LOAD);

	cs->mult = clocksource_khz2mult(khz, cs->shift);
	clocksource_register(cs);
}

static void __iomem * const clkevt_base = (void __iomem *)TIMER1_VA_BASE;

/*
 * IRQ handler for the timer
 */
static irqreturn_t integrator_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = dev_id;

	/* clear the interrupt */
	writel(1, clkevt_base + TIMER_INTCLR);

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static void clkevt_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{
	u32 ctrl = readl(clkevt_base + TIMER_CTRL) & ~TIMER_CTRL_ENABLE;

	BUG_ON(mode == CLOCK_EVT_MODE_ONESHOT);

	if (mode == CLOCK_EVT_MODE_PERIODIC) {
		writel(ctrl, clkevt_base + TIMER_CTRL);
		writel(timer_reload, clkevt_base + TIMER_LOAD);
		ctrl |= TIMER_CTRL_PERIODIC | TIMER_CTRL_ENABLE;
	}

	writel(ctrl, clkevt_base + TIMER_CTRL);
}

static int clkevt_set_next_event(unsigned long next, struct clock_event_device *evt)
{
	unsigned long ctrl = readl(clkevt_base + TIMER_CTRL);

	writel(ctrl & ~TIMER_CTRL_ENABLE, clkevt_base + TIMER_CTRL);
	writel(next, clkevt_base + TIMER_LOAD);
	writel(ctrl | TIMER_CTRL_ENABLE, clkevt_base + TIMER_CTRL);

	return 0;
}

static struct clock_event_device integrator_clockevent = {
	.name		= "timer1",
	.shift		= 34,
	.features	= CLOCK_EVT_FEAT_PERIODIC,
	.set_mode	= clkevt_set_mode,
	.set_next_event	= clkevt_set_next_event,
	.rating		= 300,
	.cpumask	= cpu_all_mask,
};

static struct irqaction integrator_timer_irq = {
	.name		= "timer",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= integrator_timer_interrupt,
	.dev_id		= &integrator_clockevent,
};

static void integrator_clockevent_init(u32 khz)
{
	struct clock_event_device *evt = &integrator_clockevent;
	unsigned int ctrl = 0;

	if (khz * 1000 > 0x100000 * HZ) {
		khz /= 256;
		ctrl |= TIMER_CTRL_DIV256;
	} else if (khz * 1000 > 0x10000 * HZ) {
		khz /= 16;
		ctrl |= TIMER_CTRL_DIV16;
	}

	timer_reload = khz * 1000 / HZ;
	writel(ctrl, clkevt_base + TIMER_CTRL);

	evt->irq = IRQ_TIMERINT1;
	evt->mult = div_sc(khz, NSEC_PER_MSEC, evt->shift);
	evt->max_delta_ns = clockevent_delta2ns(0xffff, evt);
	evt->min_delta_ns = clockevent_delta2ns(0xf, evt);

	setup_irq(IRQ_TIMERINT1, &integrator_timer_irq);
	clockevents_register_device(evt);
}

/*
 * Set up timer(s).
 */
static void __init ap_init_timer(void)
{
	u32 khz = TICKS_PER_uSEC * 1000;

	writel(0, TIMER0_VA_BASE + TIMER_CTRL);
	writel(0, TIMER1_VA_BASE + TIMER_CTRL);
	writel(0, TIMER2_VA_BASE + TIMER_CTRL);

	integrator_clocksource_init(khz);
	integrator_clockevent_init(khz);
}

static struct sys_timer ap_timer = {
	.init		= ap_init_timer,
};

MACHINE_START(INTEGRATOR, "ARM-Integrator")
	/* Maintainer: ARM Ltd/Deep Blue Solutions Ltd */
	.phys_io	= 0x16000000,
	.io_pg_offst	= ((0xf1600000) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.map_io		= ap_map_io,
	.init_irq	= ap_init_irq,
	.timer		= &ap_timer,
	.init_machine	= ap_init,
MACHINE_END
