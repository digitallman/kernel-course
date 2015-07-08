/* fork() vs pthread_create() */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

static pthread_t thread;

void *thread_func(void *opaque)
{
	printf("Thread func, pid=%u, ppid=%u\n", getpid(), getppid());
	getchar();
	printf("Thread func finished!\n");
}

int main()
{
	printf("Invoking fork()\n");
	switch (fork()) {
	case 0:
		printf("Child process, pid=%u, ppid=%u\n", getpid(), getppid());
		getchar();
		printf("Child process finished!\n");
		break;
	case -1:
		perror("Couldn't create child process\n");
		return 1;
	default:
		printf("Main process, pid=%u, ppid=%u\n", getpid(), getppid());
	
		if (pthread_create(&thread, NULL, thread_func, NULL))
			printf("Couldn't create POSIX thread\n");
	
		if (wait(NULL) == -1)
			printf("wait() failed\n");

		pthread_join(thread, NULL);
		
		getchar();
		printf("Main process finished!\n");
	}

	return 0;
}
