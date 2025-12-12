#include "fheader.h"
#include<ncurses.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<locale.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>

void *funcLog(void *args){
    object *log;
    log = (object*)args;
	srand(time(NULL)+log->y);
	log->x = (rand()%maxx/2) + LOGLEN;
	log->c = 'l';
	long usleep_time = 100000-15000*diff;
	fprintf(fp,"log id: %d x: %d y: %d direzione: %d\n",log->id,log->x,log->y,log->signal);
	while(true)
	{
		log->x+=log->signal;
		if(log->x > (maxx-LOGLEN)-1 || log->x < 1)
			log->signal*=-1;
	
        pthread_mutex_lock(&log_mutex[log->id]);
        logs_data[log->id] = *log;
        pthread_mutex_unlock(&log_mutex[log->id]);
		usleep(usleep_time);	
	}

}