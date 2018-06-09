/* 
    内核程序中改变状态或规则链的相关函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */

#ifndef __KERNEL_OPERATE_H__
#define __KERNEL_OPERATE_H__
#include "../lib/command.h"
#include "../lib/rule_chain.h"
/* 防火墙开关函数返回值 */
#define FIREWALL_SWITCH_ON 0
#define ALREADY_SWITCH_ON 1
#define FIREWALL_SWITCH_OFF 2
#define ALREADY_SWITCH_OFF 3

/* IN,OUT,FORWARD规则链 */ 
struct rule_chain * in_chain_head = NULL;
struct rule_chain * out_chain_head = NULL;
struct rule_chain * forward_chain_head = NULL;

/* 防火墙开关 */
bool firewall_switch = false;

/* 启动防火墙 */
int firewall_switch_on(void){
    if(firewall_switch)
        return ALREADY_SWITCH_ON;
    else{
        firewall_switch = true;
        return FIREWALL_SWITCH_ON;
    }
}

/* 关闭防火墙 */
int firewall_switch_off(void){
    if(firewall_switch){
        delete_rule_chain(&in_chain_head);
        delete_rule_chain(&out_chain_head);
        delete_rule_chain(&forward_chain_head);
        firewall_switch = false;
        return FIREWALL_SWITCH_OFF;
    }
    else{
        return ALREADY_SWITCH_OFF;
    }
}

/* 在对应链上添加节点 */
int append_chain(struct command_info * info){
    struct rule_chain ** chain_head = NULL;
    switch(info->chain){
        case COMMAND_INPUT:
            chain_head = &in_chain_head;
            break;
        case COMMAND_OUTPUT:
            chain_head = &out_chain_head;
            break;
        case COMMAND_FORWARD:
            chain_head = &forward_chain_head;
            break;
        default:
            return -1;
    }
    struct rule_chain * tmp_chain = create_rule_chain(info->s_addr, info->s_mask, info->s_port,
                                    info->d_addr, info->d_mask, info->d_port, info->proto, info->policy);
    append_rule_chain(chain_head, tmp_chain);
    return 0;
}

/* 在对应链上删除节点 */
int delete_chain(struct command_info * info){
    struct rule_chain ** chain_head = NULL;
    switch(info->chain){
        case COMMAND_INPUT:
            chain_head = &in_chain_head;
            break;
        case COMMAND_OUTPUT:
            chain_head = &out_chain_head;
            break;
        case COMMAND_FORWARD:
            chain_head = &forward_chain_head;
            break;
        default:
            return -1;
    }
    return delete_rule_chain_node_by_info(chain_head, info->s_addr, info->s_mask, info->s_port,
    	info->d_addr, info->d_mask, info->d_port, info->proto, info->policy);
}

/* 命令对链的各种操作 */
int chain_operate(struct command_info * info){
	int ret = 0;
    if(info->operation == COMMAND_ADD){
        ret = append_chain(info);
    }
    else if(info->operation == COMMAND_DELETE){
    	ret = delete_chain(info);
    }
    return ret;
}


#endif