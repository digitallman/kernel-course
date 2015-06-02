#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static const char* get_state_str(unsigned long state)
{
	switch (state) {
	case TASK_RUNNING:
		return "RUNNING";
	case TASK_INTERRUPTIBLE:
		return "WAIT INTERRUPTIBLE";
	case TASK_UNINTERRUPTIBLE:
		return "WAIT UNINTERRUPTIBLE";
	default:
		return "another task state";
	} 
}

static void enumerate_processes(void)
{
	struct task_struct *p;
	unsigned int p_count = 0;

	for_each_process(p) {
		pr_info("%s: PID=%d, state=\'%s\'\n", 
				__func__, p->pid, get_state_str(p->state));
		p_count++;
	}

	pr_info("%s: total %u processes enumerated\n", __func__, p_count);
}

static void wakeup_process(pid_t pid)
{
	struct task_struct *p;

	for_each_process(p) {
		if (p->pid != pid) 
			continue;

		if (!wake_up_process(p)) {
			pr_info("%s: process PID=%d already running\n", __func__, p->pid);
		} else {
			pr_info("%s: process PID=%d is woken up\n", __func__, p->pid);
		}

		pid = 0;
		break;
	}

	if (pid)
		pr_info("%s: PID=%d not found\n", __func__, pid);
}

static pid_t pid;
module_param(pid, int, S_IRUGO);

static int find_load(void)
{
	enumerate_processes();
	wakeup_process(pid);

	return -ECANCELED;
}

static void find_unload(void)
{
	printk(KERN_INFO "%s\n", __func__);
}

module_init(find_load);
module_exit(find_unload);
