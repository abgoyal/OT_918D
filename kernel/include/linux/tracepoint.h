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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef _LINUX_TRACEPOINT_H
#define _LINUX_TRACEPOINT_H

/*
 * Kernel Tracepoint API.
 *
 * See Documentation/trace/tracepoints.txt.
 *
 * (C) Copyright 2008 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * Heavily inspired from the Linux Kernel Markers.
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#include <linux/immediate.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/rcupdate.h>

struct module;
struct tracepoint;

struct tracepoint_func {
	void *func;
	void *data;
};

struct tracepoint {
	const char *name;		/* Tracepoint name */
	DEFINE_IMV(char, state);	/* State. */
	void (*regfunc)(void);
	void (*unregfunc)(void);
	struct tracepoint_func *funcs;
} __attribute__((aligned(32)));		/*
					 * Aligned on 32 bytes because it is
					 * globally visible and gcc happily
					 * align these on the structure size.
					 * Keep in sync with vmlinux.lds.h.
					 */

/*
 * Connect a probe to a tracepoint.
 * Internal API, should not be used directly.
 */
extern int tracepoint_probe_register(const char *name, void *probe, void *data);

/*
 * Disconnect a probe from a tracepoint.
 * Internal API, should not be used directly.
 */
extern int
tracepoint_probe_unregister(const char *name, void *probe, void *data);

extern int tracepoint_probe_register_noupdate(const char *name, void *probe,
					      void *data);
extern int tracepoint_probe_unregister_noupdate(const char *name, void *probe,
						void *data);
extern void tracepoint_probe_update_all(void);

struct tracepoint_iter {
	struct module *module;
	struct tracepoint *tracepoint;
};

extern void tracepoint_iter_start(struct tracepoint_iter *iter);
extern void tracepoint_iter_next(struct tracepoint_iter *iter);
extern void tracepoint_iter_stop(struct tracepoint_iter *iter);
extern void tracepoint_iter_reset(struct tracepoint_iter *iter);
extern int tracepoint_get_iter_range(struct tracepoint **tracepoint,
	struct tracepoint *begin, struct tracepoint *end);

/*
 * tracepoint_synchronize_unregister must be called between the last tracepoint
 * probe unregistration and the end of module exit to make sure there is no
 * caller executing a probe when it is freed.
 */
static inline void tracepoint_synchronize_unregister(void)
{
	synchronize_sched();
}

#define PARAMS(args...) args

#ifdef CONFIG_TRACEPOINTS
extern void tracepoint_update_probe_range(struct tracepoint *begin,
	struct tracepoint *end);
#else
static inline void tracepoint_update_probe_range(struct tracepoint *begin,
	struct tracepoint *end)
{ }
#endif /* CONFIG_TRACEPOINTS */

#endif /* _LINUX_TRACEPOINT_H */

/*
 * Note: we keep the TRACE_EVENT and DECLARE_TRACE outside the include
 *  file ifdef protection.
 *  This is due to the way trace events work. If a file includes two
 *  trace event headers under one "CREATE_TRACE_POINTS" the first include
 *  will override the TRACE_EVENT and break the second include.
 */

#ifndef DECLARE_TRACE

#define TP_PROTO(args...)	args
#define TP_ARGS(args...)	args

#define DECLARE_TRACE_NOP(name, proto, args, data_proto, data_args)	\
	static inline void trace_##name(proto)				\
	{ }								\
	static inline void _trace_##name(proto)				\
	{ }								\
	static inline int register_trace_##name(void (*probe)(data_proto), \
						void *data)		\
	{								\
		return -ENOSYS;						\
	}								\
	static inline int unregister_trace_##name(void (*probe)(data_proto), \
						  void *data)		\
	{								\
		return -ENOSYS;						\
	}								\
	static inline void check_trace_callback_type_##name(void (*cb)(data_proto)) \
	{								\
	}

#define DEFINE_TRACE_FN_NOP(name, reg, unreg)
#define DEFINE_TRACE_NOP(name)
#define EXPORT_TRACEPOINT_SYMBOL_GPL_NOP(name)
#define EXPORT_TRACEPOINT_SYMBOL_NOP(name)

#ifdef CONFIG_TRACEPOINTS

/*
 * it_func[0] is never NULL because there is at least one element in the array
 * when the array itself is non NULL.
 *
 * Note, the proto and args passed in includes "__data" as the first parameter.
 * The reason for this is to handle the "void" prototype. If a tracepoint
 * has a "void" prototype, then it is invalid to declare a function
 * as "(void *, void)". The DECLARE_TRACE_NOARGS() will pass in just
 * "void *data", where as the DECLARE_TRACE() will pass in "void *data, proto".
 */
#define __DO_TRACE(tp, proto, args)					\
	do {								\
		struct tracepoint_func *it_func_ptr;			\
		void *it_func;						\
		void *__data;						\
									\
		rcu_read_lock_sched_notrace();				\
		it_func_ptr = rcu_dereference_sched((tp)->funcs);	\
		if (it_func_ptr) {					\
			do {						\
				it_func = (it_func_ptr)->func;		\
				__data = (it_func_ptr)->data;		\
				((void(*)(proto))(it_func))(args);	\
			} while ((++it_func_ptr)->func);		\
		}							\
		rcu_read_unlock_sched_notrace();			\
	} while (0)

#define __CHECK_TRACE(name, generic, proto, args)			\
	do {								\
		if (!generic) {						\
			if (unlikely(imv_read(__tracepoint_##name.state))) \
				__DO_TRACE(&__tracepoint_##name,	\
					TP_PROTO(proto), TP_ARGS(args));\
		} else {						\
			if (unlikely(_imv_read(__tracepoint_##name.state))) \
				__DO_TRACE(&__tracepoint_##name,	\
					TP_PROTO(proto), TP_ARGS(args));\
		}							\
	} while (0)

/*
 * Make sure the alignment of the structure in the __tracepoints section will
 * not add unwanted padding between the beginning of the section and the
 * structure. Force alignment to the same alignment as the section start.
 *
 * The "generic" argument, passed to the declared __trace_##name inline
 * function controls which tracepoint enabling mechanism must be used.
 * If generic is true, a variable read is used.
 * If generic is false, immediate values are used.
 */
#define __DECLARE_TRACE(name, proto, args, data_proto, data_args)	\
	extern struct tracepoint __tracepoint_##name;			\
	static inline void trace_##name(proto)				\
	{								\
		__CHECK_TRACE(name, 0, TP_PROTO(data_proto),		\
			      TP_ARGS(data_args));			\
	}								\
	static inline void _trace_##name(proto)				\
	{								\
		__CHECK_TRACE(name, 1, TP_PROTO(data_proto),		\
			      TP_ARGS(data_args));			\
	}								\
	static inline int						\
	register_trace_##name(void (*probe)(data_proto), void *data)	\
	{								\
		return tracepoint_probe_register(#name, (void *)probe,	\
						 data);			\
	}								\
	static inline int						\
	unregister_trace_##name(void (*probe)(data_proto), void *data)	\
	{								\
		return tracepoint_probe_unregister(#name, (void *)probe, \
						   data);		\
	}								\
	static inline void						\
	check_trace_callback_type_##name(void (*cb)(data_proto))	\
	{								\
	}

#define DEFINE_TRACE_FN(name, reg, unreg)				\
	static const char __tpstrtab_##name[]				\
	__attribute__((section("__tracepoints_strings"))) = #name;	\
	struct tracepoint __tracepoint_##name				\
	__attribute__((section("__tracepoints"), aligned(32))) =	\
		{ __tpstrtab_##name, 0, reg, unreg, NULL }

#define DEFINE_TRACE(name)						\
	DEFINE_TRACE_FN(name, NULL, NULL);

#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)				\
	EXPORT_SYMBOL_GPL(__tracepoint_##name)
#define EXPORT_TRACEPOINT_SYMBOL(name)					\
	EXPORT_SYMBOL(__tracepoint_##name)

#else /* !CONFIG_TRACEPOINTS */

#define DEFINE_TRACE_FN			DEFINE_TRACE_FN_NOP
#define DEFINE_TRACE			DECLARE_TRACE_NOP
#define EXPORT_TRACEPOINT_SYMBOL_GPL	EXPORT_TRACEPOINT_SYMBOL_GPL_NOP
#define EXPORT_TRACEPOINT_SYMBOL	EXPORT_TRACEPOINT_SYMBOL_NOP

#endif /* CONFIG_TRACEPOINTS */

/*
 * The need for the DECLARE_TRACE_NOARGS() is to handle the prototype
 * (void). "void" is a special value in a function prototype and can
 * not be combined with other arguments. Since the DECLARE_TRACE()
 * macro adds a data element at the beginning of the prototype,
 * we need a way to differentiate "(void *data, proto)" from
 * "(void *data, void)". The second prototype is invalid.
 *
 * DECLARE_TRACE_NOARGS() passes "void" as the tracepoint prototype
 * and "void *__data" as the callback prototype.
 *
 * DECLARE_TRACE() passes "proto" as the tracepoint protoype and
 * "void *__data, proto" as the callback prototype.
 */
#define DECLARE_TRACE_NOARGS(name)					\
		__DECLARE_TRACE(name, void, , void *__data, __data)

#define DECLARE_TRACE(name, proto, args)				\
		__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),	\
				PARAMS(void *__data, proto),		\
				PARAMS(__data, args))

#endif /* DECLARE_TRACE */

#ifndef TRACE_EVENT
/*
 * For use with the TRACE_EVENT macro:
 *
 * We define a tracepoint, its arguments, its printk format
 * and its 'fast binay record' layout.
 *
 * Firstly, name your tracepoint via TRACE_EVENT(name : the
 * 'subsystem_event' notation is fine.
 *
 * Think about this whole construct as the
 * 'trace_sched_switch() function' from now on.
 *
 *
 *  TRACE_EVENT(sched_switch,
 *
 *	*
 *	* A function has a regular function arguments
 *	* prototype, declare it via TP_PROTO():
 *	*
 *
 *	TP_PROTO(struct rq *rq, struct task_struct *prev,
 *		 struct task_struct *next),
 *
 *	*
 *	* Define the call signature of the 'function'.
 *	* (Design sidenote: we use this instead of a
 *	*  TP_PROTO1/TP_PROTO2/TP_PROTO3 ugliness.)
 *	*
 *
 *	TP_ARGS(rq, prev, next),
 *
 *	*
 *	* Fast binary tracing: define the trace record via
 *	* TP_STRUCT__entry(). You can think about it like a
 *	* regular C structure local variable definition.
 *	*
 *	* This is how the trace record is structured and will
 *	* be saved into the ring buffer. These are the fields
 *	* that will be exposed to user-space in
 *	* /sys/kernel/debug/tracing/events/<*>/format.
 *	*
 *	* The declared 'local variable' is called '__entry'
 *	*
 *	* __field(pid_t, prev_prid) is equivalent to a standard declariton:
 *	*
 *	*	pid_t	prev_pid;
 *	*
 *	* __array(char, prev_comm, TASK_COMM_LEN) is equivalent to:
 *	*
 *	*	char	prev_comm[TASK_COMM_LEN];
 *	*
 *
 *	TP_STRUCT__entry(
 *		__array(	char,	prev_comm,	TASK_COMM_LEN	)
 *		__field(	pid_t,	prev_pid			)
 *		__field(	int,	prev_prio			)
 *		__array(	char,	next_comm,	TASK_COMM_LEN	)
 *		__field(	pid_t,	next_pid			)
 *		__field(	int,	next_prio			)
 *	),
 *
 *	*
 *	* Assign the entry into the trace record, by embedding
 *	* a full C statement block into TP_fast_assign(). You
 *	* can refer to the trace record as '__entry' -
 *	* otherwise you can put arbitrary C code in here.
 *	*
 *	* Note: this C code will execute every time a trace event
 *	* happens, on an active tracepoint.
 *	*
 *
 *	TP_fast_assign(
 *		memcpy(__entry->next_comm, next->comm, TASK_COMM_LEN);
 *		__entry->prev_pid	= prev->pid;
 *		__entry->prev_prio	= prev->prio;
 *		memcpy(__entry->prev_comm, prev->comm, TASK_COMM_LEN);
 *		__entry->next_pid	= next->pid;
 *		__entry->next_prio	= next->prio;
 *	)
 *
 *	*
 *	* Formatted output of a trace record via TP_printk().
 *	* This is how the tracepoint will appear under ftrace
 *	* plugins that make use of this tracepoint.
 *	*
 *	* (raw-binary tracing wont actually perform this step.)
 *	*
 *
 *	TP_printk("task %s:%d [%d] ==> %s:%d [%d]",
 *		__entry->prev_comm, __entry->prev_pid, __entry->prev_prio,
 *		__entry->next_comm, __entry->next_pid, __entry->next_prio),
 *
 * );
 *
 * This macro construct is thus used for the regular printk format
 * tracing setup, it is used to construct a function pointer based
 * tracepoint callback (this is used by programmatic plugins and
 * can also by used by generic instrumentation like SystemTap), and
 * it is also used to expose a structured trace record in
 * /sys/kernel/debug/tracing/events/.
 *
 * A set of (un)registration functions can be passed to the variant
 * TRACE_EVENT_FN to perform any (un)registration work.
 */

#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)
#define DEFINE_EVENT(template, name, proto, args)		\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define DEFINE_EVENT_PRINT(template, name, proto, args, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))

#define TRACE_EVENT(name, proto, args, struct, assign, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define TRACE_EVENT_FN(name, proto, args, struct,		\
		assign, print, reg, unreg)			\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))

#endif /* ifdef TRACE_EVENT (see note above) */