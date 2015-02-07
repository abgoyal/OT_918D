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

#ifndef _M68K_CACHEFLUSH_H
#define _M68K_CACHEFLUSH_H

#include <linux/mm.h>

/* cache code */
#define FLUSH_I_AND_D	(0x00000808)
#define FLUSH_I		(0x00000008)

/*
 * Cache handling functions
 */

static inline void flush_icache(void)
{
	if (CPU_IS_040_OR_060)
		asm volatile (	"nop\n"
			"	.chip	68040\n"
			"	cpusha	%bc\n"
			"	.chip	68k");
	else {
		unsigned long tmp;
		asm volatile (	"movec	%%cacr,%0\n"
			"	or.w	%1,%0\n"
			"	movec	%0,%%cacr"
			: "=&d" (tmp)
			: "id" (FLUSH_I));
	}
}

/*
 * invalidate the cache for the specified memory range.
 * It starts at the physical address specified for
 * the given number of bytes.
 */
extern void cache_clear(unsigned long paddr, int len);
/*
 * push any dirty cache in the specified memory range.
 * It starts at the physical address specified for
 * the given number of bytes.
 */
extern void cache_push(unsigned long paddr, int len);

/*
 * push and invalidate pages in the specified user virtual
 * memory range.
 */
extern void cache_push_v(unsigned long vaddr, int len);

/* This is needed whenever the virtual mapping of the current
   process changes.  */
#define __flush_cache_all()					\
({								\
	if (CPU_IS_040_OR_060)					\
		__asm__ __volatile__("nop\n\t"			\
				     ".chip 68040\n\t"		\
				     "cpusha %dc\n\t"		\
				     ".chip 68k");		\
	else {							\
		unsigned long _tmp;				\
		__asm__ __volatile__("movec %%cacr,%0\n\t"	\
				     "orw %1,%0\n\t"		\
				     "movec %0,%%cacr"		\
				     : "=&d" (_tmp)		\
				     : "di" (FLUSH_I_AND_D));	\
	}							\
})

#define __flush_cache_030()					\
({								\
	if (CPU_IS_020_OR_030) {				\
		unsigned long _tmp;				\
		__asm__ __volatile__("movec %%cacr,%0\n\t"	\
				     "orw %1,%0\n\t"		\
				     "movec %0,%%cacr"		\
				     : "=&d" (_tmp)		\
				     : "di" (FLUSH_I_AND_D));	\
	}							\
})

#define flush_cache_all() __flush_cache_all()

#define flush_cache_vmap(start, end)		flush_cache_all()
#define flush_cache_vunmap(start, end)		flush_cache_all()

static inline void flush_cache_mm(struct mm_struct *mm)
{
	if (mm == current->mm)
		__flush_cache_030();
}

#define flush_cache_dup_mm(mm)			flush_cache_mm(mm)

/* flush_cache_range/flush_cache_page must be macros to avoid
   a dependency on linux/mm.h, which includes this file... */
static inline void flush_cache_range(struct vm_area_struct *vma,
				     unsigned long start,
				     unsigned long end)
{
	if (vma->vm_mm == current->mm)
	        __flush_cache_030();
}

static inline void flush_cache_page(struct vm_area_struct *vma, unsigned long vmaddr, unsigned long pfn)
{
	if (vma->vm_mm == current->mm)
	        __flush_cache_030();
}


/* Push the page at kernel virtual address and clear the icache */
/* RZ: use cpush %bc instead of cpush %dc, cinv %ic */
static inline void __flush_page_to_ram(void *vaddr)
{
	if (CPU_IS_040_OR_060) {
		__asm__ __volatile__("nop\n\t"
				     ".chip 68040\n\t"
				     "cpushp %%bc,(%0)\n\t"
				     ".chip 68k"
				     : : "a" (__pa(vaddr)));
	} else {
		unsigned long _tmp;
		__asm__ __volatile__("movec %%cacr,%0\n\t"
				     "orw %1,%0\n\t"
				     "movec %0,%%cacr"
				     : "=&d" (_tmp)
				     : "di" (FLUSH_I));
	}
}

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
#define flush_dcache_page(page)		__flush_page_to_ram(page_address(page))
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)
#define flush_icache_page(vma, page)	__flush_page_to_ram(page_address(page))

extern void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
				    unsigned long addr, int len);
extern void flush_icache_range(unsigned long address, unsigned long endaddr);

static inline void copy_to_user_page(struct vm_area_struct *vma,
				     struct page *page, unsigned long vaddr,
				     void *dst, void *src, int len)
{
	flush_cache_page(vma, vaddr, page_to_pfn(page));
	memcpy(dst, src, len);
	flush_icache_user_range(vma, page, vaddr, len);
}
static inline void copy_from_user_page(struct vm_area_struct *vma,
				       struct page *page, unsigned long vaddr,
				       void *dst, void *src, int len)
{
	flush_cache_page(vma, vaddr, page_to_pfn(page));
	memcpy(dst, src, len);
}

#endif /* _M68K_CACHEFLUSH_H */
