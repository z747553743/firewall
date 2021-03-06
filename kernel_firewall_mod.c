/* 
    防火墙内核模块
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include "lib/proto_analysis.h"
#include "lib/rule_chain.h"
#include "lib/rule_pattern.h"
#include "kernel/kernel_log_io.h"
#include "kernel/kernel_operate.h"
#include "kernel/kernel_netlink.h"

MODULE_LICENSE("GPL");

/* 用于注册函数的数据结构 */
static struct nf_hook_ops nfho_in;
static struct nf_hook_ops nfho_out;
static struct nf_hook_ops nfho_forward;

/* IN,OUT,FORWARD规则链 */ 
extern struct rule_chain * in_chain_head;
extern struct rule_chain * out_chain_head;
extern struct rule_chain * forward_chain_head;

/* 防火墙开关 */
extern bool firewall_switch;

/* 注册的in_hook函数 */
unsigned int nf_hook_in_func(unsigned int hooknum,
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn)(struct sk_buff *))
{
    if(!firewall_switch)
        return NF_ACCEPT;
    struct proto_info proto = {0,0,0,0,0};
    get_protocol(skb, &proto);
    switch(search_rule_chain(in_chain_head, &proto)){
        case FW_ACCEPT:
            return NF_ACCEPT;
        case FW_DROP:
            return NF_DROP;
    }
    return NF_DROP;
}

/* 注册的out_hook函数 */
unsigned int nf_hook_out_func(unsigned int hooknum,
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn)(struct sk_buff *))
{
    if(!firewall_switch)
        return NF_ACCEPT;
    struct proto_info proto = {0,0,0,0,0};
    get_protocol(skb, &proto);
    switch(search_rule_chain(out_chain_head, &proto)){
        case FW_ACCEPT:
            return NF_ACCEPT;
        case FW_DROP:
            return NF_DROP;
    }
    return NF_DROP;
}

/* 注册的forward_hook函数 */
unsigned int nf_hook_forward_func(unsigned int hooknum,
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn)(struct sk_buff *))
{
    if(!firewall_switch)
        return NF_ACCEPT;
    struct proto_info proto = {0,0,0,0,0};
    get_protocol(skb, &proto);
    switch(search_rule_chain(forward_chain_head, &proto)){
        case FW_ACCEPT:
            return NF_ACCEPT;
        case FW_DROP:
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

    nfho_out.hook = nf_hook_out_func;
    nfho_out.hooknum  = NF_INET_LOCAL_OUT; /* 使用IPv4的LOCAL_OUT钩子 */
    nfho_out.pf       = PF_INET;
    nfho_out.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&nfho_out);

    nfho_forward.hook = nf_hook_forward_func;
    nfho_forward.hooknum  = NF_INET_FORWARD; /* 使用IPv4的FORWARD钩子 */
    nfho_forward.pf       = PF_INET;
    nfho_forward.priority = NF_IP_PRI_FIRST; 
    nf_register_hook(&nfho_forward);

    //firewall_switch_on();//启动防火墙
    netlink_init();
    return 0;
}

static void __exit firewall_exit(void){
	netlink_exit();
    firewall_switch_off(); //关闭防火墙
    delete_rule_chain(&in_chain_head);
    delete_rule_chain(&out_chain_head);
    delete_rule_chain(&forward_chain_head);
    nf_unregister_hook(&nfho_in); //将Local_IN钩子从内核中删除
    nf_unregister_hook(&nfho_out);
    nf_unregister_hook(&nfho_forward);
    printk(KERN_INFO "fire wall module exit\n");
}

module_init(firewall_init);
module_exit(firewall_exit);
MODULE_AUTHOR("ZengLiangwei");
MODULE_DESCRIPTION("FireWall practice");
MODULE_VERSION("0.0.1");