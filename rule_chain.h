/* 
	防火墙INPUT，OUTPUT，FORWARD链相关的结构以及宏定义
	包含对链节点处理的基本函数
	Author: ZengLiangwei
	Email: 747553743@qq.com
 */
#ifndef __RULE_CHAIN_H__
#define __RULE_CHAIN_H__
#include <linux/slab.h>
/* 策略标志宏 */
#define FW_ACCEPT 0
#define FW_DROP 1

/* 通用宏 */
#define ALL_POLICY 0
#define ALL_PORT 0

/* 规则链节点 */
struct rule_chain{
	struct rule_chain * pre; //前向指针
	struct rule_chain * next; //后向指针
	unsigned int s_addr; //源IP地址
	unsigned int s_mask; //源IP地址掩码
	unsigned short s_port; //源端口
	unsigned int d_addr; //目的IP地址
	unsigned int d_mask; //目的IP地址掩码
	unsigned short d_port; //目的端口
	unsigned char proto; //协议类型
	unsigned char policy; //使用策略
};

/* 申请新规则链节点 */
struct rule_chain * create_rule_chain(
	unsigned int s_addr, unsigned int s_mask, unsigned short s_port,
	unsigned int d_addr, unsigned int d_mask, unsigned short d_port,
	unsigned char proto, unsigned char policy)
{
	struct rule_chain * new_chain = (struct rule_chain *) kmalloc(sizeof(struct rule_chain), GFP_KERNEL);
	if(new_chain == NULL) // 申请失败
		return new_chain;
	new_chain->pre = NULL;
	new_chain->next = NULL;
	new_chain->s_addr = s_addr;
	new_chain->s_mask = s_mask;
	new_chain->s_port = s_port;
	new_chain->d_addr = d_addr;
	new_chain->d_mask = d_mask;
	new_chain->d_port = d_port;
	new_chain->proto = proto;
	new_chain->policy = policy;
	return new_chain;
}

/* 释放规则链节点 */
void destroy_rule_chain_node(struct rule_chain ** chain)
{
	if(*chain != NULL)
		kfree(*chain);
	*chain = NULL;
}

/* 插入规则链节点 */
void insert_rule_chain(struct rule_chain ** head, struct rule_chain * node, int index)
{
	if(index < 0 || node == NULL)
		return;
	int i = 0;
	struct rule_chain * pre_node = NULL, *tmp_node = *head;
	while(i < index && tmp_node != NULL){
		pre_node = tmp_node;
		tmp_node = tmp_node->next;
		i++;
	}
	if(pre_node == NULL){
		node->next = tmp_node;
		if(tmp_node != NULL)
			tmp_node->pre = node;
		*head = node;
	}
	else{
		node->next = pre_node->next;
		node->next->pre = node;
		node->pre = pre_node;
		pre_node->next = node;
	}
}

/* 规则链最后插入节点 */
void append_rule_chain(struct rule_chain ** head, struct rule_chain * node)
{
	struct rule_chain * tmp_node = *head;
	if(node == NULL)
		return;
	if(*head == NULL)
		*head = node;
	else{
		while(tmp_node->next != NULL)
			tmp_node = tmp_node->next;
		tmp_node->next = node;
		node->pre = tmp_node;
	}
}

/* 删除规则链节点 */
void delete_rule_chain_node(struct rule_chain ** head, struct rule_chain * node)
{
	if(node == NULL)
		return;
	if(*head == node){
		*head = node->next;
		node->next->pre = NULL;
	}
	else{
		node->pre->next = node->next;
		node->next->pre = node->pre;
	}
	destroy_rule_chain_node(&node);
}

/* 删除整条规则链 */
void delete_rule_chain(struct rule_chain ** head){
	struct rule_chain * node = *head;
	struct rule_chain * next_node = NULL;
	while(node != NULL){
		next_node = node->next;
		destroy_rule_chain_node(&node);
		node = next_node;
	}
	*head = NULL;
}

#endif