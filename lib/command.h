/* 
    命令结构以及相关宏定义
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#ifndef __COMMAND_H__
#define __COMMAND_H__

/* 操作代码宏 */
#define SWITCH_ON 0
#define SWITCH_OFF 1
#define CHAIN_OPERATE 2
#define CHAIN_CHECK 3

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
#define COMMAND_ERROR_M_ 8

/* 策略标志宏 */
#define FW_ACCEPT 0
#define FW_DROP 1

/* 通用宏 */
#define ALL_POLICY 0
#define ALL_PORT 0
#define KERNEL_MAX_MSG_SIZE 2048

/* 返回结果类型 */
#define RET_TEXT 0
#define RET_CHAIN 1

struct command_info{
    int operation; //规则链操作代号
    int chain; //规则链代号
    unsigned char proto; //协议类型
    unsigned int s_addr;
    unsigned int s_mask;
    unsigned short s_port;
    unsigned int d_addr;
    unsigned int d_mask;
    unsigned short d_port;
    unsigned char policy; //使用策略
};

struct msg_command{
    unsigned char todo;
    struct command_info info;
    unsigned char errcode;
};

struct ret_info{
    char msg[KERNEL_MAX_MSG_SIZE];
};

struct msg_ret{
    unsigned char type;
    struct msg_command info;
}

#endif