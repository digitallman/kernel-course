#include <linux/module.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

#define NUM_KTHREADS 5

static struct task_struct *my_threads[NUM_KTHREADS];

/* 
 * Naive ways:
 * static int var = 0;
 * static volatile int var = 0;
 */

/* 
 * Right way: 
 */
static atomic_t var = { 0 };

#define sleep_sec(secs) schedule_timeout_interruptible(msecs_to_jiffies(secs*1000))

static int kthread_func_read(void *arg)
{
	while (!kthread_should_stop()) {
		pr_info("%s: var = %d\n", __func__, atomic_read(&var));
		sleep_sec(1);
	}

	return 0;
}

static int kthread_func_write(void *arg)
{
	atomic_set(&var, 1);
	return 0;
}

static int (*kthread_func[])(void *) = {
	&kthread_func_write,
	&kthread_func_read,
	NULL
};
 
static int kthreads_load(void)
{
	int i;
	pr_info("%s\n", __func__);
	
	for (i = 0; i < NUM_KTHREADS && kthread_func[i]; i++) {
		
		my_threads[i] = kthread_create(kthread_func[i], NULL, "%s/%d",
						"kthread", i);
		if (IS_ERR(my_threads[i])) {
			pr_info("%s: cannot create kthread/%d\n", __func__, i);
			return PTR_ERR(my_threads[i]);
		}

		/* Protect my_thread from being destroyed by the kernel */
		get_task_struct(my_threads[i]);

		/* Kick the thread */
		wake_up_process(my_threads[i]);
	}

	return 0;
}

static void kthreads_unload(void)
{
	int i;

	for (i = 0; i < NUM_KTHREADS && kthread_func[i]; i++) {
		kthread_stop(my_threads[i]);
		pr_info("%s: kthread/%d stopped\n", __func__, i);
		put_task_struct(my_threads[i]);
	}

	pr_info("%s: var = %d\n", __func__, atomic_read(&var));
}

module_init(kthreads_load);
module_exit(kthreads_unload);
