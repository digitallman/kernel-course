#undef DEBUG

#include <linux/module.h>
#include <linux/printk.h>

/* For linux/ratelimit.h */
#include <linux/ratelimit.h>

MODULE_LICENSE("GPL");


static int messages_load(void)
{
	int i;
	printk("KERN_DEFAULT test message\n");

	printk(KERN_INFO "KERN_INFO test message\n");
	printk(KERN_DEBUG "KERN_DEBUG test message\n");
	printk(KERN_NOTICE "KERN_NOTICE test message\n");

	printk(KERN_WARNING "test message\n");
	printk(KERN_ERR "KERN_ERR test message\n");
	printk(KERN_CRIT "KERN_CRIT test message\n");

	printk(KERN_ALERT "KERN_ALERT test message\n");
	printk(KERN_EMERG "KERN_EMERG test message\n");

	for (i = 1; i < 100000; i++) {
		printk_once("PRINTK_ONCE: i = %d\n", i);
		printk_ratelimited(KERN_INFO "i=%d\n", i);
	}


	pr_warn("pr_warn()\n");
	
	return 0;
}

static void messages_unload(void)
{
	printk(KERN_INFO "%s\n", __func__);
}

module_init(messages_load);
module_exit(messages_unload);
