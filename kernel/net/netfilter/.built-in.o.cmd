cmd_net/netfilter/built-in.o :=  arm-eabi-ld.bfd -EL    -r -o net/netfilter/built-in.o net/netfilter/netfilter.o net/netfilter/nfnetlink.o net/netfilter/nfnetlink_queue.o net/netfilter/nf_conntrack.o net/netfilter/nf_conntrack_proto_gre.o net/netfilter/nf_conntrack_proto_udplite.o net/netfilter/nf_conntrack_netlink.o net/netfilter/nf_conntrack_ftp.o net/netfilter/nf_conntrack_pptp.o net/netfilter/nf_conntrack_sip.o net/netfilter/nf_conntrack_tftp.o net/netfilter/x_tables.o net/netfilter/xt_tcpudp.o net/netfilter/xt_CLASSIFY.o net/netfilter/xt_NFQUEUE.o net/netfilter/xt_iprange.o net/netfilter/xt_length.o net/netfilter/xt_limit.o net/netfilter/xt_mac.o net/netfilter/xt_multiport.o net/netfilter/xt_owner.o net/netfilter/xt_physdev.o net/netfilter/xt_policy.o net/netfilter/xt_realm.o net/netfilter/xt_recent.o net/netfilter/xt_state.o net/netfilter/xt_string.o net/netfilter/xt_time.o 