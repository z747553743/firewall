/* 
    内核netlink通信相关函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#ifndef __KERNEL_NETLINK_H__
#define __KERNEL_NETLINK_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include "../lib/command.h"
#include "kernel_operate.h"

#define NETLINK_USER 22
#define USER_MSG (NETLINK_USER + 1)
#define USER_PORT  50

extern bool firewall_switch;
static struct sock *netlinkfd = NULL;
char ret_msg[KERNEL_MAX_MSG_SIZE];

int send_msg(int8_t *pbuf, uint16_t len)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;

    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if(!nl_skb)
    {
        printk("netlink_alloc_skb error\n");
        return -1;
    }

    nlh = nlmsg_put(nl_skb, 0, 0, USER_MSG, len, 0);
    if(nlh == NULL)
    {
        printk("nlmsg_put() error\n");
        nlmsg_free(nl_skb);
        return -1;
    }
    memcpy(nlmsg_data(nlh), pbuf, len);

    ret = netlink_unicast(netlinkfd, nl_skb, USER_PORT, MSG_DONTWAIT);

    return ret;
}


static void recv_command(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = NULL;
    void *data = NULL;
    int ret;
    struct msg_command * command = NULL;
    struct command_info * info = NULL;
    if(skb->len >= nlmsg_total_size(0))
    {
        nlh = nlmsg_hdr(skb);
        data = NLMSG_DATA(nlh);
        if(data)
        {
            command = (struct msg_command *) data;
            info = &(command->info);
            if(command->todo == SWITCH_ON){
                ret = firewall_switch_on();
                if(ret == FIREWALL_SWITCH_ON)
                    send_msg("switch on", 10);
                else if(ret == ALREADY_SWITCH_ON)
                    send_msg("already swtich on", 18);
                else
                    send_msg("switch on error", 16);
            }
            else if(command->todo == SWITCH_OFF){
                ret = firewall_switch_off();
                if(ret == FIREWALL_SWITCH_OFF)
                    send_msg("switch off", 11);
                else if(ret == ALREADY_SWITCH_OFF)
                    send_msg("already swtich off", 19);
                else
                    send_msg("switch off error", 17);
            }
            else if(command->todo == CHAIN_OPERATE){
                if(!chain_operate(&command->info)){
                    send_msg("ok", 3);
                }
                else{
                    send_msg("error", 6);
                }
            }
            else if(command->todo == CHAIN_CHECK){

            }
        }
    }
} 

struct netlink_kernel_cfg cfg = 
{
    .input = recv_command,
};

int netlink_init(void)
{
    printk("init netlink_demo!\n");
    if(netlinkfd == NULL)
        netlinkfd = netlink_kernel_create(&init_net, USER_MSG, &cfg);
    if(!netlinkfd)
    {
        printk(KERN_ERR "can not create a netlink socket!\n");
        return -1;
    }

    printk("netlink demo init ok!\n");
    return 0;
}

void netlink_exit(void)
{
    if(netlinkfd != NULL)
        sock_release(netlinkfd->sk_socket);
    netlinkfd = NULL;
    printk(KERN_DEBUG "netlink exit\n!");
}

#endif