#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include "proto_analysis.h"
#include <linux/ip.h>

MODULE_LICENSE("GPL");

/* 用于注册我们的函数的数据结构 */
static struct nf_hook_ops nfho;
/* 注册的hook函数的实现 */
unsigned int hook_func(unsigned int hooknum,
	struct sk_buff *skb,
	const struct net_device *in,
	const struct net_device *out,
	int (*okfn)(struct sk_buff *))
{
	struct proto_info proto = {0,0,0,0,0};
	get_protocol(skb, &proto);
	printk(KERN_ALERT "get %d data\n",proto.proto);
	switch(proto.proto){
		case IPPROTO_TCP:
			printk(KERN_ALERT "get tcp data\n");
			break;
		case IPPROTO_UDP:
			printk(KERN_ALERT "get udp data\n");
			break;
		default:
			printk(KERN_ALERT "get other data\n");
			break;
	}
	return NF_ACCEPT;
}

static int __init firewall_init(void){
	printk(KERN_ALERT "fire wall module init\n");
	nfho.hook = hook_func;         /* 该钩子对应的处理函数 */
	nfho.hooknum  = NF_INET_LOCAL_IN; /* 使用IPv4的第一个hook */
	nfho.pf       = PF_INET;
	nfho.priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */
	nf_register_hook(&nfho);  //将用户自己定义的钩子注册到内核中
	return 0;
}

static void __exit firewall_exit(void){
	nf_unregister_hook(&nfho); //将用户自己定义的钩子从内核中删除
	printk(KERN_ALERT "fire wall module exit\n");
}
module_init(firewall_init);
module_exit(firewall_exit);
MODULE_AUTHOR("Liangwei Zeng");
MODULE_DESCRIPTION("HAHA");
MODULE_VERSION("0.0.1");


