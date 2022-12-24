#include "lib_dbg.h"

#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>

#define KEY ((key_t)(1234))
#define SEGSIZE 1000
char* ap;
int msg_cnt = 0;

int dbg_open(){
	int id = shmget(KEY, SEGSIZE, IPC_CREAT | 0666);
	if (id < 0) return 1;

	ap = (char*) shmat(id, 0, 0);
	return 0;
}


void dbg_write(const char* buf)
{
	char msg[100];

	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);
	strftime(msg, 50, "%H:%M:%S", timeinfo);
	msg[49] = '\t';	
	
	printf("Temp value %s.\n", msg);

	memcpy(msg + 50, buf, 50);

	printf("Sending value %s...\n", msg);

	sprintf(ap + msg_cnt*100, "%s", msg);
	printf("Sent!\n");

	msg_cnt++;
	if(msg_cnt == 10) msg_cnt = 0;
}


void dbg_close(){
	shmdt (ap);
}



