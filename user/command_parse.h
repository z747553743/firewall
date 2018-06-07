/* 
	命令字符串解析相关函数
	Author: ZengLiangwei
	Email: 747553743@qq.com
 */
#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__
#include <linux/string.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include "../lib/rule_chain.h"
/* 规则链代号宏 */
#define COMMAND_INPUT 0
#define COMMAND_OUTPUT 1
#define COMMAND_FORWARD 2
/* 规则链操作代号宏 */
#define COMMAND_ADD 0
#define COMMAND_INSERT 1
#define COMMAND_DELETE 2
/* 规则解析错误代码宏 */
#define COMMAND_NO_ERROR 0
#define COMMAND_ERROR_CHAIN 1
#define COMMAND_ERROR_P_ 2
#define COMMAND_ERROR_J_ 3
#define COMMAND_ERROR_S_ 4
#define COMMAND_ERROR_D_ 5
#define COMMAND_ERROR_I_ 6
#define COMMAND_ERROR_O_ 7

struct command_info{
	int operation; //规则链操作代号
	int chain; //规则链代号
	unsigned char proto; //协议类型
	unsigned int s_addr;
	unsigned short s_port;
	unsigned int d_addr;
	unsigned short d_port;
	unsigned char policy; //使用策略
	unsigned char correction;
};

/* 判断字符串是否为注释或为空行 */
bool is_comment(char * line){
	while(*line == ' ')
		line++;
	if(*line != '#' && *line != '\n' && *line != '\0')
		return false;
	else
		return true;
}

/* 去除换行符 */
void line_strip(char * line){
	while(*line != '\0'){
		if(*line == '\n'){
			*line = '\0';
		}
		line++;
	}
}

/* 分割命令字符串为数组，方便解析 */
int split_command_line(char * argv[], char * command){
	char* const delim = " ";  
	char *token, *str = command;
	int count = 0;
	while(token = strsep(&str, delim)){
		if(*token != '\0' || *token != '\n' || *token != ' ')
			argv[count++] = token;
	}
	return count;
}

/*extern char *optarg;   //当前选项参数字串（如果有）
extern int optind;     //argv的当前索引值

struct command_info get_command_info(int argc,char *argv[]){
	struct command_info info = {0, 0, 0, 0, 0, 0, 0, 0, COMMAND_NO_ERROR};
	int opt=0;
	while((opt=getopt(argc,argv,"A:D:I:p:j:s:i:d:o:"))!=-1)
	{
		switch(opt)
		{
			case 'A':
				info.operation = COMMAND_ADD;	
			case 'D':
				info.operation = COMMAND_DELETE;
			case 'I':
				info.operation = COMMAND_INSERT;
				if(strcmp(optarg, "INPUT"))
					info.chain = COMMAND_INPUT;
				else if(strcmp(optarg, "OUTPUT"))
					info.chain = COMMAND_OUTPUT;
				else if(strcmp(optarg, "FORWARD"))
					info.chain = COMMAND_FORWARD;
				else
					info.correction = COMMAND_ERROR_CHAIN;
				break;
			case 'p':
				if(strcmp(optarg, "tcp"))
					info.proto = IPPROTO_TCP;
				else if(strcmp(optarg, "icmp"))
					info.proto = IPPROTO_ICMP;
				else if(strcmp(optarg, "udp"))
					info.proto = IPPROTO_UDP;
				else
					info.correction = COMMAND_ERROR_P_;
				break;
			case 'j':
				if(strcmp(optarg, "ACCEPT"))
					info.policy = FW_ACCEPT;
				else if(strcmp(optarg, "DROP"))
					info.policy = FW_DROP;
				else
					info.correction = COMMAND_ERROR_J_;
				break;
			case 's':
				ntohl(in_aton(optarg));
		}
	}
	return info;
}*/

#endif