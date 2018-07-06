/* 
    用户netlink通信相关函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#ifndef __USER_NETLINK_H__
#define __USER_NETLINK_H__
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/command.h"

#define NETLINK_USER 22
#define USER_MSG (NETLINK_USER + 1)
#define MSG_LEN 100

#define MAX_PLOAD 100

struct _my_msg
{
    struct nlmsghdr hdr;
    int8_t data[MSG_LEN];
};

int skfd;
struct sockaddr_nl local, dest_addr;
struct nlmsghdr *nlh = NULL;
int create_socket(void)
{
    skfd = socket(AF_NETLINK, SOCK_RAW, USER_MSG);
    if(skfd == -1)
    {
        printf("create socket error...%s\n", strerror(errno));
        return -1;
    }

    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = 50; 
    local.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&local, sizeof(local)) != 0)
    {
        printf("bind() error\n");
        close(skfd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; // to kernel
    dest_addr.nl_groups = 0;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
    memset(nlh, 0, sizeof(struct nlmsghdr));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = local.nl_pid; //self port
    return 0;
}

int send_data_to_kernel(struct msg_command * command){
    int ret;
    memcpy(NLMSG_DATA(nlh), command, sizeof(struct msg_command));
    ret = sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_nl));

    if(!ret)
    {
        perror("sendto error1\n");
        close(skfd);
        exit(-1);
    }
    return ret;
}

int recv_data_from_kernel(struct _my_msg * info){
    int ret;
    memset(info, 0, sizeof(struct _my_msg));
    ret = recvfrom(skfd, info, sizeof(struct _my_msg), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if(!ret)
    {
        perror("recv form kernel error\n");
        close(skfd);
        exit(-1);
    }
    printf("%s\n", info->data);
    return ret;
}

void close_socket(void){
    close(skfd);
    free((void *)nlh);
}

#endif