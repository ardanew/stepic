#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thr(void *pArg)
{
	pid_t pid = getpid();
	printf("hello from thread, pid = %ld\n", (long)pid);
	FILE *f = fopen("/home/box/main.pid");
	if( f == NULL ) {printf("fopen err"); return -1;}
	if(fprintf(f, "%ld\n", (long)pid) < 0) { printf("err fprintf\n"); return -1;}
	pause();
	return (void*)5;
}

int main(int argc, char **argv)
{
	pthread_t id;
	//pthread_attr_t attr;
	if( pthread_create(&id, nullptr, thr, nullptr) != 0 )
	{ printf("pthread_create err\n"); return -1; }

	void *res;
	if( pthread_join(id, &res) != 0 )
	{ printf("pthread_join err\n"); return -1; }

	printf("thread joined\n");
	return 0;
}
