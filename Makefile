
obj-m += stregkernel.o

CC ?= gcc
KERNEL_VERSION ?= $(shell uname -r)
KDIR ?= $(KERNEL)/lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf bin

tests:
	mkdir -p bin
	$(CC) -I. src/test_utils.c -o bin/test_utils
