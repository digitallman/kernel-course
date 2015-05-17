#include <linux/module.h>

MODULE_LICENSE("GPL");

void second_func(void)
{
	printk(KERN_INFO "%s\n", __func__);
}

EXPORT_SYMBOL(second_func);

static int second_load(void)
{
	printk(KERN_INFO "Second loaded\n");

	return 0;
}

static void second_unload(void)
{
	printk(KERN_INFO "Second unloaded\n");
}

module_init(second_load);
module_exit(second_unload);
