#at first type on ur terminal that $(uname -r) then u will get the version..
# that is using on ur system
 
obj-m += hello.o
 
KDIR =/usr/src/linux-headers-$(shell uname -r)
 
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
install:
	insmod hello.ko
uninstall:
	rmmod hello.ko 
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
