#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static void print_process_info(void)
{
        printk(KERN_INFO "%s: PID = %d\n", __func__, current->pid);
        printk(KERN_INFO "%s: state = 0x%02lx\n", __func__, current->state);
        printk(KERN_INFO "%s: flags = 0x%04x\n", __func__, current->flags);
        printk(KERN_INFO "%s: on_cpu = %d\n", __func__, current->on_cpu);
}

/* module init function is called from finit_module() system call */
static int dummy_load(void)
{
	printk(KERN_INFO "%s\n", __func__);

	print_process_info();

	return 0;
}

static void dummy_unload(void)
{
	printk(KERN_INFO "%s\n", __func__);
}

module_init(dummy_load);
module_exit(dummy_unload);
