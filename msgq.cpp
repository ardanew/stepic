#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>

struct message {
 long mtype;
 char mtext[80];
};

int main(int argc, char **argv)
{
 const char SYNCFNAME[] = "/tmp/msg.temp";
 FILE *f = fopen(SYNCFNAME, "w");
 if( f == NULL ) { printf("fopen err\n"); return -1; }
 fclose(f);
 key_t key = ftok(SYNCFNAME, 1);
 if( key == -1 ) { printf("ftok err\n"); return -1; }

 int msgid = msgget(key, (IPC_CREAT | 0666) );
 if( msgid == -1 ) { printf("msgget error\n"); return -1; }

 message buf;
 ssize_t rcvRes = msgrcv(msgid, &buf, sizeof(buf.mtext), 0, 0);
 if( rcvRes < 0 ) { printf("msgrcv error\n"); return -1; }

 const char MSGFILENAME[] = "/home/box/message.txt";
 f = fopen(MSGFILENAME, "w");
 if( fprintf(f, "%s\n", buf.mtext) < 0 ) { printf("fprintf err\n"); return -1; }
 fflush(f);
 fclose(f);

 return 0;
}