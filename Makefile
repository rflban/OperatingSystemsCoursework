CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE),)
	obj-m := fnrootkit.o
	fnrootkit-objs := ./src/proc.o ./src/fnrootkit.o
else
	CFLAGS += -Wall
	CC := gcc $(CFLAGS)
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
