#include <linux/module.h>

MODULE_LICENSE("GPL");

extern void second_func(void);

static int first_load(void)
{
	printk(KERN_INFO "First loaded\n");
	second_func();

	return 0;
}

static void first_unload(void)
{
	printk(KERN_INFO "First unloaded!\n");
}

module_init(first_load);
module_exit(first_unload);
