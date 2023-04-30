obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: all
	sudo insmod main.ko
	major_number=503; \
	sudo mknod /dev/xpo_gonilnik_kodiranje c $$major_number 0
	sudo chmod 666 /dev/xpo_gonilnik_kodiranje



unload:
	sudo rmmod main
	sudo rm -f /dev/xpo_gonilnik_kodiranje
