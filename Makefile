# 防火墙的Makefile文件
# Author: ZengLiangwei
# Email: 747553743@qq.com
 
obj-m += kernel_firewall_mod.o
 
KDIR =/usr/src/linux-headers-$(shell uname -r)
CXXFLAGS = $(CXXDEBUG) $(CXXWFLAGS) $(ECXXFLAGS) -Wno-strict-aliasing
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	gcc -o firewall firewall.c
install:
	insmod kernel_firewall_mod.ko
uninstall:
	rmmod kernel_firewall_mod.ko 
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *.cmd .*.cmd .*.swp firewall
