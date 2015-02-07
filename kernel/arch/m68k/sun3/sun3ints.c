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
 * linux/arch/m68k/sun3/sun3ints.c -- Sun-3(x) Linux interrupt handling code
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <asm/segment.h>
#include <asm/intersil.h>
#include <asm/oplib.h>
#include <asm/sun3ints.h>
#include <asm/irq_regs.h>
#include <linux/seq_file.h>

extern void sun3_leds (unsigned char);

void sun3_disable_interrupts(void)
{
	sun3_disable_irq(0);
}

void sun3_enable_interrupts(void)
{
	sun3_enable_irq(0);
}

static int led_pattern[8] = {
       ~(0x80), ~(0x01),
       ~(0x40), ~(0x02),
       ~(0x20), ~(0x04),
       ~(0x10), ~(0x08)
};

volatile unsigned char* sun3_intreg;

void sun3_enable_irq(unsigned int irq)
{
	*sun3_intreg |=  (1 << irq);
}

void sun3_disable_irq(unsigned int irq)
{
	*sun3_intreg &= ~(1 << irq);
}

static irqreturn_t sun3_int7(int irq, void *dev_id)
{
	*sun3_intreg |=  (1 << irq);
	if (!(kstat_cpu(0).irqs[irq] % 2000))
		sun3_leds(led_pattern[(kstat_cpu(0).irqs[irq] % 16000) / 2000]);
	return IRQ_HANDLED;
}

static irqreturn_t sun3_int5(int irq, void *dev_id)
{
#ifdef CONFIG_SUN3
	intersil_clear();
#endif
        *sun3_intreg |=  (1 << irq);
#ifdef CONFIG_SUN3
	intersil_clear();
#endif
        do_timer(1);
#ifndef CONFIG_SMP
	update_process_times(user_mode(get_irq_regs()));
#endif
        if (!(kstat_cpu(0).irqs[irq] % 20))
                sun3_leds(led_pattern[(kstat_cpu(0).irqs[irq] % 160) / 20]);
	return IRQ_HANDLED;
}

static irqreturn_t sun3_vec255(int irq, void *dev_id)
{
//	intersil_clear();
	return IRQ_HANDLED;
}

static void sun3_inthandle(unsigned int irq, struct pt_regs *fp)
{
        *sun3_intreg &= ~(1 << irq);

	__m68k_handle_int(irq, fp);
}

static struct irq_controller sun3_irq_controller = {
	.name		= "sun3",
	.lock		= __SPIN_LOCK_UNLOCKED(sun3_irq_controller.lock),
	.startup	= m68k_irq_startup,
	.shutdown	= m68k_irq_shutdown,
	.enable		= sun3_enable_irq,
	.disable	= sun3_disable_irq,
};

void __init sun3_init_IRQ(void)
{
	*sun3_intreg = 1;

	m68k_setup_auto_interrupt(sun3_inthandle);
	m68k_setup_irq_controller(&sun3_irq_controller, IRQ_AUTO_1, 7);
	m68k_setup_user_interrupt(VEC_USER, 128, NULL);

	if (request_irq(IRQ_AUTO_5, sun3_int5, 0, "int5", NULL))
		pr_err("Couldn't register %s interrupt\n", "int5");
	if (request_irq(IRQ_AUTO_7, sun3_int7, 0, "int7", NULL))
		pr_err("Couldn't register %s interrupt\n", "int7");
	if (request_irq(IRQ_USER+127, sun3_vec255, 0, "vec255", NULL))
		pr_err("Couldn't register %s interrupt\n", "vec255");
}
