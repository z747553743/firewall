/* 
    内核日志的相关IO函数
    Author: ZengLiangwei
    Email: 747553743@qq.com
 */
#ifndef __KERNEL_LOG_IO_H__
#define __KERNEL_LOG_IO_H__
#include <linux/kernel.h>

/* 每次读取内核文件一行 */
int read_kernel_file_line(struct file * r_file, char * buf, int length){
    static char tmp_buf[2048] = {0};
    static int cur_pointer = 0;
    static loff_t pos = 0;
    int count = 0, buf_index = 0, end_flag = 0, tmp_index = cur_pointer;
    while(!end_flag){
        if(tmp_buf[cur_pointer] == '\0'){
            count = vfs_read(r_file, tmp_buf, sizeof(tmp_buf) - 1, &pos);
            tmp_buf[count] = '\0';
            cur_pointer = 0;
            if(count == 0)
                break;
        }
        while(tmp_buf[cur_pointer] != '\n' && tmp_buf[cur_pointer] != '\0')
            cur_pointer++;
        if(tmp_buf[cur_pointer] == '\n'){
            cur_pointer++;
            end_flag = 1;
        }
        while(tmp_index < cur_pointer)
            buf[buf_index++] = tmp_buf[tmp_index++];
    }

    buf[buf_index] = '\0';
    return buf_index;
}

int read_kernel_file(void){
    struct file * log_file = NULL;
    char buf[2048] = {0};
    char * argv[20] = {NULL};
    int count = 0;
    mm_segment_t old_fs;
    const char * filename = "rule.conf";
    log_file = filp_open(filename, O_RDONLY, 0);
    if(!log_file || IS_ERR(log_file))
        return -1;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    while((count = read_kernel_file_line(log_file, buf, sizeof(buf) - 1)) > 0){
        buf[count] = '\0';
        printk(KERN_INFO "%s\n", buf);
    }
    set_fs(old_fs);
    filp_close(log_file, NULL);
    return 0;
}


#endif