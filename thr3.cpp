#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct Cond
{
	pthread_mutex_t m;
	pthread_cond_t c;
};

void* thr(void *pArg)
{

//	printf("starting wait..\n");
	struct Cond *pCond = static_cast<Cond*>(pArg);
	pthread_cond_wait(&pCond->c, &pCond->m);
//	printf("wait stopped\n");

	return (void*)5;
}

void* thr2(void *pArg)
{
	printf("waiting for barrier (thr)\n");
	pthread_barrier_wait(static_cast<pthread_barrier_t*>(pArg));
	printf("barrier received (thr)\n");

	return (void*)6;
}


int main(int argc, char **argv)
{
	pid_t pid = getpid();
	FILE *f = fopen("/home/box/main.pid", "w");
	if( f == NULL ) {printf("fopen err"); return -1;}
	if(fprintf(f, "%ld\n", (long)pid) < 0) { printf("err fprintf\n"); return -1;}

	Cond cond;
	cond.c = PTHREAD_COND_INITIALIZER;
	cond.m = PTHREAD_MUTEX_INITIALIZER;

	pthread_t id;
	if( pthread_create(&id, nullptr, thr, &cond) != 0 )
	{ printf("pthread_create err\n"); return -1; }

	pthread_t id2;
	pthread_barrier_t b;
	pthread_barrier_init(&b, NULL, 2);

	if( pthread_create(&id2, nullptr, thr2, &b) != 0 )
	{ printf("pthread_create 2 err\n"); return -1; }
	

	pause();
//	printf("sleeping...\n");
//	sleep(2);
//	printf("signalling...\n");

	// do signal
	pthread_cond_signal(&cond.c);

	void *res;
	if( pthread_join(id, &res) != 0 )
	{ printf("pthread_join err\n"); return -1; }
	pthread_mutex_destroy(&cond.m);
	pthread_cond_destroy(&cond.c);


//	printf("waiting for barrier (main)\n");
//	sleep(2);
	pthread_barrier_wait(&b);
//	printf("barrier rcvd (main)\n");
	pthread_barrier_destroy(&b);

	//2nd thread
	if( pthread_join(id2, &res) != 0 )
	{ printf("pthread_join 2 err\n"); return -1; }
	


	printf("thread joined\n");
	return 0;
}
