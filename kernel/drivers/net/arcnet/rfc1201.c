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
 * Linux ARCnet driver - RFC1201 (standard) packet encapsulation
 * 
 * Written 1994-1999 by Avery Pennarun.
 * Derived from skeleton.c by Donald Becker.
 *
 * Special thanks to Contemporary Controls, Inc. (www.ccontrols.com)
 *  for sponsoring the further development of this driver.
 *
 * **********************
 *
 * The original copyright of skeleton.c was as follows:
 *
 * skeleton.c Written 1993 by Donald Becker.
 * Copyright 1993 United States Government as represented by the
 * Director, National Security Agency.  This software may only be used
 * and distributed according to the terms of the GNU General Public License as
 * modified by SRC, incorporated herein by reference.
 *
 * **********************
 *
 * For more details, see drivers/net/arcnet.c
 *
 * **********************
 */
#include <linux/gfp.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/arcdevice.h>

MODULE_LICENSE("GPL");
#define VERSION "arcnet: RFC1201 \"standard\" (`a') encapsulation support loaded.\n"


static __be16 type_trans(struct sk_buff *skb, struct net_device *dev);
static void rx(struct net_device *dev, int bufnum,
	       struct archdr *pkthdr, int length);
static int build_header(struct sk_buff *skb, struct net_device *dev,
			unsigned short type, uint8_t daddr);
static int prepare_tx(struct net_device *dev, struct archdr *pkt, int length,
		      int bufnum);
static int continue_tx(struct net_device *dev, int bufnum);

static struct ArcProto rfc1201_proto =
{
	.suffix		= 'a',
	.mtu		= 1500,	/* could be more, but some receivers can't handle it... */
	.is_ip          = 1,    /* This is for sending IP and ARP packages */
	.rx		= rx,
	.build_header	= build_header,
	.prepare_tx	= prepare_tx,
	.continue_tx	= continue_tx,
	.ack_tx         = NULL
};


static int __init arcnet_rfc1201_init(void)
{
	printk(VERSION);

	arc_proto_map[ARC_P_IP]
	    = arc_proto_map[ARC_P_IPV6]
	    = arc_proto_map[ARC_P_ARP]
	    = arc_proto_map[ARC_P_RARP]
	    = arc_proto_map[ARC_P_IPX]
	    = arc_proto_map[ARC_P_NOVELL_EC]
	    = &rfc1201_proto;

	/* if someone else already owns the broadcast, we won't take it */
	if (arc_bcast_proto == arc_proto_default)
		arc_bcast_proto = &rfc1201_proto;

	return 0;
}

static void __exit arcnet_rfc1201_exit(void)
{
	arcnet_unregister_proto(&rfc1201_proto);
}

module_init(arcnet_rfc1201_init);
module_exit(arcnet_rfc1201_exit);

/*
 * Determine a packet's protocol ID.
 * 
 * With ARCnet we have to convert everything to Ethernet-style stuff.
 */
static __be16 type_trans(struct sk_buff *skb, struct net_device *dev)
{
	struct archdr *pkt = (struct archdr *) skb->data;
	struct arc_rfc1201 *soft = &pkt->soft.rfc1201;
	int hdr_size = ARC_HDR_SIZE + RFC1201_HDR_SIZE;

	/* Pull off the arcnet header. */
	skb_reset_mac_header(skb);
	skb_pull(skb, hdr_size);

	if (pkt->hard.dest == 0)
		skb->pkt_type = PACKET_BROADCAST;
	else if (dev->flags & IFF_PROMISC) {
		/* if we're not sending to ourselves :) */
		if (pkt->hard.dest != dev->dev_addr[0])
			skb->pkt_type = PACKET_OTHERHOST;
	}
	/* now return the protocol number */
	switch (soft->proto) {
	case ARC_P_IP:
		return htons(ETH_P_IP);
	case ARC_P_IPV6:
		return htons(ETH_P_IPV6);
	case ARC_P_ARP:
		return htons(ETH_P_ARP);
	case ARC_P_RARP:
		return htons(ETH_P_RARP);

	case ARC_P_IPX:
	case ARC_P_NOVELL_EC:
		return htons(ETH_P_802_3);
	default:
		dev->stats.rx_errors++;
		dev->stats.rx_crc_errors++;
		return 0;
	}

	return htons(ETH_P_IP);
}


/* packet receiver */
static void rx(struct net_device *dev, int bufnum,
	       struct archdr *pkthdr, int length)
{
	struct arcnet_local *lp = netdev_priv(dev);
	struct sk_buff *skb;
	struct archdr *pkt = pkthdr;
	struct arc_rfc1201 *soft = &pkthdr->soft.rfc1201;
	int saddr = pkt->hard.source, ofs;
	struct Incoming *in = &lp->rfc1201.incoming[saddr];

	BUGMSG(D_DURING, "it's an RFC1201 packet (length=%d)\n", length);

	if (length >= MinTU)
		ofs = 512 - length;
	else
		ofs = 256 - length;

	if (soft->split_flag == 0xFF) {		/* Exception Packet */
		if (length >= 4 + RFC1201_HDR_SIZE)
			BUGMSG(D_DURING, "compensating for exception packet\n");
		else {
			BUGMSG(D_EXTRA, "short RFC1201 exception packet from %02Xh",
			       saddr);
			return;
		}

		/* skip over 4-byte junkola */
		length -= 4;
		ofs += 4;
		lp->hw.copy_from_card(dev, bufnum, 512 - length,
				      soft, sizeof(pkt->soft));
	}
	if (!soft->split_flag) {	/* not split */
		BUGMSG(D_RX, "incoming is not split (splitflag=%d)\n",
		       soft->split_flag);

		if (in->skb) {	/* already assembling one! */
			BUGMSG(D_EXTRA, "aborting assembly (seq=%d) for unsplit packet (splitflag=%d, seq=%d)\n",
			 in->sequence, soft->split_flag, soft->sequence);
			lp->rfc1201.aborted_seq = soft->sequence;
			dev_kfree_skb_irq(in->skb);
			dev->stats.rx_errors++;
			dev->stats.rx_missed_errors++;
			in->skb = NULL;
		}
		in->sequence = soft->sequence;

		skb = alloc_skb(length + ARC_HDR_SIZE, GFP_ATOMIC);
		if (skb == NULL) {
			BUGMSG(D_NORMAL, "Memory squeeze, dropping packet.\n");
			dev->stats.rx_dropped++;
			return;
		}
		skb_put(skb, length + ARC_HDR_SIZE);
		skb->dev = dev;

		pkt = (struct archdr *) skb->data;
		soft = &pkt->soft.rfc1201;

		/* up to sizeof(pkt->soft) has already been copied from the card */
		memcpy(pkt, pkthdr, sizeof(struct archdr));
		if (length > sizeof(pkt->soft))
			lp->hw.copy_from_card(dev, bufnum, ofs + sizeof(pkt->soft),
				       pkt->soft.raw + sizeof(pkt->soft),
					      length - sizeof(pkt->soft));

		/*
		 * ARP packets have problems when sent from some DOS systems: the
		 * source address is always 0!  So we take the hardware source addr
		 * (which is impossible to fumble) and insert it ourselves.
		 */
		if (soft->proto == ARC_P_ARP) {
			struct arphdr *arp = (struct arphdr *) soft->payload;

			/* make sure addresses are the right length */
			if (arp->ar_hln == 1 && arp->ar_pln == 4) {
				uint8_t *cptr = (uint8_t *) arp + sizeof(struct arphdr);

				if (!*cptr) {	/* is saddr = 00? */
					BUGMSG(D_EXTRA,
					       "ARP source address was 00h, set to %02Xh.\n",
					       saddr);
					dev->stats.rx_crc_errors++;
					*cptr = saddr;
				} else {
					BUGMSG(D_DURING, "ARP source address (%Xh) is fine.\n",
					       *cptr);
				}
			} else {
				BUGMSG(D_NORMAL, "funny-shaped ARP packet. (%Xh, %Xh)\n",
				       arp->ar_hln, arp->ar_pln);
				dev->stats.rx_errors++;
				dev->stats.rx_crc_errors++;
			}
		}
		BUGLVL(D_SKB) arcnet_dump_skb(dev, skb, "rx");

		skb->protocol = type_trans(skb, dev);
		netif_rx(skb);
	} else {		/* split packet */
		/*
		 * NOTE: MSDOS ARP packet correction should only need to apply to
		 * unsplit packets, since ARP packets are so short.
		 *
		 * My interpretation of the RFC1201 document is that if a packet is
		 * received out of order, the entire assembly process should be
		 * aborted.
		 *
		 * The RFC also mentions "it is possible for successfully received
		 * packets to be retransmitted." As of 0.40 all previously received
		 * packets are allowed, not just the most recent one.
		 *
		 * We allow multiple assembly processes, one for each ARCnet card
		 * possible on the network.  Seems rather like a waste of memory,
		 * but there's no other way to be reliable.
		 */

		BUGMSG(D_RX, "packet is split (splitflag=%d, seq=%d)\n",
		       soft->split_flag, in->sequence);

		if (in->skb && in->sequence != soft->sequence) {
			BUGMSG(D_EXTRA, "wrong seq number (saddr=%d, expected=%d, seq=%d, splitflag=%d)\n",
			       saddr, in->sequence, soft->sequence,
			       soft->split_flag);
			dev_kfree_skb_irq(in->skb);
			in->skb = NULL;
			dev->stats.rx_errors++;
			dev->stats.rx_missed_errors++;
			in->lastpacket = in->numpackets = 0;
		}
		if (soft->split_flag & 1) {	/* first packet in split */
			BUGMSG(D_RX, "brand new splitpacket (splitflag=%d)\n",
			       soft->split_flag);
			if (in->skb) {	/* already assembling one! */
				BUGMSG(D_EXTRA, "aborting previous (seq=%d) assembly "
				       "(splitflag=%d, seq=%d)\n",
				       in->sequence, soft->split_flag,
				       soft->sequence);
				dev->stats.rx_errors++;
				dev->stats.rx_missed_errors++;
				dev_kfree_skb_irq(in->skb);
			}
			in->sequence = soft->sequence;
			in->numpackets = ((unsigned) soft->split_flag >> 1) + 2;
			in->lastpacket = 1;

			if (in->numpackets > 16) {
				BUGMSG(D_EXTRA, "incoming packet more than 16 segments; dropping. (splitflag=%d)\n",
				       soft->split_flag);
				lp->rfc1201.aborted_seq = soft->sequence;
				dev->stats.rx_errors++;
				dev->stats.rx_length_errors++;
				return;
			}
			in->skb = skb = alloc_skb(508 * in->numpackets + ARC_HDR_SIZE,
						  GFP_ATOMIC);
			if (skb == NULL) {
				BUGMSG(D_NORMAL, "(split) memory squeeze, dropping packet.\n");
				lp->rfc1201.aborted_seq = soft->sequence;
				dev->stats.rx_dropped++;
				return;
			}
			skb->dev = dev;
			pkt = (struct archdr *) skb->data;
			soft = &pkt->soft.rfc1201;

			memcpy(pkt, pkthdr, ARC_HDR_SIZE + RFC1201_HDR_SIZE);
			skb_put(skb, ARC_HDR_SIZE + RFC1201_HDR_SIZE);

			soft->split_flag = 0;	/* end result won't be split */
		} else {	/* not first packet */
			int packetnum = ((unsigned) soft->split_flag >> 1) + 1;

			/*
			 * if we're not assembling, there's no point trying to
			 * continue.
			 */
			if (!in->skb) {
				if (lp->rfc1201.aborted_seq != soft->sequence) {
					BUGMSG(D_EXTRA, "can't continue split without starting "
					       "first! (splitflag=%d, seq=%d, aborted=%d)\n",
					soft->split_flag, soft->sequence,
					       lp->rfc1201.aborted_seq);
					dev->stats.rx_errors++;
					dev->stats.rx_missed_errors++;
				}
				return;
			}
			in->lastpacket++;
			if (packetnum != in->lastpacket) {	/* not the right flag! */
				/* harmless duplicate? ignore. */
				if (packetnum <= in->lastpacket - 1) {
					BUGMSG(D_EXTRA, "duplicate splitpacket ignored! (splitflag=%d)\n",
					       soft->split_flag);
					dev->stats.rx_errors++;
					dev->stats.rx_frame_errors++;
					return;
				}
				/* "bad" duplicate, kill reassembly */
				BUGMSG(D_EXTRA, "out-of-order splitpacket, reassembly "
				       "(seq=%d) aborted (splitflag=%d, seq=%d)\n",
				       in->sequence, soft->split_flag, soft->sequence);
				lp->rfc1201.aborted_seq = soft->sequence;
				dev_kfree_skb_irq(in->skb);
				in->skb = NULL;
				dev->stats.rx_errors++;
				dev->stats.rx_missed_errors++;
				in->lastpacket = in->numpackets = 0;
				return;
			}
			pkt = (struct archdr *) in->skb->data;
			soft = &pkt->soft.rfc1201;
		}

		skb = in->skb;

		lp->hw.copy_from_card(dev, bufnum, ofs + RFC1201_HDR_SIZE,
				      skb->data + skb->len,
				      length - RFC1201_HDR_SIZE);
		skb_put(skb, length - RFC1201_HDR_SIZE);

		/* are we done? */
		if (in->lastpacket == in->numpackets) {
			in->skb = NULL;
			in->lastpacket = in->numpackets = 0;

	    BUGMSG(D_SKB_SIZE, "skb: received %d bytes from %02X (unsplit)\n",
    		skb->len, pkt->hard.source);
	    BUGMSG(D_SKB_SIZE, "skb: received %d bytes from %02X (split)\n",
    		skb->len, pkt->hard.source);
			BUGLVL(D_SKB) arcnet_dump_skb(dev, skb, "rx");

			skb->protocol = type_trans(skb, dev);
			netif_rx(skb);
		}
	}
}


/* Create the ARCnet hard/soft headers for RFC1201. */
static int build_header(struct sk_buff *skb, struct net_device *dev,
			unsigned short type, uint8_t daddr)
{
	struct arcnet_local *lp = netdev_priv(dev);
	int hdr_size = ARC_HDR_SIZE + RFC1201_HDR_SIZE;
	struct archdr *pkt = (struct archdr *) skb_push(skb, hdr_size);
	struct arc_rfc1201 *soft = &pkt->soft.rfc1201;

	/* set the protocol ID according to RFC1201 */
	switch (type) {
	case ETH_P_IP:
		soft->proto = ARC_P_IP;
		break;
	case ETH_P_IPV6:
		soft->proto = ARC_P_IPV6;
		break;
	case ETH_P_ARP:
		soft->proto = ARC_P_ARP;
		break;
	case ETH_P_RARP:
		soft->proto = ARC_P_RARP;
		break;
	case ETH_P_IPX:
	case ETH_P_802_3:
	case ETH_P_802_2:
		soft->proto = ARC_P_IPX;
		break;
	case ETH_P_ATALK:
		soft->proto = ARC_P_ATALK;
		break;
	default:
		BUGMSG(D_NORMAL, "RFC1201: I don't understand protocol %d (%Xh)\n",
		       type, type);
		dev->stats.tx_errors++;
		dev->stats.tx_aborted_errors++;
		return 0;
	}

	/*
	 * Set the source hardware address.
	 *
	 * This is pretty pointless for most purposes, but it can help in
	 * debugging.  ARCnet does not allow us to change the source address in
	 * the actual packet sent)
	 */
	pkt->hard.source = *dev->dev_addr;

	soft->sequence = htons(lp->rfc1201.sequence++);
	soft->split_flag = 0;	/* split packets are done elsewhere */

	/* see linux/net/ethernet/eth.c to see where I got the following */

	if (dev->flags & (IFF_LOOPBACK | IFF_NOARP)) {
		/* 
		 * FIXME: fill in the last byte of the dest ipaddr here to better
		 * comply with RFC1051 in "noarp" mode.  For now, always broadcasting
		 * will probably at least get packets sent out :)
		 */
		pkt->hard.dest = 0;
		return hdr_size;
	}
	/* otherwise, drop in the dest address */
	pkt->hard.dest = daddr;
	return hdr_size;
}


static void load_pkt(struct net_device *dev, struct arc_hardware *hard,
		     struct arc_rfc1201 *soft, int softlen, int bufnum)
{
	struct arcnet_local *lp = netdev_priv(dev);
	int ofs;

	/* assume length <= XMTU: someone should have handled that by now. */

	if (softlen > MinTU) {
		hard->offset[0] = 0;
		hard->offset[1] = ofs = 512 - softlen;
	} else if (softlen > MTU) {	/* exception packet - add an extra header */
		struct arc_rfc1201 excsoft;

		excsoft.proto = soft->proto;
		excsoft.split_flag = 0xff;
		excsoft.sequence = htons(0xffff);

		hard->offset[0] = 0;
		ofs = 512 - softlen;
		hard->offset[1] = ofs - RFC1201_HDR_SIZE;
		lp->hw.copy_to_card(dev, bufnum, ofs - RFC1201_HDR_SIZE,
				    &excsoft, RFC1201_HDR_SIZE);
	} else
		hard->offset[0] = ofs = 256 - softlen;

	lp->hw.copy_to_card(dev, bufnum, 0, hard, ARC_HDR_SIZE);
	lp->hw.copy_to_card(dev, bufnum, ofs, soft, softlen);

	lp->lastload_dest = hard->dest;
}


static int prepare_tx(struct net_device *dev, struct archdr *pkt, int length,
		      int bufnum)
{
	struct arcnet_local *lp = netdev_priv(dev);
	const int maxsegsize = XMTU - RFC1201_HDR_SIZE;
	struct Outgoing *out;


	BUGMSG(D_DURING, "prepare_tx: txbufs=%d/%d/%d\n",
	       lp->next_tx, lp->cur_tx, bufnum);

	length -= ARC_HDR_SIZE;	/* hard header is not included in packet length */
	pkt->soft.rfc1201.split_flag = 0;

	/* need to do a split packet? */
	if (length > XMTU) {
		out = &lp->outgoing;

		out->length = length - RFC1201_HDR_SIZE;
		out->dataleft = lp->outgoing.length;
		out->numsegs = (out->dataleft + maxsegsize - 1) / maxsegsize;
		out->segnum = 0;

		BUGMSG(D_DURING, "rfc1201 prep_tx: ready for %d-segment split "
		       "(%d bytes, seq=%d)\n", out->numsegs, out->length,
		       pkt->soft.rfc1201.sequence);

		return 0;	/* not done */
	}
	/* just load the packet into the buffers and send it off */
	load_pkt(dev, &pkt->hard, &pkt->soft.rfc1201, length, bufnum);

	return 1;		/* done */
}


static int continue_tx(struct net_device *dev, int bufnum)
{
	struct arcnet_local *lp = netdev_priv(dev);
	struct Outgoing *out = &lp->outgoing;
	struct arc_hardware *hard = &out->pkt->hard;
	struct arc_rfc1201 *soft = &out->pkt->soft.rfc1201, *newsoft;
	int maxsegsize = XMTU - RFC1201_HDR_SIZE;
	int seglen;

	BUGMSG(D_DURING,
	  "rfc1201 continue_tx: loading segment %d(+1) of %d (seq=%d)\n",
	       out->segnum, out->numsegs, soft->sequence);

	/* the "new" soft header comes right before the data chunk */
	newsoft = (struct arc_rfc1201 *)
	    (out->pkt->soft.raw + out->length - out->dataleft);

	if (!out->segnum)	/* first packet; newsoft == soft */
		newsoft->split_flag = ((out->numsegs - 2) << 1) | 1;
	else {
		newsoft->split_flag = out->segnum << 1;
		newsoft->proto = soft->proto;
		newsoft->sequence = soft->sequence;
	}

	seglen = maxsegsize;
	if (seglen > out->dataleft)
		seglen = out->dataleft;
	out->dataleft -= seglen;

	load_pkt(dev, hard, newsoft, seglen + RFC1201_HDR_SIZE, bufnum);

	out->segnum++;
	if (out->segnum >= out->numsegs)
		return 1;
	else
		return 0;
}
