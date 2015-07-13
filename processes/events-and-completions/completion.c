#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/completion.h>

MODULE_LICENSE("GPL");

static struct task_struct *my_thread;

DECLARE_COMPLETION(my_completion);

/* QUIZ: what is process state while waiting for completion? */
/* QUIZ: use wait-for-event to play with multiple kthreads */
/* QUIZ: try exclusive/broadcast completion with multiple kthreads */
/* QUIZ: implement waiting for completion with a timeout = 1 sec. */

static int kthread_func(void *arg)
{
	pr_info("%s\n", __func__);

	while (!kthread_should_stop()) {
		pr_info("%s: waiting for completion...\n", __func__);
		wait_for_completion(&my_completion);

		pr_info("%s: completion received\n", __func__);
	}

	return 0;
}

static int completion_load(void)
{
	pr_info("%s\n", __func__);

	my_thread = kthread_create(kthread_func, NULL, "%s",
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

static void completion_unload(void)
{
	complete(&my_completion);

	kthread_stop(my_thread);
	pr_info("%s: udemy kthread stopped\n", __func__);
	put_task_struct(my_thread);
}

module_init(completion_load);
module_exit(completion_unload);
