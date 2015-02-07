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
 * arch/sh/mm/cache.c
 *
 * Copyright (C) 1999, 2000, 2002  Niibe Yutaka
 * Copyright (C) 2002 - 2010  Paul Mundt
 *
 * Released under the terms of the GNU GPL v2.0.
 */
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/smp.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

void (*local_flush_cache_all)(void *args) = cache_noop;
void (*local_flush_cache_mm)(void *args) = cache_noop;
void (*local_flush_cache_dup_mm)(void *args) = cache_noop;
void (*local_flush_cache_page)(void *args) = cache_noop;
void (*local_flush_cache_range)(void *args) = cache_noop;
void (*local_flush_dcache_page)(void *args) = cache_noop;
void (*local_flush_icache_range)(void *args) = cache_noop;
void (*local_flush_icache_page)(void *args) = cache_noop;
void (*local_flush_cache_sigtramp)(void *args) = cache_noop;

void (*__flush_wback_region)(void *start, int size);
EXPORT_SYMBOL(__flush_wback_region);
void (*__flush_purge_region)(void *start, int size);
EXPORT_SYMBOL(__flush_purge_region);
void (*__flush_invalidate_region)(void *start, int size);
EXPORT_SYMBOL(__flush_invalidate_region);

static inline void noop__flush_region(void *start, int size)
{
}

static inline void cacheop_on_each_cpu(void (*func) (void *info), void *info,
                                   int wait)
{
	preempt_disable();

	/*
	 * It's possible that this gets called early on when IRQs are
	 * still disabled due to ioremapping by the boot CPU, so don't
	 * even attempt IPIs unless there are other CPUs online.
	 */
	if (num_online_cpus() > 1)
		smp_call_function(func, info, wait);

	func(info);

	preempt_enable();
}

void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		       unsigned long vaddr, void *dst, const void *src,
		       unsigned long len)
{
	if (boot_cpu_data.dcache.n_aliases && page_mapped(page) &&
	    !test_bit(PG_dcache_dirty, &page->flags)) {
		void *vto = kmap_coherent(page, vaddr) + (vaddr & ~PAGE_MASK);
		memcpy(vto, src, len);
		kunmap_coherent(vto);
	} else {
		memcpy(dst, src, len);
		if (boot_cpu_data.dcache.n_aliases)
			set_bit(PG_dcache_dirty, &page->flags);
	}

	if (vma->vm_flags & VM_EXEC)
		flush_cache_page(vma, vaddr, page_to_pfn(page));
}

void copy_from_user_page(struct vm_area_struct *vma, struct page *page,
			 unsigned long vaddr, void *dst, const void *src,
			 unsigned long len)
{
	if (boot_cpu_data.dcache.n_aliases && page_mapped(page) &&
	    !test_bit(PG_dcache_dirty, &page->flags)) {
		void *vfrom = kmap_coherent(page, vaddr) + (vaddr & ~PAGE_MASK);
		memcpy(dst, vfrom, len);
		kunmap_coherent(vfrom);
	} else {
		memcpy(dst, src, len);
		if (boot_cpu_data.dcache.n_aliases)
			set_bit(PG_dcache_dirty, &page->flags);
	}
}

void copy_user_highpage(struct page *to, struct page *from,
			unsigned long vaddr, struct vm_area_struct *vma)
{
	void *vfrom, *vto;

	vto = kmap_atomic(to, KM_USER1);

	if (boot_cpu_data.dcache.n_aliases && page_mapped(from) &&
	    !test_bit(PG_dcache_dirty, &from->flags)) {
		vfrom = kmap_coherent(from, vaddr);
		copy_page(vto, vfrom);
		kunmap_coherent(vfrom);
	} else {
		vfrom = kmap_atomic(from, KM_USER0);
		copy_page(vto, vfrom);
		kunmap_atomic(vfrom, KM_USER0);
	}

	if (pages_do_alias((unsigned long)vto, vaddr & PAGE_MASK))
		__flush_purge_region(vto, PAGE_SIZE);

	kunmap_atomic(vto, KM_USER1);
	/* Make sure this page is cleared on other CPU's too before using it */
	smp_wmb();
}
EXPORT_SYMBOL(copy_user_highpage);

void clear_user_highpage(struct page *page, unsigned long vaddr)
{
	void *kaddr = kmap_atomic(page, KM_USER0);

	clear_page(kaddr);

	if (pages_do_alias((unsigned long)kaddr, vaddr & PAGE_MASK))
		__flush_purge_region(kaddr, PAGE_SIZE);

	kunmap_atomic(kaddr, KM_USER0);
}
EXPORT_SYMBOL(clear_user_highpage);

void __update_cache(struct vm_area_struct *vma,
		    unsigned long address, pte_t pte)
{
	struct page *page;
	unsigned long pfn = pte_pfn(pte);

	if (!boot_cpu_data.dcache.n_aliases)
		return;

	page = pfn_to_page(pfn);
	if (pfn_valid(pfn)) {
		int dirty = test_and_clear_bit(PG_dcache_dirty, &page->flags);
		if (dirty)
			__flush_purge_region(page_address(page), PAGE_SIZE);
	}
}

void __flush_anon_page(struct page *page, unsigned long vmaddr)
{
	unsigned long addr = (unsigned long) page_address(page);

	if (pages_do_alias(addr, vmaddr)) {
		if (boot_cpu_data.dcache.n_aliases && page_mapped(page) &&
		    !test_bit(PG_dcache_dirty, &page->flags)) {
			void *kaddr;

			kaddr = kmap_coherent(page, vmaddr);
			/* XXX.. For now kunmap_coherent() does a purge */
			/* __flush_purge_region((void *)kaddr, PAGE_SIZE); */
			kunmap_coherent(kaddr);
		} else
			__flush_purge_region((void *)addr, PAGE_SIZE);
	}
}

void flush_cache_all(void)
{
	cacheop_on_each_cpu(local_flush_cache_all, NULL, 1);
}
EXPORT_SYMBOL(flush_cache_all);

void flush_cache_mm(struct mm_struct *mm)
{
	if (boot_cpu_data.dcache.n_aliases == 0)
		return;

	cacheop_on_each_cpu(local_flush_cache_mm, mm, 1);
}

void flush_cache_dup_mm(struct mm_struct *mm)
{
	if (boot_cpu_data.dcache.n_aliases == 0)
		return;

	cacheop_on_each_cpu(local_flush_cache_dup_mm, mm, 1);
}

void flush_cache_page(struct vm_area_struct *vma, unsigned long addr,
		      unsigned long pfn)
{
	struct flusher_data data;

	data.vma = vma;
	data.addr1 = addr;
	data.addr2 = pfn;

	cacheop_on_each_cpu(local_flush_cache_page, (void *)&data, 1);
}

void flush_cache_range(struct vm_area_struct *vma, unsigned long start,
		       unsigned long end)
{
	struct flusher_data data;

	data.vma = vma;
	data.addr1 = start;
	data.addr2 = end;

	cacheop_on_each_cpu(local_flush_cache_range, (void *)&data, 1);
}
EXPORT_SYMBOL(flush_cache_range);

void flush_dcache_page(struct page *page)
{
	cacheop_on_each_cpu(local_flush_dcache_page, page, 1);
}
EXPORT_SYMBOL(flush_dcache_page);

void flush_icache_range(unsigned long start, unsigned long end)
{
	struct flusher_data data;

	data.vma = NULL;
	data.addr1 = start;
	data.addr2 = end;

	cacheop_on_each_cpu(local_flush_icache_range, (void *)&data, 1);
}

void flush_icache_page(struct vm_area_struct *vma, struct page *page)
{
	/* Nothing uses the VMA, so just pass the struct page along */
	cacheop_on_each_cpu(local_flush_icache_page, page, 1);
}

void flush_cache_sigtramp(unsigned long address)
{
	cacheop_on_each_cpu(local_flush_cache_sigtramp, (void *)address, 1);
}

static void compute_alias(struct cache_info *c)
{
	c->alias_mask = ((c->sets - 1) << c->entry_shift) & ~(PAGE_SIZE - 1);
	c->n_aliases = c->alias_mask ? (c->alias_mask >> PAGE_SHIFT) + 1 : 0;
}

static void __init emit_cache_params(void)
{
	printk(KERN_NOTICE "I-cache : n_ways=%d n_sets=%d way_incr=%d\n",
		boot_cpu_data.icache.ways,
		boot_cpu_data.icache.sets,
		boot_cpu_data.icache.way_incr);
	printk(KERN_NOTICE "I-cache : entry_mask=0x%08x alias_mask=0x%08x n_aliases=%d\n",
		boot_cpu_data.icache.entry_mask,
		boot_cpu_data.icache.alias_mask,
		boot_cpu_data.icache.n_aliases);
	printk(KERN_NOTICE "D-cache : n_ways=%d n_sets=%d way_incr=%d\n",
		boot_cpu_data.dcache.ways,
		boot_cpu_data.dcache.sets,
		boot_cpu_data.dcache.way_incr);
	printk(KERN_NOTICE "D-cache : entry_mask=0x%08x alias_mask=0x%08x n_aliases=%d\n",
		boot_cpu_data.dcache.entry_mask,
		boot_cpu_data.dcache.alias_mask,
		boot_cpu_data.dcache.n_aliases);

	/*
	 * Emit Secondary Cache parameters if the CPU has a probed L2.
	 */
	if (boot_cpu_data.flags & CPU_HAS_L2_CACHE) {
		printk(KERN_NOTICE "S-cache : n_ways=%d n_sets=%d way_incr=%d\n",
			boot_cpu_data.scache.ways,
			boot_cpu_data.scache.sets,
			boot_cpu_data.scache.way_incr);
		printk(KERN_NOTICE "S-cache : entry_mask=0x%08x alias_mask=0x%08x n_aliases=%d\n",
			boot_cpu_data.scache.entry_mask,
			boot_cpu_data.scache.alias_mask,
			boot_cpu_data.scache.n_aliases);
	}
}

void __init cpu_cache_init(void)
{
	unsigned int cache_disabled = 0;

#ifdef CCR
	cache_disabled = !(__raw_readl(CCR) & CCR_CACHE_ENABLE);
#endif

	compute_alias(&boot_cpu_data.icache);
	compute_alias(&boot_cpu_data.dcache);
	compute_alias(&boot_cpu_data.scache);

	__flush_wback_region		= noop__flush_region;
	__flush_purge_region		= noop__flush_region;
	__flush_invalidate_region	= noop__flush_region;

	/*
	 * No flushing is necessary in the disabled cache case so we can
	 * just keep the noop functions in local_flush_..() and __flush_..()
	 */
	if (unlikely(cache_disabled))
		goto skip;

	if (boot_cpu_data.family == CPU_FAMILY_SH2) {
		extern void __weak sh2_cache_init(void);

		sh2_cache_init();
	}

	if (boot_cpu_data.family == CPU_FAMILY_SH2A) {
		extern void __weak sh2a_cache_init(void);

		sh2a_cache_init();
	}

	if (boot_cpu_data.family == CPU_FAMILY_SH3) {
		extern void __weak sh3_cache_init(void);

		sh3_cache_init();

		if ((boot_cpu_data.type == CPU_SH7705) &&
		    (boot_cpu_data.dcache.sets == 512)) {
			extern void __weak sh7705_cache_init(void);

			sh7705_cache_init();
		}
	}

	if ((boot_cpu_data.family == CPU_FAMILY_SH4) ||
	    (boot_cpu_data.family == CPU_FAMILY_SH4A) ||
	    (boot_cpu_data.family == CPU_FAMILY_SH4AL_DSP)) {
		extern void __weak sh4_cache_init(void);

		sh4_cache_init();

		if ((boot_cpu_data.type == CPU_SH7786) ||
		    (boot_cpu_data.type == CPU_SHX3)) {
			extern void __weak shx3_cache_init(void);

			shx3_cache_init();
		}
	}

	if (boot_cpu_data.family == CPU_FAMILY_SH5) {
		extern void __weak sh5_cache_init(void);

		sh5_cache_init();
	}

skip:
	emit_cache_params();
}
