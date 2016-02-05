#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
        key_t key = ftok("/tmp/mem.temp", 1);
        if( key < 0 ) { printf("ftok() err\n"); return -1; }

        int id = shmget(key, 1024*1024, IPC_CREAT | 0666);
        if( id < 0 ) { printf("shmget() err\n"); return -1; }

        char *pMem = shmat(id, NULL, 0);
        for(char *p = pMem; p < pMem + 1024*1024; p++)
                *p = 42;

        pause();
        return 0;
}

