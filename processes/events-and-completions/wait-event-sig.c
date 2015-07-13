#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/signal.h>

MODULE_LICENSE("GPL");

#define NUM_KTHREADS 5

static struct task_struct *my_threads[NUM_KTHREADS];

DECLARE_WAIT_QUEUE_HEAD(wait_q);
static volatile int cond;

/* QUIZ: print signal number */

static int kthread_func(void *arg)
{
	int ret = 0;

	/* Tell the kernel not to ignore the signal */
	allow_signal(SIGTERM);

	while (!kthread_should_stop() && !cond) {
		pr_info("%s: kthread/%d is waiting for event...\n",
			__func__, (int)arg);

		ret = wait_event_interruptible(wait_q, cond == 1);
		if (!ret) {
			pr_info("%s: kthread/%d woke up, cond=%d\n",
			__func__, (int)arg, cond);
		} else if (ret == -ERESTARTSYS) {
			pr_info("%s: kthread/%d was interrupted by a signal, cond=%d\n",
			__func__, (int)arg, cond);

			/* Reset pending signals */
			flush_signals(current);
			continue;
		}

		break;
	}

	return ret;
}

static int kthreads_load(void)
{
	int i;
	
	pr_info("%s\n", __func__);
	
	for (i = 0; i < NUM_KTHREADS; i++) {
		my_threads[i] = kthread_create(kthread_func, ((void *)i), "%s/%d",
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

	cond = 1;

	pr_info("\n");

	wake_up_all(&wait_q);

	for (i = 0; i < NUM_KTHREADS; i++) {
		kthread_stop(my_threads[i]);
		pr_info("%s: kthread/%d stopped\n", __func__, i);
		put_task_struct(my_threads[i]);
	}
}

module_init(kthreads_load);
module_exit(kthreads_unload);
