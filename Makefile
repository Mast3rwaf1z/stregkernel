
obj-m += stregkernel.o

KERNEL_VERSION ?= $(shell uname -r)
KDIR ?= $(KERNEL)/lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
