/* 
	命令字符串解析相关函数
	Author: ZengLiangwei
	Email: 747553743@qq.com
 */
#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__
#include <linux/string.h>
/* 规则链代号宏 */
#define COMMAND_INPUT 0
#define COMMAND_OUTPUT 1
#define COMMAND_FORWARD 2
/* 规则链操作代号宏 */
#define COMMAND_ADD 0
#define COMMAND_INSERT 1
#define COMMAND_DELETE 2


struct command_info{
	int operation; //规则链操作代号
	int chain; //规则链代号
	unsigned char proto; //协议类型
	unsigned int s_addr;
	unsigned short s_port;
	unsigned int d_addr;
	unsigned short d_port;
	unsigned char policy; //使用策略
}

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



#endif