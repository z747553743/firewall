/* 
    命令字符串解析相关函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "../lib/command.h"

/* 判断字符串是否为注释或为空行 */
int is_comment(char * line)
{
    while(*line == ' ')
        line++;
    if(*line != '#' && *line != '\n' && *line != '\0')
        return 0;
    else
        return 1;
}

/* 去除换行符 */
void line_strip(char * line)
{
    while(*line != '\0'){
        if(*line == '\n'){
            *line = '\0';
        }
        line++;
    }
}

/* 分割命令字符串为数组，方便解析 */
int split_command_line(char * argv[], char * command)
{
    char* const delim = " ";  
    char *token, *str = command;
    int count = 0;
    while(token = strsep(&str, delim)){
        if(*token != '\0' || *token != '\n' || *token != ' ')
            argv[count++] = token;
    }
    return count;
}

/* 将前缀长度转为对应掩码 */
unsigned int encode_mask(unsigned int prefix_len){
	int result = 0x80000000;
	if(prefix_len == 0)
		return 0;
	result = result >> (prefix_len - 1);
	return result;
}

extern char *optarg;   //当前选项参数字串（如果有）
extern int optind;     //argv的当前索引值

struct msg_command get_command_info(int argc,char *argv[]){
	struct msg_command command = {CHAIN_OPERATE, {0, 0, 0, 0, 0, 0, 0, 0}, COMMAND_NO_ERROR};
    struct command_info * info = &command.info;
    int opt=0, s_pre_len = 32, d_pre_len = 32;
    char * c_point = NULL;
    while((opt=getopt(argc,argv,"m:A:D:I:p:j:s:i:d:o:"))!=-1)
    {
        switch(opt)
        {
            case 'm':
            	if(!strcmp(optarg, "on"))
                    command.todo = SWITCH_ON;
                else if(!strcmp(optarg, "off"))
                    command.todo = SWITCH_OFF;
                else
                	command.errcode = COMMAND_ERROR_M_;
                break;
            case 'A':
                info->operation = COMMAND_ADD;
                if(!strcmp(optarg, "INPUT"))
                    info->chain = COMMAND_INPUT;
                else if(!strcmp(optarg, "OUTPUT"))
                    info->chain = COMMAND_OUTPUT;
                else if(!strcmp(optarg, "FORWARD"))
                    info->chain = COMMAND_FORWARD;
                else
                    command.errcode = COMMAND_ERROR_CHAIN;
                break;
            case 'D':
                info->operation = COMMAND_DELETE;
                if(!strcmp(optarg, "INPUT"))
                    info->chain = COMMAND_INPUT;
                else if(!strcmp(optarg, "OUTPUT"))
                    info->chain = COMMAND_OUTPUT;
                else if(!strcmp(optarg, "FORWARD"))
                    info->chain = COMMAND_FORWARD;
                else
                    command.errcode = COMMAND_ERROR_CHAIN;
                break;
            case 'I':
                info->operation = COMMAND_INSERT;
                if(!strcmp(optarg, "INPUT"))
                    info->chain = COMMAND_INPUT;
                else if(!strcmp(optarg, "OUTPUT"))
                    info->chain = COMMAND_OUTPUT;
                else if(!strcmp(optarg, "FORWARD"))
                    info->chain = COMMAND_FORWARD;
                else
                    command.errcode = COMMAND_ERROR_CHAIN;
                break;
            case 'p':
                if(!strcmp(optarg, "tcp"))
                    info->proto = IPPROTO_TCP;
                else if(!strcmp(optarg, "icmp"))
                    info->proto = IPPROTO_ICMP;
                else if(!strcmp(optarg, "udp"))
                    info->proto = IPPROTO_UDP;
                else
                    command.errcode = COMMAND_ERROR_P_;
                break;
            case 'j':
                if(!strcmp(optarg, "ACCEPT"))
                    info->policy = FW_ACCEPT;
                else if(!strcmp(optarg, "DROP"))
                    info->policy = FW_DROP;
                else
                    command.errcode = COMMAND_ERROR_J_;
                break;
            case 's':
            	c_point = strchr(optarg, '/');
            	if(c_point){
            		*(c_point++) = '\0';
            		s_pre_len = atoi(c_point);
            		if(s_pre_len < 0 || s_pre_len > 32)
            			command.errcode = COMMAND_ERROR_S_;
            	}
                info->s_addr = inet_network(optarg);
                info->s_mask = encode_mask(s_pre_len);
                break;
            case 'd':
            	c_point = strchr(optarg, '/');
            	if(c_point){
            		*(c_point++) = '\0';
            		d_pre_len = atoi(c_point);
            		if(d_pre_len < 0 || d_pre_len > 32)
            			command.errcode = COMMAND_ERROR_D_;
            	}
                info->d_addr = inet_network(optarg);
                info->d_mask = encode_mask(d_pre_len);
                break;
            case 'i':
                info->s_port = atoi(optarg);
                break;
            case 'o':
                info->d_port = atoi(optarg);
                break;
        }
    }
    return command;
}

#endif