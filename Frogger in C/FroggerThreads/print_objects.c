#include "fheader.h"
#include<ncurses.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<locale.h>
//#include<pthread.h>

/*
    IL file in questione contiene le funzioni di gestione della stampa degli oggetti dinamici (veicoli, rana, tronchi, proiettili)
*/

//Cicla sui veicoli presenti: assegna lo sprite per ogni veicolo in base alla direzione e al tipo, successivamente lo stampa
void print_vehicles(vehicle vecs[], char sprites[][MAXVEHICLE_LEN+1])
{
    int cat;
    for(int i=0; i<(nlanes*nVehicles); i++)
    {
        if(vecs[i].direction && vecs[i].type == CAR) cat=0;
        if(!vecs[i].direction && vecs[i].type == CAR) cat=1;
        if(vecs[i].direction && vecs[i].type == TRUCK) cat=2;
        if(!vecs[i].direction && vecs[i].type == TRUCK) cat=3;

        //Per ogni veicolo viene richiamata la funzione di stampa del singolo
        printVehicle(vecs[i],sprites[cat]); //Stampa veicolo
    }
}
//-------------------------------------------------------------------------------------------------------------------------

//Stampa dell'oggetto rana: gesitsce inoltre la coordinate se la rana si trova sopra un tronco
int print_frog(object *frog, int frog_on_log, int *previous_logx, object logs[], int idlog)
{   
    /*
        *'frog_on_log' è una variabile che assume:
            -valore 0 se la rana non è sopra un tronco
            -valore 1 se la rana è sopra il tronco e il tronco si sta muovendo da sinistra verso destra
            -valore -1 se la rana è sopra il tronco e il tronco si sta muovendo da destra verso sinistra

        *Se frog_on_log ha valore diverso da 0, questo viene sommato alle coordinate della rana globali e locali;
        * ma solo se la coordinata del tronco in x salvata su 'previous_logx' al momento in cui la rana è salita sul tronco è
        * diversa da quella appena letta dalle coordinate condivise, per sincronizzarne il movimento

        *idlog indica l'identificatore del tronco su cui la rana è salita
    */

    if(frog_on_log && *previous_logx != logs[idlog].x)
    {
        frog->x += frog_on_log;

        pthread_mutex_lock(&frog_mutex);
            frog_data.x += frog_on_log;
        pthread_mutex_unlock(&frog_mutex);
        //Se c'è stato un movimento del tronco previous_logx viene aggiornato con quella coordinata 
        *previous_logx = logs[idlog].x;
    }
    //Stampa
    wattron(gamewin, COLOR_PAIR(1));
    mvwprintw(gamewin, frog->y, frog->x, "*^*");
    wattroff(gamewin, COLOR_PAIR(1));

}
//-----------------------------------------------------------------------------------------------------------------