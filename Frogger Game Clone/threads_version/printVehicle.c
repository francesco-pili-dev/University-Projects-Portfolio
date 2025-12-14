#include "fheader.h"
#include<ncurses.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

void printVehicle(vehicle v, char* s)  //Gestione stampa a video dei veicoli --------------------------------------------------------------------------------------------------------------------
{

	for(int i=0; i<v.nChar_toPrint; i++)
	{
		
		if(s[(v.char_toStart)?v.char_toStart:i] == '_'){
            //pthread_mutex_lock(&mutexcurses);
                wattron(gamewin, COLOR_PAIR(v.color));
                mvwaddch(gamewin, v.y, v.x + i, ' ');
                wattroff(gamewin, COLOR_PAIR(v.color));
            //pthread_mutex_lock(&mutexcurses);   
		}
		else {
            //pthread_mutex_lock(&mutexcurses);
			mvwaddch(gamewin, v.y, v.x + i, s[(v.char_toStart)?v.char_toStart:i]);
            //pthread_mutex_unlock(&mutexcurses);
        }
		if(v.char_toStart) v.char_toStart++;
	}
}//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
