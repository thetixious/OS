obj-m := ioctl_driver.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
ARCH ?= $(shell uname -m)

all:
	make -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) clean