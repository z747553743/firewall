/* 
	内核程序中改变状态或规则链的相关函数
	Author: ZengLiangwei
	Email: 747553743@qq.com
 */

#ifndef __KERNEL_OPERATE_H__
#define __KERNEL_OPERATE_H__

/* 防火墙开关函数返回值 */
#define SWITCH_ON 0
#define ALREADY_SWITCH_ON 1
#define SWITCH_OFF 2
#define ALREADY_SWITCH_OFF 3

/* IN,OUT,FORWARD规则链 */ 
struct rule_chain * in_chain_head = NULL;
struct rule_chain * out_chain_head = NULL;
struct rule_chain * forward_chain_head = NULL;

/* 防火墙开关 */
bool firewall_switch = false;

int firewall_switch_on(void){
	if(firewall_switch)
		return ALREADY_SWITCH_ON;
	else{
		firewall_switch = true;
		struct rule_chain * tmp_chain = create_rule_chain(ntohl(in_aton("119.75.213.61")), 0xFFFF0000, ALL_PORT, 0, 0x0, ALL_PORT, IPPROTO_TCP, FW_DROP);
		append_rule_chain(&in_chain_head, tmp_chain);
		tmp_chain = create_rule_chain(ntohl(in_aton("58.205.212.207")), 0xFFFF0000, ALL_PORT, 0, 0x0, ALL_PORT, IPPROTO_ICMP, FW_DROP);
		append_rule_chain(&in_chain_head, tmp_chain);
		out_chain_head = create_rule_chain(0, 0x0, ALL_PORT, 0, 0x0, ALL_PORT, IPPROTO_ICMP, FW_ACCEPT);
		forward_chain_head = create_rule_chain(0, 0x0, ALL_PORT, 0, 0x0, ALL_PORT, IPPROTO_TCP, FW_ACCEPT);
		return SWITCH_ON;
	}
}

int firewall_switch_off(void){
	if(firewall_switch){
		firewall_switch = false;
		delete_rule_chain(&in_chain_head);
		delete_rule_chain(&out_chain_head);
		delete_rule_chain(&forward_chain_head);
		return SWITCH_OFF;
	}
	else{
		return ALREADY_SWITCH_OFF;
	}
}

#endif