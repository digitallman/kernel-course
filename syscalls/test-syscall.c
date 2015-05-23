
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

#define NR_printk_int 545

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;
	
	int ret = atoi(argv[1]);
	
	ret = syscall(NR_printk_int, ret);
	if (ret < 0)
		perror("NR_printk_int");	

	return 0;
}
