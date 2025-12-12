#include "header.h"
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>

//Genera le coordinate dell'oggeto 'tronco'
/*PARAMETRI:
    1.Pièpe su cui scrivere le coordinate aggiornate
    2. coordinata y corrispondente al corso del fiume su cui generare il trono
    3. direzione del tronco: è contraria alla flusso precedente (se esiste)
*/
void fun_log(int *pipelog, int y, int dir) //Inizio funzione di gestione dei processi tronco --------------------------------------------------------------------------------------------------------------
{
   
	object log;
	log.y = y;
	srand(time(NULL)+y);
    //Inizializzazione ddel tronco  ------------------------------------
    //la coordinata x viene randomizzata nel range della dimensione in larghezza della finestra di gioco
	log.x = (rand()%maxx/2) + LOGLEN;
	log.signal = dir;
	log.c = 'l';
    //---------------------------------------------------------------------
    //La difficoltà dei tronchi aumenta in base alla difficoltà
	long usleep_time = 100000-15000*diff;

    //Chiusura della pipe in lettura
	close(pipelog[0]);
	
    //Ciclo di generazione delle coordinate
	while(true)
	{
        //Il valore dell'ascissa viene incrementato o decrementato in base alla direzione del tronco
		log.x+=log.signal;
        //Se il tronco raggiunge uno dei bordi orizzontali dello schermo la sua direzione viene invertita
		if(log.x > (maxx-LOGLEN)-1 || log.x < 1)
			log.signal*=-1;

		//Scrittura su pipe
		write(pipelog[1], &log, sizeof(object));

		usleep(usleep_time);
		
	}

} //Termine funzione di gestione dei processi tronco ------------------------------------------------------------