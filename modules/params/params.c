#include <linux/module.h>

MODULE_LICENSE("GPL");

static char *my_name = "Oldman";
module_param(my_name, charp, S_IRUGO);

static int my_age = 100;
module_param(my_age, int, S_IRUGO);

static int params_load(void)
{
	printk(KERN_INFO "%s: name=%s, age=%d\n", __func__, my_name, my_age);
	return 0;
}

static void params_unload(void)
{
	printk(KERN_INFO "%s\n", __func__);
}

module_init(params_load);
module_exit(params_unload);
