obj-m := atomic-basics.o atomic-inc.o

# TODO: semaphore, mutex, spinlock, rcu (just show examples)

KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
