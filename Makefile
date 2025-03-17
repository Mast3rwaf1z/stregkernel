MOD_NAME:=stregsystem
obj-m += $(MOD_NAME).o
MOD_FILE:=$(MOD_NAME).ko
KERNEL_DIR:=/lib/modules/$(shell uname -r)/build
INSTALL_MOD_PATH:=$(PWD)

all:
	make -C $(KERNEL_DIR) M=$(INSTALL_MOD_PATH) modules

reload: clean all
	sudo rmmod $(MOD_NAME)
	sudo insmod $(MOD_FILE)

clean:
	make -C $(KERNEL_DIR) M=$(INSTALL_MOD_PATH) clean
