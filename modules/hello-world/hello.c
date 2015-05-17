#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_load(void)
{
	printk(KERN_INFO "Hello World!\n");

	return 0;
}

static void hello_unload(void)
{
	printk(KERN_INFO "Bye World!\n");
}

module_init(hello_load);
module_exit(hello_unload);
