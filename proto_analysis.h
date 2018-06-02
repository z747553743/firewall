#ifndef __PROTO_ANALYSIS_H__
#define __PROTO_ANALYSIS_H__
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

struct proto_info {
	unsigned char proto;
	unsigned int s_ip;
	unsigned short s_port;
	unsigned int d_ip;
	unsigned short d_port;
};

void get_protocol(struct sk_buff *skb, struct proto_info * proto){
	struct iphdr * iph = ip_hdr(skb);
	struct tcphdr * tcph = NULL;
	struct udphdr * udph = NULL;
	proto->proto = iph->protocol;
	printk(KERN_ALERT "analysis %d proto\n",iph->protocol);
	proto->s_ip = iph->saddr;
	proto->d_ip = iph->daddr;
	
	switch(iph->protocol){
		case IPPROTO_TCP:
			tcph = (struct tcphdr *) (iph + iph->ihl * 4);
			proto->s_port = tcph->source;
			proto->d_port = tcph->dest;
			break;
		case IPPROTO_UDP:
			udph = (struct udphdr *) (iph + iph->ihl * 4);
			proto->s_port = udph->source;
			proto->d_port = udph->dest;
			break;
	}
}




#endif
