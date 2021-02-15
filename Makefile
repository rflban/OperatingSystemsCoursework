CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE),)
    obj-m := fnrootkit.o
else
	CFLAGS := -Wall
	CC := gcc $(CFLAGS)
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
