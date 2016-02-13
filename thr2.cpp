#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thr1(void *pArg)
{
	pthread_mutex_lock( static_cast<pthread_mutex_t*>(pArg) );
	return (void*)5;
}

void* thr2(void *pArg)
{
	pthread_spin_lock( static_cast<pthread_spinlock_t*>(pArg) );
	return (void*)6;
}

void* thr3(void *pArg)
{
	pthread_rwlock_wrlock( static_cast<pthread_rwlock_t*>(pArg));
	return (void*)7;
}

void* thr4(void *pArg)
{
	pthread_rwlock_rdlock( static_cast<pthread_rwlock_t*>(pArg));
	return (void*)8;
}

int main(int argc, char **argv)
{
	FILE *f = fopen("/home/box/main.pid", "w");	if( f == NULL ) { printf("fopen err\n"); return -1; }
	if( fprintf(f, "%ld\n", (long)getpid()) < 0 ) { printf("fprintf err\n"); return -1; }
	fflush(f);
	fclose(f);

	pthread_t id1, id2, id3, id4;

	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&m);
	if( pthread_create(&id1, nullptr, thr1, &m) != 0 )	{ printf("pthread_create 1 err\n"); return -1; }

	pthread_spinlock_t s;
	pthread_spin_init(&s, PTHREAD_PROCESS_PRIVATE);
	pthread_spin_lock(&s);
	if( pthread_create(&id2, nullptr, thr2, (void*)&s) != 0 )	{ printf("pthread_create 2 err\n"); return -1; }

	pthread_rwlock_t r = PTHREAD_RWLOCK_INITIALIZER;
	pthread_rwlock_t r2 = PTHREAD_RWLOCK_INITIALIZER;
	pthread_rwlock_wrlock(&r);
	pthread_rwlock_wrlock(&r2);
	if( pthread_create(&id3, nullptr, thr3, &r) != 0 )	{ printf("pthread_create 3 err\n"); return -1; }
	if( pthread_create(&id4, nullptr, thr4, &r2) != 0 )	{ printf("pthread_create 4 err\n"); return -1; }


	pause();

	void *res;
	pthread_mutex_unlock(&m);
	if( pthread_join(id1, &res) != 0 )	{ printf("pthread_join 1 err\n"); return -1; }
	pthread_mutex_destroy(&m);

	pthread_spin_unlock(&s);
	if( pthread_join(id2, &res) != 0 )	{ printf("pthread_join 2 err\n"); return -1; }
	pthread_spin_destroy(&s);

	pthread_rwlock_unlock(&r);
	pthread_rwlock_unlock(&r2);
	if( pthread_join(id3, &res) != 0 )	{ printf("pthread_join 3 err\n"); return -1; }
	if( pthread_join(id4, &res) != 0 )	{ printf("pthread_join 4 err\n"); return -1; }
	pthread_rwlock_destroy(&r);
	pthread_rwlock_destroy(&r2);

	printf("threads joined\n");
	return 0;
}
