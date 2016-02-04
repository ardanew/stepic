#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char **argv)
{
	key_t key = ftok("/tmp/sem.tmp", 1);
	if( key < 0 ) { printf("ftok err\n"); return -1; }
	
	int id = semget(key, 16, 0666 | IPC_CREAT);
	if( id < 0 ) { printf("semget err\n"); return -1; }
	
	unsigned short xVals[16];
	for(unsigned short i = 0; i < 16; i++)
		xVals[i] = i;
	
	semun arg;
	arg.array = xVals;
	
	if( semctl(id, 0, SETALL, arg) < 0 )
		{ printf("semctl err\n"); return -1; }
	
	return 0;
}