# 防火墙的Makefile文件
# Author: ZengLiangwei
# Email: 747553743@qq.com
 
obj-m += firewall.o
 
KDIR =/usr/src/linux-headers-$(shell uname -r)
 
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
install:
	insmod firewall.ko
uninstall:
	rmmod firewall.ko 
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *.cmd .*.cmd .*.swp
