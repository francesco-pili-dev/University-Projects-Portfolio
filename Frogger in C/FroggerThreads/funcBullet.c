#include "fheader.h"
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>

//Funzione Thread proiettie -------------------------------------------------------------------------------------------------------------------
void *funcBullet(void *args)
{
    
    object *bullet;
    bullet = (object*)args;
    fprintf(fp, "Creato Thread proiettile numero %d\n", bullet->id);
    long time = (bullet->c == 'b')?50000:75000;

    while(bullet->y != -1)  //Il Thread proiettile termina la sua esecuzione se raggiunge i limiti dello schermo
	{
		
		if(bullet->c == 'b')
		{
            //Il proiettile sparato dalla rana viaggia dal basso verso l'alto
			bullet->y--;
			if(bullet->y==0){
				bullet->y = -1;
                //Alla funzione chiamante viene comunicata la terminazione de Thread: nella casella associata dell'array di proiettili condiviso è
                //possibile assegnare nuovi proiettili		
                bullet->signal = -1;
            }	
		}
		else
		{
            //Il proiettile sparato dal nemico viaggia dall'alto verso il basso
			bullet->y++;
			if(bullet->y==maxy){
				bullet->y = -1;		
                bullet->signal = -1;
            }	
		}	
        //La traiettoria del proiettile è sempre verticale come da specifiche
        //Aggiornamento delle coordinate globali
		pthread_mutex_lock(&bull_mutex[bullet->id]);
            bullets_data[bullet->id] = *bullet;
        pthread_mutex_unlock(&bull_mutex[bullet->id]);
        fprintf(fp, "proiettile %d\n",bullet->id);
		usleep(time);
	}
    	
    //fprintf(fp, "Terminato Thread proiettile numero %d\n", bullet->id);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
