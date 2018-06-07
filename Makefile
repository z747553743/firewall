# 防火墙的Makefile文件
# Author: ZengLiangwei
# Email: 747553743@qq.com
 
obj-m += kernel_firewall_mod.o
 
KDIR =/usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
install:
	insmod kernel_firewall_mod.ko
uninstall:
	rmmod kernel_firewall_mod.ko 
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *.cmd .*.cmd .*.swp
