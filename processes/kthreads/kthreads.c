#include <linux/module.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

static struct task_struct *my_thread;

static struct my_data {
	const char *str;
} my_data = {
	"thread func"
};

static int kthread_func(void *arg)
{
	int i = 0;
	struct my_data *data = arg;

	pr_info("%s\n", __func__);

	while (!kthread_should_stop()) {
		pr_info("%s: i=%d\n", data->str, i++);
		schedule_timeout_interruptible(msecs_to_jiffies(1000));
	}

	return 0;
}

static int kthreads_load(void)
{
	pr_info("%s\n", __func__);
	
	my_thread = kthread_create(kthread_func, &my_data, "%s",
							"udemy kthread");
	if (IS_ERR(my_thread)) {
		pr_info("%s: cannot create kernel thread\n", __func__);
		return PTR_ERR(my_thread);
	}

	/* Protect my_thread from being freed by the kernel */
	get_task_struct(my_thread);

	/* Kick the thread */
	wake_up_process(my_thread);

	return 0;
}

static void kthreads_unload(void)
{
	kthread_stop(my_thread);
	pr_info("%s: udemy kthread stopped\n", __func__);
	put_task_struct(my_thread);
}

module_init(kthreads_load);
module_exit(kthreads_unload);
