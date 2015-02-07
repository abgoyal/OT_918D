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
 * Based on the same principle as kgdboe using the NETPOLL api, this
 * driver uses a console polling api to implement a gdb serial inteface
 * which is multiplexed on a console port.
 *
 * Maintainer: Jason Wessel <jason.wessel@windriver.com>
 *
 * 2007-2008 (c) Jason Wessel - Wind River Systems, Inc.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/kgdb.h>
#include <linux/kdb.h>
#include <linux/tty.h>
#include <linux/console.h>

#define MAX_CONFIG_LEN		40

static struct kgdb_io		kgdboc_io_ops;

/* -1 = init not run yet, 0 = unconfigured, 1 = configured. */
static int configured		= -1;

static char config[MAX_CONFIG_LEN];
static struct kparam_string kps = {
	.string			= config,
	.maxlen			= MAX_CONFIG_LEN,
};

static struct tty_driver	*kgdb_tty_driver;
static int			kgdb_tty_line;

#ifdef CONFIG_KDB_KEYBOARD
static int kgdboc_register_kbd(char **cptr)
{
	if (strncmp(*cptr, "kbd", 3) == 0) {
		if (kdb_poll_idx < KDB_POLL_FUNC_MAX) {
			kdb_poll_funcs[kdb_poll_idx] = kdb_get_kbd_char;
			kdb_poll_idx++;
			if (cptr[0][3] == ',')
				*cptr += 4;
			else
				return 1;
		}
	}
	return 0;
}

static void kgdboc_unregister_kbd(void)
{
	int i;

	for (i = 0; i < kdb_poll_idx; i++) {
		if (kdb_poll_funcs[i] == kdb_get_kbd_char) {
			kdb_poll_idx--;
			kdb_poll_funcs[i] = kdb_poll_funcs[kdb_poll_idx];
			kdb_poll_funcs[kdb_poll_idx] = NULL;
			i--;
		}
	}
}
#else /* ! CONFIG_KDB_KEYBOARD */
#define kgdboc_register_kbd(x) 0
#define kgdboc_unregister_kbd()
#endif /* ! CONFIG_KDB_KEYBOARD */

static int kgdboc_option_setup(char *opt)
{
	if (strlen(opt) > MAX_CONFIG_LEN) {
		printk(KERN_ERR "kgdboc: config string too long\n");
		return -ENOSPC;
	}
	strcpy(config, opt);

	return 0;
}

__setup("kgdboc=", kgdboc_option_setup);

static void cleanup_kgdboc(void)
{
	kgdboc_unregister_kbd();
	if (configured == 1)
		kgdb_unregister_io_module(&kgdboc_io_ops);
}

static int configure_kgdboc(void)
{
	struct tty_driver *p;
	int tty_line = 0;
	int err;
	char *cptr = config;
	struct console *cons;

	err = kgdboc_option_setup(config);
	if (err || !strlen(config) || isspace(config[0]))
		goto noconfig;

	err = -ENODEV;
	kgdboc_io_ops.is_console = 0;
	kgdb_tty_driver = NULL;

	if (kgdboc_register_kbd(&cptr))
		goto do_register;

	p = tty_find_polling_driver(cptr, &tty_line);
	if (!p)
		goto noconfig;

	cons = console_drivers;
	while (cons) {
		int idx;
		if (cons->device && cons->device(cons, &idx) == p &&
		    idx == tty_line) {
			kgdboc_io_ops.is_console = 1;
			break;
		}
		cons = cons->next;
	}

	kgdb_tty_driver = p;
	kgdb_tty_line = tty_line;

do_register:
	err = kgdb_register_io_module(&kgdboc_io_ops);
	if (err)
		goto noconfig;

	configured = 1;

	return 0;

noconfig:
	config[0] = 0;
	configured = 0;
	cleanup_kgdboc();

	return err;
}

static int __init init_kgdboc(void)
{
	/* Already configured? */
	if (configured == 1)
		return 0;

	return configure_kgdboc();
}

static int kgdboc_get_char(void)
{
	if (!kgdb_tty_driver)
		return -1;
	return kgdb_tty_driver->ops->poll_get_char(kgdb_tty_driver,
						kgdb_tty_line);
}

static void kgdboc_put_char(u8 chr)
{
	if (!kgdb_tty_driver)
		return;
	kgdb_tty_driver->ops->poll_put_char(kgdb_tty_driver,
					kgdb_tty_line, chr);
}

static int param_set_kgdboc_var(const char *kmessage, struct kernel_param *kp)
{
	int len = strlen(kmessage);

	if (len >= MAX_CONFIG_LEN) {
		printk(KERN_ERR "kgdboc: config string too long\n");
		return -ENOSPC;
	}

	/* Only copy in the string if the init function has not run yet */
	if (configured < 0) {
		strcpy(config, kmessage);
		return 0;
	}

	if (kgdb_connected) {
		printk(KERN_ERR
		       "kgdboc: Cannot reconfigure while KGDB is connected.\n");

		return -EBUSY;
	}

	strcpy(config, kmessage);
	/* Chop out \n char as a result of echo */
	if (config[len - 1] == '\n')
		config[len - 1] = '\0';

	if (configured == 1)
		cleanup_kgdboc();

	/* Go and configure with the new params. */
	return configure_kgdboc();
}

static void kgdboc_pre_exp_handler(void)
{
	/* Increment the module count when the debugger is active */
	if (!kgdb_connected)
		try_module_get(THIS_MODULE);
}

static void kgdboc_post_exp_handler(void)
{
	/* decrement the module count when the debugger detaches */
	if (!kgdb_connected)
		module_put(THIS_MODULE);
}

static struct kgdb_io kgdboc_io_ops = {
	.name			= "kgdboc",
	.read_char		= kgdboc_get_char,
	.write_char		= kgdboc_put_char,
	.pre_exception		= kgdboc_pre_exp_handler,
	.post_exception		= kgdboc_post_exp_handler,
};

#ifdef CONFIG_KGDB_SERIAL_CONSOLE
/* This is only available if kgdboc is a built in for early debugging */
int __init kgdboc_early_init(char *opt)
{
	/* save the first character of the config string because the
	 * init routine can destroy it.
	 */
	char save_ch;

	kgdboc_option_setup(opt);
	save_ch = config[0];
	init_kgdboc();
	config[0] = save_ch;
	return 0;
}

early_param("ekgdboc", kgdboc_early_init);
#endif /* CONFIG_KGDB_SERIAL_CONSOLE */

module_init(init_kgdboc);
module_exit(cleanup_kgdboc);
module_param_call(kgdboc, param_set_kgdboc_var, param_get_string, &kps, 0644);
MODULE_PARM_DESC(kgdboc, "<serial_device>[,baud]");
MODULE_DESCRIPTION("KGDB Console TTY Driver");
MODULE_LICENSE("GPL");
