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
 * TI DaVinci GPIO Support
 *
 * Copyright (c) 2006-2007 David Brownell
 * Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>

#include <mach/gpio.h>

#include <asm/mach/irq.h>

struct davinci_gpio_regs {
	u32	dir;
	u32	out_data;
	u32	set_data;
	u32	clr_data;
	u32	in_data;
	u32	set_rising;
	u32	clr_rising;
	u32	set_falling;
	u32	clr_falling;
	u32	intstat;
};

#define chip2controller(chip)	\
	container_of(chip, struct davinci_gpio_controller, chip)

static struct davinci_gpio_controller chips[DIV_ROUND_UP(DAVINCI_N_GPIO, 32)];
static void __iomem *gpio_base;

static struct davinci_gpio_regs __iomem __init *gpio2regs(unsigned gpio)
{
	void __iomem *ptr;

	if (gpio < 32 * 1)
		ptr = gpio_base + 0x10;
	else if (gpio < 32 * 2)
		ptr = gpio_base + 0x38;
	else if (gpio < 32 * 3)
		ptr = gpio_base + 0x60;
	else if (gpio < 32 * 4)
		ptr = gpio_base + 0x88;
	else if (gpio < 32 * 5)
		ptr = gpio_base + 0xb0;
	else
		ptr = NULL;
	return ptr;
}

static inline struct davinci_gpio_regs __iomem *irq2regs(int irq)
{
	struct davinci_gpio_regs __iomem *g;

	g = (__force struct davinci_gpio_regs __iomem *)get_irq_chip_data(irq);

	return g;
}

static int __init davinci_gpio_irq_setup(void);

/*--------------------------------------------------------------------------*/

/* board setup code *MUST* setup pinmux and enable the GPIO clock. */
static inline int __davinci_direction(struct gpio_chip *chip,
			unsigned offset, bool out, int value)
{
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;
	unsigned long flags;
	u32 temp;
	u32 mask = 1 << offset;

	spin_lock_irqsave(&d->lock, flags);
	temp = __raw_readl(&g->dir);
	if (out) {
		temp &= ~mask;
		__raw_writel(mask, value ? &g->set_data : &g->clr_data);
	} else {
		temp |= mask;
	}
	__raw_writel(temp, &g->dir);
	spin_unlock_irqrestore(&d->lock, flags);

	return 0;
}

static int davinci_direction_in(struct gpio_chip *chip, unsigned offset)
{
	return __davinci_direction(chip, offset, false, 0);
}

static int
davinci_direction_out(struct gpio_chip *chip, unsigned offset, int value)
{
	return __davinci_direction(chip, offset, true, value);
}

/*
 * Read the pin's value (works even if it's set up as output);
 * returns zero/nonzero.
 *
 * Note that changes are synched to the GPIO clock, so reading values back
 * right after you've set them may give old values.
 */
static int davinci_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;

	return (1 << offset) & __raw_readl(&g->in_data);
}

/*
 * Assuming the pin is muxed as a gpio output, set its output value.
 */
static void
davinci_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;

	__raw_writel((1 << offset), value ? &g->set_data : &g->clr_data);
}

static int __init davinci_gpio_setup(void)
{
	int i, base;
	unsigned ngpio;
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	struct davinci_gpio_regs *regs;

	if (soc_info->gpio_type != GPIO_TYPE_DAVINCI)
		return 0;

	/*
	 * The gpio banks conceptually expose a segmented bitmap,
	 * and "ngpio" is one more than the largest zero-based
	 * bit index that's valid.
	 */
	ngpio = soc_info->gpio_num;
	if (ngpio == 0) {
		pr_err("GPIO setup:  how many GPIOs?\n");
		return -EINVAL;
	}

	if (WARN_ON(DAVINCI_N_GPIO < ngpio))
		ngpio = DAVINCI_N_GPIO;

	gpio_base = ioremap(soc_info->gpio_base, SZ_4K);
	if (WARN_ON(!gpio_base))
		return -ENOMEM;

	for (i = 0, base = 0; base < ngpio; i++, base += 32) {
		chips[i].chip.label = "DaVinci";

		chips[i].chip.direction_input = davinci_direction_in;
		chips[i].chip.get = davinci_gpio_get;
		chips[i].chip.direction_output = davinci_direction_out;
		chips[i].chip.set = davinci_gpio_set;

		chips[i].chip.base = base;
		chips[i].chip.ngpio = ngpio - base;
		if (chips[i].chip.ngpio > 32)
			chips[i].chip.ngpio = 32;

		spin_lock_init(&chips[i].lock);

		regs = gpio2regs(base);
		chips[i].regs = regs;
		chips[i].set_data = &regs->set_data;
		chips[i].clr_data = &regs->clr_data;
		chips[i].in_data = &regs->in_data;

		gpiochip_add(&chips[i].chip);
	}

	soc_info->gpio_ctlrs = chips;
	soc_info->gpio_ctlrs_num = DIV_ROUND_UP(ngpio, 32);

	davinci_gpio_irq_setup();
	return 0;
}
pure_initcall(davinci_gpio_setup);

/*--------------------------------------------------------------------------*/
/*
 * We expect irqs will normally be set up as input pins, but they can also be
 * used as output pins ... which is convenient for testing.
 *
 * NOTE:  The first few GPIOs also have direct INTC hookups in addition
 * to their GPIOBNK0 irq, with a bit less overhead.
 *
 * All those INTC hookups (direct, plus several IRQ banks) can also
 * serve as EDMA event triggers.
 */

static void gpio_irq_disable(unsigned irq)
{
	struct davinci_gpio_regs __iomem *g = irq2regs(irq);
	u32 mask = (u32) get_irq_data(irq);

	__raw_writel(mask, &g->clr_falling);
	__raw_writel(mask, &g->clr_rising);
}

static void gpio_irq_enable(unsigned irq)
{
	struct davinci_gpio_regs __iomem *g = irq2regs(irq);
	u32 mask = (u32) get_irq_data(irq);
	unsigned status = irq_desc[irq].status;

	status &= IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING;
	if (!status)
		status = IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING;

	if (status & IRQ_TYPE_EDGE_FALLING)
		__raw_writel(mask, &g->set_falling);
	if (status & IRQ_TYPE_EDGE_RISING)
		__raw_writel(mask, &g->set_rising);
}

static int gpio_irq_type(unsigned irq, unsigned trigger)
{
	struct davinci_gpio_regs __iomem *g = irq2regs(irq);
	u32 mask = (u32) get_irq_data(irq);

	if (trigger & ~(IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		return -EINVAL;

	irq_desc[irq].status &= ~IRQ_TYPE_SENSE_MASK;
	irq_desc[irq].status |= trigger;

	/* don't enable the IRQ if it's currently disabled */
	if (irq_desc[irq].depth == 0) {
		__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_FALLING)
			     ? &g->set_falling : &g->clr_falling);
		__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_RISING)
			     ? &g->set_rising : &g->clr_rising);
	}
	return 0;
}

static struct irq_chip gpio_irqchip = {
	.name		= "GPIO",
	.enable		= gpio_irq_enable,
	.disable	= gpio_irq_disable,
	.set_type	= gpio_irq_type,
};

static void
gpio_irq_handler(unsigned irq, struct irq_desc *desc)
{
	struct davinci_gpio_regs __iomem *g = irq2regs(irq);
	u32 mask = 0xffff;

	/* we only care about one bank */
	if (irq & 1)
		mask <<= 16;

	/* temporarily mask (level sensitive) parent IRQ */
	desc->chip->mask(irq);
	desc->chip->ack(irq);
	while (1) {
		u32		status;
		int		n;
		int		res;

		/* ack any irqs */
		status = __raw_readl(&g->intstat) & mask;
		if (!status)
			break;
		__raw_writel(status, &g->intstat);
		if (irq & 1)
			status >>= 16;

		/* now demux them to the right lowlevel handler */
		n = (int)get_irq_data(irq);
		while (status) {
			res = ffs(status);
			n += res;
			generic_handle_irq(n - 1);
			status >>= res;
		}
	}
	desc->chip->unmask(irq);
	/* now it may re-trigger */
}

static int gpio_to_irq_banked(struct gpio_chip *chip, unsigned offset)
{
	struct davinci_gpio_controller *d = chip2controller(chip);

	if (d->irq_base >= 0)
		return d->irq_base + offset;
	else
		return -ENODEV;
}

static int gpio_to_irq_unbanked(struct gpio_chip *chip, unsigned offset)
{
	struct davinci_soc_info *soc_info = &davinci_soc_info;

	/* NOTE:  we assume for now that only irqs in the first gpio_chip
	 * can provide direct-mapped IRQs to AINTC (up to 32 GPIOs).
	 */
	if (offset < soc_info->gpio_unbanked)
		return soc_info->gpio_irq + offset;
	else
		return -ENODEV;
}

static int gpio_irq_type_unbanked(unsigned irq, unsigned trigger)
{
	struct davinci_gpio_regs __iomem *g = irq2regs(irq);
	u32 mask = (u32) get_irq_data(irq);

	if (trigger & ~(IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		return -EINVAL;

	__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_FALLING)
		     ? &g->set_falling : &g->clr_falling);
	__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_RISING)
		     ? &g->set_rising : &g->clr_rising);

	return 0;
}

/*
 * NOTE:  for suspend/resume, probably best to make a platform_device with
 * suspend_late/resume_resume calls hooking into results of the set_wake()
 * calls ... so if no gpios are wakeup events the clock can be disabled,
 * with outputs left at previously set levels, and so that VDD3P3V.IOPWDN0
 * (dm6446) can be set appropriately for GPIOV33 pins.
 */

static int __init davinci_gpio_irq_setup(void)
{
	unsigned	gpio, irq, bank;
	struct clk	*clk;
	u32		binten = 0;
	unsigned	ngpio, bank_irq;
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	struct davinci_gpio_regs	__iomem *g;

	ngpio = soc_info->gpio_num;

	bank_irq = soc_info->gpio_irq;
	if (bank_irq == 0) {
		printk(KERN_ERR "Don't know first GPIO bank IRQ.\n");
		return -EINVAL;
	}

	clk = clk_get(NULL, "gpio");
	if (IS_ERR(clk)) {
		printk(KERN_ERR "Error %ld getting gpio clock?\n",
		       PTR_ERR(clk));
		return PTR_ERR(clk);
	}
	clk_enable(clk);

	/* Arrange gpio_to_irq() support, handling either direct IRQs or
	 * banked IRQs.  Having GPIOs in the first GPIO bank use direct
	 * IRQs, while the others use banked IRQs, would need some setup
	 * tweaks to recognize hardware which can do that.
	 */
	for (gpio = 0, bank = 0; gpio < ngpio; bank++, gpio += 32) {
		chips[bank].chip.to_irq = gpio_to_irq_banked;
		chips[bank].irq_base = soc_info->gpio_unbanked
			? -EINVAL
			: (soc_info->intc_irq_num + gpio);
	}

	/*
	 * AINTC can handle direct/unbanked IRQs for GPIOs, with the GPIO
	 * controller only handling trigger modes.  We currently assume no
	 * IRQ mux conflicts; gpio_irq_type_unbanked() is only for GPIOs.
	 */
	if (soc_info->gpio_unbanked) {
		static struct irq_chip gpio_irqchip_unbanked;

		/* pass "bank 0" GPIO IRQs to AINTC */
		chips[0].chip.to_irq = gpio_to_irq_unbanked;
		binten = BIT(0);

		/* AINTC handles mask/unmask; GPIO handles triggering */
		irq = bank_irq;
		gpio_irqchip_unbanked = *get_irq_desc_chip(irq_to_desc(irq));
		gpio_irqchip_unbanked.name = "GPIO-AINTC";
		gpio_irqchip_unbanked.set_type = gpio_irq_type_unbanked;

		/* default trigger: both edges */
		g = gpio2regs(0);
		__raw_writel(~0, &g->set_falling);
		__raw_writel(~0, &g->set_rising);

		/* set the direct IRQs up to use that irqchip */
		for (gpio = 0; gpio < soc_info->gpio_unbanked; gpio++, irq++) {
			set_irq_chip(irq, &gpio_irqchip_unbanked);
			set_irq_data(irq, (void *) __gpio_mask(gpio));
			set_irq_chip_data(irq, (__force void *) g);
			irq_desc[irq].status |= IRQ_TYPE_EDGE_BOTH;
		}

		goto done;
	}

	/*
	 * Or, AINTC can handle IRQs for banks of 16 GPIO IRQs, which we
	 * then chain through our own handler.
	 */
	for (gpio = 0, irq = gpio_to_irq(0), bank = 0;
			gpio < ngpio;
			bank++, bank_irq++) {
		unsigned		i;

		/* disabled by default, enabled only as needed */
		g = gpio2regs(gpio);
		__raw_writel(~0, &g->clr_falling);
		__raw_writel(~0, &g->clr_rising);

		/* set up all irqs in this bank */
		set_irq_chained_handler(bank_irq, gpio_irq_handler);
		set_irq_chip_data(bank_irq, (__force void *) g);
		set_irq_data(bank_irq, (void *) irq);

		for (i = 0; i < 16 && gpio < ngpio; i++, irq++, gpio++) {
			set_irq_chip(irq, &gpio_irqchip);
			set_irq_chip_data(irq, (__force void *) g);
			set_irq_data(irq, (void *) __gpio_mask(gpio));
			set_irq_handler(irq, handle_simple_irq);
			set_irq_flags(irq, IRQF_VALID);
		}

		binten |= BIT(bank);
	}

done:
	/* BINTEN -- per-bank interrupt enable. genirq would also let these
	 * bits be set/cleared dynamically.
	 */
	__raw_writel(binten, gpio_base + 0x08);

	printk(KERN_INFO "DaVinci: %d gpio irqs\n", irq - gpio_to_irq(0));

	return 0;
}