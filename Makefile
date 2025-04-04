
obj-m += stregkernel.o

stregkernel-objs := src/http.o src/integrations/balance.o src/integrations/flogo.o src/integrations/history.o src/integrations/products.o src/integrations/quickbuy.o src/integrations/settings.o src/chrdev.o src/http.o src/stregsystem.o src/utils.o src/tcp.o

CC ?= gcc
KERNEL_VERSION ?= $(shell uname -r)
KDIR ?= $(KERNEL)/lib/modules/$(KERNEL_VERSION)/build

all:
	make CFLAGS_MODULE="-I$(PWD)" -C $(KDIR) M=$(PWD) modules

clean:
	make CFLAGS=MODULE="-I$(PWD)" -C $(KDIR) M=$(PWD) clean
	rm -rf bin

tests:
	mkdir -p bin
	$(CC) -I. src/tests/utils.c -o bin/test_utils
