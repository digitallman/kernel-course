#include <linux/module.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

#define NUM_KTHREADS 5

static struct task_struct *my_threads[NUM_KTHREADS];

#define sleep_msec(msecs) schedule_timeout_interruptible(msecs_to_jiffies(msecs))
#define INC_BAD
//#define INC_ATOMIC
//#define INC_SPINLOCK
//#define INC_MUTEX
//#define INC_SEM

#if defined(INC_BAD)
#include <linux/atomic.h>

static atomic_t var;

/* This will give different result everytime */
static int kthread_func_inc(void *arg)
{
	int i;

	for (i = 0; i < 100; i++) {
		int tmp = atomic_read(&var) + 1;
		atomic_set(&var, tmp);
		sleep_msec(1);
	}

	return 0;
}

#elif defined(INC_ATOMIC)
#include <linux/atomic.h>

static atomic_t var;

/* This is a consistent way with guaranteed atomicity of the increment */
static int kthread_func_inc(void *arg) 
{
	int i;

	for (i = 0; i < 100; i++) {
		atomic_inc(&var);
		sleep_msec(1);
	}
}

#elif defined(INC_SPINLOCK)
/* Demonstration of atomic increment with spinlock */
static DEFINE_SPINLOCK(inc_lock);

static int var;

static int kthread_func_inc(void *arg)
{
	int i;

	/* Don't sleep under spinlock! */
	for (i = 0; i < 100; i++) {
		spin_lock(&inc_lock);
		var++;
		spin_unlock(&inc_lock);
	}

	return 0;
}

#elif defined(INC_MUTEX)
/* Demonstration of atomic increment with mutex */
static DEFINE_MUTEX(inc_mutex);

static int var;

static int kthread_func_inc(void *arg)
{
	int i;

	for (i = 0; i < 100; i++) {
		mutex_lock(&inc_mutex);
		var++;
		sleep_msec(1);
		mutex_unlock(&inc_mutex);
	}

	return 0;
}

#elif defined(INC_SEM)
#include <linux/semaphore.h>

static struct semaphore inc_sem = __SEMAPHORE_INITIALIZER(inc_sem, 1);

static int var;

static int kthread_func_inc(void *arg)
{
	int i;

	for (i = 0; i < 100; i++) {
		down(&inc_sem);
		var++;
		sleep_msec(1);
		up(&inc_sem);
	}

	return 0;
}
#endif

static int (*kthread_func[])(void *) = {
	&kthread_func_inc,
	&kthread_func_inc,
	NULL
};
 
static int kthreads_load(void)
{
	int i;
	pr_info("%s: testing locking\n", __func__);

	for (i = 0; i < NUM_KTHREADS && kthread_func[i]; i++) {
		my_threads[i] = kthread_create(kthread_func[i], NULL, "%s/%d",
						"kthread", i);
		if (IS_ERR(my_threads[i])) {
			pr_info("%s: cannot create kthread/%d\n", __func__, i);
			return PTR_ERR(my_threads[i]);
		}

		/* Bind to CPU */
		kthread_bind(my_threads[i], i);

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
#if defined(INC_ATOMIC) || defined(INC_BAD)
	pr_info("%s: var = %d\n", __func__, atomic_read(&var));
#else
	pr_info("%s: var = %d\n", __func__, var);
#endif
}

module_init(kthreads_load);
module_exit(kthreads_unload);
