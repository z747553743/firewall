/* 
	将规则链上与数据特征进行匹配的相关函数
	author: ZengLiangwei
	email: 747553743@qq.com
 */

#ifndef __RULE_PATTERN_H__
#define __RULE_PATTERN_H__

#include "rule_chain.h"
#include "proto_analysis.h"

/* 判断当前协议与规则链节点是否匹配 */
bool match_pattern(struct rule_chain * node, struct proto_info * proto)
{
	if(node->proto != 0){
		if(node->proto != proto->proto)
			return false;
	}
	if((node->s_addr & node->s_mask) != (proto->s_addr & node->s_mask))
		return false;
	if((node->d_addr & node->d_mask) != (proto->d_addr & node->d_mask))
		return false;
	if(node->s_port != 0){
		if(node->s_port != proto->s_port)
			return false;
	}
	if(node->d_port != 0){
		if(node->d_port != proto->d_port)
			return false;
	}
	return true;
}

/* 扫描规则链中是否有匹配规则，并且返回处理方式 */
unsigned char search_rule_chain(struct rule_chain * head, struct proto_info * proto)
{
	struct rule_chain * node = head;
	while(node != NULL){
		if(match_pattern(node, proto))
			return node->policy;
		node = node->next;
	}
	return FW_ACCEPT;
}

#endif