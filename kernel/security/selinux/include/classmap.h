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

#define COMMON_FILE_SOCK_PERMS "ioctl", "read", "write", "create", \
    "getattr", "setattr", "lock", "relabelfrom", "relabelto", "append"

#define COMMON_FILE_PERMS COMMON_FILE_SOCK_PERMS, "unlink", "link", \
    "rename", "execute", "swapon", "quotaon", "mounton"

#define COMMON_SOCK_PERMS COMMON_FILE_SOCK_PERMS, "bind", "connect", \
    "listen", "accept", "getopt", "setopt", "shutdown", "recvfrom",  \
    "sendto", "recv_msg", "send_msg", "name_bind"

#define COMMON_IPC_PERMS "create", "destroy", "getattr", "setattr", "read", \
	    "write", "associate", "unix_read", "unix_write"

struct security_class_mapping secclass_map[] = {
	{ "security",
	  { "compute_av", "compute_create", "compute_member",
	    "check_context", "load_policy", "compute_relabel",
	    "compute_user", "setenforce", "setbool", "setsecparam",
	    "setcheckreqprot", NULL } },
	{ "process",
	  { "fork", "transition", "sigchld", "sigkill",
	    "sigstop", "signull", "signal", "ptrace", "getsched", "setsched",
	    "getsession", "getpgid", "setpgid", "getcap", "setcap", "share",
	    "getattr", "setexec", "setfscreate", "noatsecure", "siginh",
	    "setrlimit", "rlimitinh", "dyntransition", "setcurrent",
	    "execmem", "execstack", "execheap", "setkeycreate",
	    "setsockcreate", NULL } },
	{ "system",
	  { "ipc_info", "syslog_read", "syslog_mod",
	    "syslog_console", "module_request", NULL } },
	{ "capability",
	  { "chown", "dac_override", "dac_read_search",
	    "fowner", "fsetid", "kill", "setgid", "setuid", "setpcap",
	    "linux_immutable", "net_bind_service", "net_broadcast",
	    "net_admin", "net_raw", "ipc_lock", "ipc_owner", "sys_module",
	    "sys_rawio", "sys_chroot", "sys_ptrace", "sys_pacct", "sys_admin",
	    "sys_boot", "sys_nice", "sys_resource", "sys_time",
	    "sys_tty_config", "mknod", "lease", "audit_write",
	    "audit_control", "setfcap", NULL } },
	{ "filesystem",
	  { "mount", "remount", "unmount", "getattr",
	    "relabelfrom", "relabelto", "transition", "associate", "quotamod",
	    "quotaget", NULL } },
	{ "file",
	  { COMMON_FILE_PERMS,
	    "execute_no_trans", "entrypoint", "execmod", "open", NULL } },
	{ "dir",
	  { COMMON_FILE_PERMS, "add_name", "remove_name",
	    "reparent", "search", "rmdir", "open", NULL } },
	{ "fd", { "use", NULL } },
	{ "lnk_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "chr_file",
	  { COMMON_FILE_PERMS,
	    "execute_no_trans", "entrypoint", "execmod", "open", NULL } },
	{ "blk_file",
	  { COMMON_FILE_PERMS, "open", NULL } },
	{ "sock_file",
	  { COMMON_FILE_PERMS, "open", NULL } },
	{ "fifo_file",
	  { COMMON_FILE_PERMS, "open", NULL } },
	{ "socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "tcp_socket",
	  { COMMON_SOCK_PERMS,
	    "connectto", "newconn", "acceptfrom", "node_bind", "name_connect",
	    NULL } },
	{ "udp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "rawip_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "node",
	  { "tcp_recv", "tcp_send", "udp_recv", "udp_send",
	    "rawip_recv", "rawip_send", "enforce_dest",
	    "dccp_recv", "dccp_send", "recvfrom", "sendto", NULL } },
	{ "netif",
	  {  "tcp_recv", "tcp_send", "udp_recv", "udp_send",
	     "rawip_recv", "rawip_send", "dccp_recv", "dccp_send",
	     "ingress", "egress", NULL } },
	{ "netlink_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "packet_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "key_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "unix_stream_socket",
	  { COMMON_SOCK_PERMS, "connectto", "newconn", "acceptfrom", NULL
	  } },
	{ "unix_dgram_socket",
	  { COMMON_SOCK_PERMS, NULL
	  } },
	{ "sem",
	  { COMMON_IPC_PERMS, NULL } },
	{ "msg", { "send", "receive", NULL } },
	{ "msgq",
	  { COMMON_IPC_PERMS, "enqueue", NULL } },
	{ "shm",
	  { COMMON_IPC_PERMS, "lock", NULL } },
	{ "ipc",
	  { COMMON_IPC_PERMS, NULL } },
	{ "netlink_route_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_firewall_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_tcpdiag_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_nflog_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_xfrm_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_selinux_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_audit_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", "nlmsg_relay", "nlmsg_readpriv",
	    "nlmsg_tty_audit", NULL } },
	{ "netlink_ip6fw_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_dnrt_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "association",
	  { "sendto", "recvfrom", "setcontext", "polmatch", NULL } },
	{ "netlink_kobject_uevent_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "appletalk_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "packet",
	  { "send", "recv", "relabelto", "flow_in", "flow_out",
	    "forward_in", "forward_out", NULL } },
	{ "key",
	  { "view", "read", "write", "search", "link", "setattr", "create",
	    NULL } },
	{ "dccp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", "name_connect", NULL } },
	{ "memprotect", { "mmap_zero", NULL } },
	{ "peer", { "recv", NULL } },
	{ "capability2", { "mac_override", "mac_admin", NULL } },
	{ "kernel_service", { "use_as_override", "create_files_as", NULL } },
	{ "tun_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ NULL }
  };
