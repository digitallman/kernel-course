#include <linux/module.h>

MODULE_LICENSE("GPL");

static int helper_load(void)
{
	int ret;

	pr_info("%s\n", __func__);
	
	char *env[] = {
//		"HOME=/",
       //         "PATH=/sbin:/bin",
                NULL
        };

        char *argv[] = {
               "/home/me/kernel-course/processes/user-helper/test.sh",
                NULL
        };

	ret = call_usermodehelper(argv[0], argv, env, UMH_WAIT_EXEC);
	if (ret) {
		pr_err("%s: call_usermodehelper failed: %d\n", __func__, ret);
		return ret;
	}
		
	return 0;
}

static void helper_unload(void)
{
}

module_init(helper_load);
module_exit(helper_unload);
