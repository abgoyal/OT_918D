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
 * polling mode stateless debugging stuff, originally for NS16550 Serial Ports
 *
 * c 2001 PPC 64 Team, IBM Corp
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <stdarg.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/console.h>
#include <linux/init.h>
#include <asm/processor.h>
#include <asm/udbg.h>

void (*udbg_putc)(char c);
void (*udbg_flush)(void);
int (*udbg_getc)(void);
int (*udbg_getc_poll)(void);

/*
 * Early debugging facilities. You can enable _one_ of these via .config,
 * if you do so your kernel _will not boot_ on anything else. Be careful.
 */
void __init udbg_early_init(void)
{
#if defined(CONFIG_PPC_EARLY_DEBUG_LPAR)
	/* For LPAR machines that have an HVC console on vterm 0 */
	udbg_init_debug_lpar();
#elif defined(CONFIG_PPC_EARLY_DEBUG_G5)
	/* For use on Apple G5 machines */
	udbg_init_pmac_realmode();
#elif defined(CONFIG_PPC_EARLY_DEBUG_RTAS_PANEL)
	/* RTAS panel debug */
	udbg_init_rtas_panel();
#elif defined(CONFIG_PPC_EARLY_DEBUG_RTAS_CONSOLE)
	/* RTAS console debug */
	udbg_init_rtas_console();
#elif defined(CONFIG_PPC_EARLY_DEBUG_MAPLE)
	/* Maple real mode debug */
	udbg_init_maple_realmode();
#elif defined(CONFIG_PPC_EARLY_DEBUG_ISERIES)
	/* For iSeries - hit Ctrl-x Ctrl-x to see the output */
	udbg_init_iseries();
#elif defined(CONFIG_PPC_EARLY_DEBUG_BEAT)
	udbg_init_debug_beat();
#elif defined(CONFIG_PPC_EARLY_DEBUG_PAS_REALMODE)
	udbg_init_pas_realmode();
#elif defined(CONFIG_BOOTX_TEXT)
	udbg_init_btext();
#elif defined(CONFIG_PPC_EARLY_DEBUG_44x)
	/* PPC44x debug */
	udbg_init_44x_as1();
#elif defined(CONFIG_PPC_EARLY_DEBUG_40x)
	/* PPC40x debug */
	udbg_init_40x_realmode();
#elif defined(CONFIG_PPC_EARLY_DEBUG_CPM)
	udbg_init_cpm();
#elif defined(CONFIG_PPC_EARLY_DEBUG_USBGECKO)
	udbg_init_usbgecko();
#endif

#ifdef CONFIG_PPC_EARLY_DEBUG
	console_loglevel = 10;
#endif
}

/* udbg library, used by xmon et al */
void udbg_puts(const char *s)
{
	if (udbg_putc) {
		char c;

		if (s && *s != '\0') {
			while ((c = *s++) != '\0')
				udbg_putc(c);
		}

		if (udbg_flush)
			udbg_flush();
	}
#if 0
	else {
		printk("%s", s);
	}
#endif
}

int udbg_write(const char *s, int n)
{
	int remain = n;
	char c;

	if (!udbg_putc)
		return 0;

	if (s && *s != '\0') {
		while (((c = *s++) != '\0') && (remain-- > 0)) {
			udbg_putc(c);
		}
	}

	if (udbg_flush)
		udbg_flush();

	return n - remain;
}

int udbg_read(char *buf, int buflen)
{
	char *p = buf;
	int i, c;

	if (!udbg_getc)
		return 0;

	for (i = 0; i < buflen; ++i) {
		do {
			c = udbg_getc();
			if (c == -1 && i == 0)
				return -1;

		} while (c == 0x11 || c == 0x13);
		if (c == 0 || c == -1)
			break;
		*p++ = c;
	}

	return i;
}

#define UDBG_BUFSIZE 256
void udbg_printf(const char *fmt, ...)
{
	char buf[UDBG_BUFSIZE];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, UDBG_BUFSIZE, fmt, args);
	udbg_puts(buf);
	va_end(args);
}

void __init udbg_progress(char *s, unsigned short hex)
{
	udbg_puts(s);
	udbg_puts("\n");
}

/*
 * Early boot console based on udbg
 */
static void udbg_console_write(struct console *con, const char *s,
		unsigned int n)
{
	udbg_write(s, n);
}

static struct console udbg_console = {
	.name	= "udbg",
	.write	= udbg_console_write,
	.flags	= CON_PRINTBUFFER | CON_ENABLED | CON_BOOT | CON_ANYTIME,
	.index	= 0,
};

static int early_console_initialized;

/*
 * Called by setup_system after ppc_md->probe and ppc_md->early_init.
 * Call it again after setting udbg_putc in ppc_md->setup_arch.
 */
void __init register_early_udbg_console(void)
{
	if (early_console_initialized)
		return;

	if (!udbg_putc)
		return;

	if (strstr(boot_command_line, "udbg-immortal")) {
		printk(KERN_INFO "early console immortal !\n");
		udbg_console.flags &= ~CON_BOOT;
	}
	early_console_initialized = 1;
	register_console(&udbg_console);
}

#if 0   /* if you want to use this as a regular output console */
console_initcall(register_udbg_console);
#endif
