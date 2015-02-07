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
 * Copyright (C) 2009-2010 PetaLogix
 * Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corporation
 *
 * Provide default implementations of the DMA mapping callbacks for
 * directly mapped busses.
 */

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/gfp.h>
#include <linux/dma-debug.h>
#include <asm/bug.h>
#include <asm/cacheflush.h>

/*
 * Generic direct DMA implementation
 *
 * This implementation supports a per-device offset that can be applied if
 * the address at which memory is visible to devices is not 0. Platform code
 * can set archdata.dma_data to an unsigned long holding the offset. By
 * default the offset is PCI_DRAM_OFFSET.
 */
static inline void __dma_sync_page(unsigned long paddr, unsigned long offset,
				size_t size, enum dma_data_direction direction)
{
	switch (direction) {
	case DMA_TO_DEVICE:
		flush_dcache_range(paddr + offset, paddr + offset + size);
		break;
	case DMA_FROM_DEVICE:
		invalidate_dcache_range(paddr + offset, paddr + offset + size);
		break;
	default:
		BUG();
	}
}

static unsigned long get_dma_direct_offset(struct device *dev)
{
	if (likely(dev))
		return (unsigned long)dev->archdata.dma_data;

	return PCI_DRAM_OFFSET; /* FIXME Not sure if is correct */
}

#define NOT_COHERENT_CACHE

static void *dma_direct_alloc_coherent(struct device *dev, size_t size,
				dma_addr_t *dma_handle, gfp_t flag)
{
#ifdef NOT_COHERENT_CACHE
	return consistent_alloc(flag, size, dma_handle);
#else
	void *ret;
	struct page *page;
	int node = dev_to_node(dev);

	/* ignore region specifiers */
	flag  &= ~(__GFP_HIGHMEM);

	page = alloc_pages_node(node, flag, get_order(size));
	if (page == NULL)
		return NULL;
	ret = page_address(page);
	memset(ret, 0, size);
	*dma_handle = virt_to_phys(ret) + get_dma_direct_offset(dev);

	return ret;
#endif
}

static void dma_direct_free_coherent(struct device *dev, size_t size,
			      void *vaddr, dma_addr_t dma_handle)
{
#ifdef NOT_COHERENT_CACHE
	consistent_free(size, vaddr);
#else
	free_pages((unsigned long)vaddr, get_order(size));
#endif
}

static int dma_direct_map_sg(struct device *dev, struct scatterlist *sgl,
			     int nents, enum dma_data_direction direction,
			     struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	int i;

	/* FIXME this part of code is untested */
	for_each_sg(sgl, sg, nents, i) {
		sg->dma_address = sg_phys(sg) + get_dma_direct_offset(dev);
		__dma_sync_page(page_to_phys(sg_page(sg)), sg->offset,
							sg->length, direction);
	}

	return nents;
}

static void dma_direct_unmap_sg(struct device *dev, struct scatterlist *sg,
				int nents, enum dma_data_direction direction,
				struct dma_attrs *attrs)
{
}

static int dma_direct_dma_supported(struct device *dev, u64 mask)
{
	return 1;
}

static inline dma_addr_t dma_direct_map_page(struct device *dev,
					     struct page *page,
					     unsigned long offset,
					     size_t size,
					     enum dma_data_direction direction,
					     struct dma_attrs *attrs)
{
	__dma_sync_page(page_to_phys(page), offset, size, direction);
	return page_to_phys(page) + offset + get_dma_direct_offset(dev);
}

static inline void dma_direct_unmap_page(struct device *dev,
					 dma_addr_t dma_address,
					 size_t size,
					 enum dma_data_direction direction,
					 struct dma_attrs *attrs)
{
/* There is not necessary to do cache cleanup
 *
 * phys_to_virt is here because in __dma_sync_page is __virt_to_phys and
 * dma_address is physical address
 */
	__dma_sync_page(dma_address, 0 , size, direction);
}

struct dma_map_ops dma_direct_ops = {
	.alloc_coherent	= dma_direct_alloc_coherent,
	.free_coherent	= dma_direct_free_coherent,
	.map_sg		= dma_direct_map_sg,
	.unmap_sg	= dma_direct_unmap_sg,
	.dma_supported	= dma_direct_dma_supported,
	.map_page	= dma_direct_map_page,
	.unmap_page	= dma_direct_unmap_page,
};
EXPORT_SYMBOL(dma_direct_ops);

/* Number of entries preallocated for DMA-API debugging */
#define PREALLOC_DMA_DEBUG_ENTRIES (1 << 16)

static int __init dma_init(void)
{
       dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);

       return 0;
}
fs_initcall(dma_init);
