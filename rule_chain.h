/* 
	防火墙INPUT，OUTPUT，FORWARD链相关的结构以及宏定义
	包含对链节点处理的基本函数
 */
#ifndef __RULE_CHAIN_H__
#define __RULE_CHAIN_H__
/* 策略标志宏 */
#define FW_ACCEPT 0
#define FW_DROP 1

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
	unsigned char policy; //使用策略
};

/* 申请新规则链 */
struct rule_chain * create_rule_chain(
	unsigned int s_addr, unsigned int s_mask, unsigned short s_port,
	unsigned int d_addr, unsigned int d_mask, unsigned short d_port,
	unsigned char policy)
{
	struct rule_chain * new_chain = (struct rule_chain *) malloc(sizeof(rule_chain));
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
	return new_chain;
}

#endif