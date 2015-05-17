obj-m := first.o second.o
KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	cp *.ko /lib/modules/$(shell uname -r)/extra
	depmod

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: install
