PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR   = /usr/src/linux-headers-$(KVERSION)/
    
MODULE_NAME  = camera
obj-m       := $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(MODULE_NAME).o sensor.o
all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

.PHONY: clean
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean