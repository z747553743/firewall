/* 
    用户netlink通信相关函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include "user/user_netlink.h"
#include "user/command_parse.h"
#include "lib/command.h"


int main(int argc, char **argv)
{
    struct msg_command command;
    struct command_info * info;
    command = get_command_info(argc, argv);
    if(command.errcode == COMMAND_NO_ERROR){
        info = &(command.info);
        struct _my_msg result_msg;
        create_socket();
        send_data_to_kernel(&command);
        recv_data_from_kernel(&result_msg);
        close_socket();
    }
    else{
        switch(command.errcode){
            case COMMAND_ERROR_CHAIN:
                printf("-A/D/I parameter parse error\n");
                break;
            case COMMAND_ERROR_P_:
                printf("-p parameter parse error\n");
                break;
            case COMMAND_ERROR_J_:
                printf("-j parameter parse error\n");
                break;
            case COMMAND_ERROR_S_:
                printf("-s parameter parse error\n");
                break;
            case COMMAND_ERROR_D_:
                printf("-d parameter parse error\n");
                break;
            case COMMAND_ERROR_I_:
                printf("-i parameter parse error\n");
                break;
            case COMMAND_ERROR_O_:
                printf("-o parameter parse error\n");
                break;
            case COMMAND_ERROR_M_:
                printf("-m parameter parse error\n");
                break;
            default:
                printf("command parse error\n");
                break;
        }
    }
    
    return 0;

}