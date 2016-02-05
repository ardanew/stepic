#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
        int fd = shm_open("/test.shm", O_CREAT|O_RDWR, 0666);
        if( fd < 0 ) { printf("shm_open err\n"); return -1; } 

        if( ftruncate(fd, 1024*1024) < 0 )
        { printf("ftruncate err\n"); return -1; }

        char *pMem = (char*)mmap(NULL, 1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if( pMem < 0 ) { printf("mmap err\n"); return -1; }

        for(char *p = pMem; p < pMem + 1024*1024; p++)
                *p = 13;

        pause();
        //munmap(pMem, 1024*1024);
        //shm_unlink("/test.shm");
        return 0;
}
