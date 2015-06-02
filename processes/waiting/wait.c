#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static int state;
module_param(state, int, S_IRUGO);

/* TODO for students:
 * 1. Implement "sleep" function to delay wait_load execution for a given
 *    number of milliseconds.
 *
 * 2. When waiting in TASK_INTERRUPTIBLE state we might be woken up by a signal
 *    print the signal value to the kernel buffer and test it using "kill" tool.
*/

static int wait_load(void)
{
	if (state != TASK_INTERRUPTIBLE && state != TASK_UNINTERRUPTIBLE)
		return -EINVAL;
		
	pr_info("%s: set current process state to %d\n", __func__, state);

	/* Set current process state */
	current->state = state;

	/* Remove current process from the run-queue */
	schedule();

	/* When we are here: current->state == TASK_RUNNING */
	pr_info("%s: process is woken up, current->state = %ld\n",
			__func__, current->state);

	/* Return error code to avoid rmmod invocation */
	return -ECANCELED;
}

static void wait_unload(void)
{
}

module_init(wait_load);
module_exit(wait_unload);
