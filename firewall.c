/* 
	防火墙内核模块
	author: ZengLiangwei
	email: 747553743@qq.com
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include "proto_analysis.h"
#include "rule_chain.h"
#include "rule_pattern.h"

MODULE_LICENSE("GPL");

/* 用于注册函数的数据结构 */
static struct nf_hook_ops nfho_in;

/* IN,OUT,FORWARD规则链 */ 
struct rule_chain * in_chain_head = NULL;
struct rule_chain * out_chain_head = NULL;
struct rule_chain * forward_chain_head = NULL;

/* 注册的hook函数的实现 */
unsigned int nf_hook_in_func(unsigned int hooknum,
	struct sk_buff *skb,
	const struct net_device *in,
	const struct net_device *out,
	int (*okfn)(struct sk_buff *))
{
	struct proto_info proto = {0,0,0,0,0};
	get_protocol(skb, &proto);
	printk(KERN_INFO "get %d data\n",proto.proto);
	switch(search_rule_chain(in_chain_head, &proto)){
		case FW_ACCEPT:
			printk(KERN_INFO "accept in data\n");
			return NF_ACCEPT;
		case FW_DROP:
			printk(KERN_INFO "drop in data\n");
			return NF_DROP;
	}
	return NF_DROP;
}

static int __init firewall_init(void){
	printk(KERN_INFO "fire wall module init\n");
	nfho_in.hook = nf_hook_in_func;         /* 该钩子对应的处理函数 */
	nfho_in.hooknum  = NF_INET_LOCAL_IN; /* 使用IPv4的LOCAL_IN钩子 */
	nfho_in.pf       = PF_INET;
	nfho_in.priority = NF_IP_PRI_FIRST;   /* 定义优先级 */
	nf_register_hook(&nfho_in);  //将Local_IN钩子注册到内核中
	in_chain_head = create_rule_chain(0, 0, 0, 0, 0, 0, IPPROTO_TCP, FW_DROP);
	out_chain_head = create_rule_chain(0, 0, 0, 0, 0, 0, IPPROTO_TCP, FW_ACCEPT);
	forward_chain_head = create_rule_chain(0, 0, 0, 0, 0, 0, IPPROTO_TCP, FW_ACCEPT);
	return 0;
}

static void __exit firewall_exit(void){
	destroy_rule_chain(&in_chain_head);
	destroy_rule_chain(&out_chain_head);
	destroy_rule_chain(&forward_chain_head);
	nf_unregister_hook(&nfho_in); //将Local_IN钩子从内核中删除
	printk(KERN_INFO "fire wall module exit\n");
}
module_init(firewall_init);
module_exit(firewall_exit);
MODULE_AUTHOR("Liangwei Zeng");
MODULE_DESCRIPTION("HAHA");
MODULE_VERSION("0.0.1");


