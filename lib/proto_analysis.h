/* 
	解析IP包中IP地址、协议、以及端口的相关结构和函数
	Author: ZengLiangwei
	Email: 747553743@qq.com
 */
#ifndef __PROTO_ANALYSIS_H__
#define __PROTO_ANALYSIS_H__
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>


struct proto_info 
{
	unsigned char proto;
	unsigned int s_addr;
	unsigned short s_port;
	unsigned int d_addr;
	unsigned short d_port;
};

/* 解析sk_buff中的协议信息 */
void get_protocol(struct sk_buff *skb, struct proto_info * proto)
{
	struct iphdr * iph = ip_hdr(skb);
	struct tcphdr * tcph = NULL;
	struct udphdr * udph = NULL;
	proto->proto = iph->protocol;
	proto->s_addr = ntohl(iph->saddr);
	proto->d_addr = ntohl(iph->daddr);
	switch(iph->protocol){
		case IPPROTO_TCP:
			tcph = (struct tcphdr *) ((u8 *) iph + (iph->ihl << 2));
			proto->s_port = ntohs(tcph->source);
			proto->d_port = ntohs(tcph->dest);
			break;
		case IPPROTO_UDP:
			udph = (struct udphdr *) ((u8 *) iph + (iph->ihl << 2));
			proto->s_port = ntohs(udph->source);
			proto->d_port = ntohs(udph->dest);
			break;
	}
}

#endif