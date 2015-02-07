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

#ifndef XEN_OPS_H
#define XEN_OPS_H

#include <linux/init.h>
#include <linux/clocksource.h>
#include <linux/irqreturn.h>
#include <xen/xen-ops.h>

/* These are code, but not functions.  Defined in entry.S */
extern const char xen_hypervisor_callback[];
extern const char xen_failsafe_callback[];

extern void *xen_initial_gdt;

struct trap_info;
void xen_copy_trap_info(struct trap_info *traps);

DECLARE_PER_CPU(struct vcpu_info, xen_vcpu_info);
DECLARE_PER_CPU(unsigned long, xen_cr3);
DECLARE_PER_CPU(unsigned long, xen_current_cr3);

extern struct start_info *xen_start_info;
extern struct shared_info xen_dummy_shared_info;
extern struct shared_info *HYPERVISOR_shared_info;

void xen_setup_mfn_list_list(void);
void xen_setup_shared_info(void);
void xen_build_mfn_list_list(void);
void xen_setup_machphys_mapping(void);
pgd_t *xen_setup_kernel_pagetable(pgd_t *pgd, unsigned long max_pfn);
void xen_ident_map_ISA(void);
void xen_reserve_top(void);

char * __init xen_memory_setup(void);
void __init xen_arch_setup(void);
void __init xen_init_IRQ(void);
void xen_enable_sysenter(void);
void xen_enable_syscall(void);
void xen_vcpu_restore(void);

void __init xen_build_dynamic_phys_to_machine(void);

void xen_init_irq_ops(void);
void xen_setup_timer(int cpu);
void xen_setup_runstate_info(int cpu);
void xen_teardown_timer(int cpu);
cycle_t xen_clocksource_read(void);
void xen_setup_cpu_clockevents(void);
unsigned long xen_tsc_khz(void);
void __init xen_time_init(void);
unsigned long xen_get_wallclock(void);
int xen_set_wallclock(unsigned long time);
unsigned long long xen_sched_clock(void);

irqreturn_t xen_debug_interrupt(int irq, void *dev_id);

bool xen_vcpu_stolen(int vcpu);

void xen_setup_vcpu_info_placement(void);

#ifdef CONFIG_SMP
void xen_smp_init(void);

extern cpumask_var_t xen_cpu_initialized_map;
#else
static inline void xen_smp_init(void) {}
#endif

#ifdef CONFIG_PARAVIRT_SPINLOCKS
void __init xen_init_spinlocks(void);
__cpuinit void xen_init_lock_cpu(int cpu);
void xen_uninit_lock_cpu(int cpu);
#else
static inline void xen_init_spinlocks(void)
{
}
static inline void xen_init_lock_cpu(int cpu)
{
}
static inline void xen_uninit_lock_cpu(int cpu)
{
}
#endif

/* Declare an asm function, along with symbols needed to make it
   inlineable */
#define DECL_ASM(ret, name, ...)		\
	ret name(__VA_ARGS__);			\
	extern char name##_end[];		\
	extern char name##_reloc[]		\

DECL_ASM(void, xen_irq_enable_direct, void);
DECL_ASM(void, xen_irq_disable_direct, void);
DECL_ASM(unsigned long, xen_save_fl_direct, void);
DECL_ASM(void, xen_restore_fl_direct, unsigned long);

/* These are not functions, and cannot be called normally */
void xen_iret(void);
void xen_sysexit(void);
void xen_sysret32(void);
void xen_sysret64(void);
void xen_adjust_exception_frame(void);

#endif /* XEN_OPS_H */
