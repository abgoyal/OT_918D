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
**  linux/amiga/chipram.c
**
**      Modified 03-May-94 by Geert Uytterhoeven <geert@linux-m68k.org>
**          - 64-bit aligned allocations for full AGA compatibility
**
**	Rewritten 15/9/2000 by Geert to use resource management
*/

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/module.h>

#include <asm/page.h>
#include <asm/amigahw.h>

unsigned long amiga_chip_size;
EXPORT_SYMBOL(amiga_chip_size);

static struct resource chipram_res = {
    .name = "Chip RAM", .start = CHIP_PHYSADDR
};
static unsigned long chipavail;


void __init amiga_chip_init(void)
{
    if (!AMIGAHW_PRESENT(CHIP_RAM))
	return;

    /*
     *  Remove the first 4 pages where PPC exception handlers will be located
     */
    amiga_chip_size -= 0x4000;
    chipram_res.end = amiga_chip_size-1;
    request_resource(&iomem_resource, &chipram_res);

    chipavail = amiga_chip_size;
}


void *amiga_chip_alloc(unsigned long size, const char *name)
{
    struct resource *res;

    /* round up */
    size = PAGE_ALIGN(size);

#ifdef DEBUG
    printk("amiga_chip_alloc: allocate %ld bytes\n", size);
#endif
    res = kzalloc(sizeof(struct resource), GFP_KERNEL);
    if (!res)
	return NULL;
    res->name = name;

    if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	kfree(res);
	return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
}
EXPORT_SYMBOL(amiga_chip_alloc);


    /*
     *  Warning:
     *  amiga_chip_alloc_res is meant only for drivers that need to allocate
     *  Chip RAM before kmalloc() is functional. As a consequence, those
     *  drivers must not free that Chip RAM afterwards.
     */

void * __init amiga_chip_alloc_res(unsigned long size, struct resource *res)
{
    unsigned long start;

    /* round up */
    size = PAGE_ALIGN(size);
    /* dmesg into chipmem prefers memory at the safe end */
    start = CHIP_PHYSADDR + chipavail - size;

#ifdef DEBUG
    printk("amiga_chip_alloc_res: allocate %ld bytes\n", size);
#endif
    if (allocate_resource(&chipram_res, res, size, start, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	printk("amiga_chip_alloc_res: first alloc failed!\n");
	if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0)
	    return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc_res: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
}

void amiga_chip_free(void *ptr)
{
    unsigned long start = ZTWO_PADDR(ptr);
    struct resource **p, *res;
    unsigned long size;

    for (p = &chipram_res.child; (res = *p); p = &res->sibling) {
	if (res->start != start)
	    continue;
	*p = res->sibling;
	size = res->end-start;
#ifdef DEBUG
	printk("amiga_chip_free: free %ld bytes at %p\n", size, ptr);
#endif
	chipavail += size;
	kfree(res);
	return;
    }
    printk("amiga_chip_free: trying to free nonexistent region at %p\n", ptr);
}
EXPORT_SYMBOL(amiga_chip_free);


unsigned long amiga_chip_avail(void)
{
#ifdef DEBUG
	printk("amiga_chip_avail : %ld bytes\n", chipavail);
#endif
	return chipavail;
}
EXPORT_SYMBOL(amiga_chip_avail);
