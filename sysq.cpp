#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <string.h>

int main(int argc, char **argv)
{
	mqd_t qHandle = mq_open("/test.mq", O_CREAT | O_RDONLY);
	if( qHandle < 0 ) { printf("mq_open() error\n"); return -1; }
	
	char szBuf[255];
	bzero(szBuf, sizeof(szBuf));
	int nRecv = mq_receive(qHandle, szBuf, sizeof(szBuf), nullptr);
	if( nRecv < 0 ) { printf("mq_receive() error\n"); return -1; }
	
	FILE *f = fopen("/home/box/message.txt", "w");
	if( f == NULL ) { printf("fopen() error\n"); return -1; }
	
	int nRes = fwrite(szBuf, nRecv, 1, f);
	if( nRes < 0 ) { printf("fwrite() error\n"); return -1; }
	
	fflush(f);
	fclose(f);
	return 0;
}