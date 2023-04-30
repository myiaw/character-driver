# main.o is made from main.c
obj-m += main.o

# shell uname -r returns the kernel version
# M=$(PWD) - tells the kernel to build the module in the current directory
# /lib/modules - tells the kernel where to find everything to build the module correctly (contains kernel headers -> declarations of functions, variables, etc.)
# modules - tells the kernel to build the module

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# c - character device
# 503 - driver_num
# 0 - first one, starts with 0.

# load the module and create the device file
load: all
	sudo insmod main.ko
	sudo mknod /dev/xpo_gonilnik_kodiranje c 503 0
	sudo chmod 666 /dev/xpo_gonilnik_kodiranje

# unload the module and remove the device file
unload:
	sudo rmmod main
	sudo rm -f /dev/xpo_gonilnik_kodiranje
