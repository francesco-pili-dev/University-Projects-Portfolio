
#include "fheader.h"
#include<ncurses.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>

//Processo rana ----------------------------------------------------------------------
void *funcFrog(void *args)
{
    object frog_coords;
    frog_coords.x = maxx/2-1;
    frog_coords.y = maxy-2;
    frog_coords.signal = 0;

	//Variabile utilizzata per registrare il comando digitato dall'utente
    int choice;
	//Tramite dei timer viene controllato lo sparo: l'utente potrà premere il comando 'sparo' una volta ogni secondo
    time_t start_timer;
    time_t current_timer;
	//----------------------------------------------------------------------------------------------------

    start_timer = time(NULL);

        while(true){  //Inizio while true ------------------

			//Lettura delle coordinte globali della rana, in caso di modifica da parte di gameArea 
			pthread_mutex_lock(&frog_mutex);
        		frog_coords = frog_data;
    		pthread_mutex_unlock(&frog_mutex);

			//Le funzioni di ncurses sono controllate da un mutex dedicato
            pthread_mutex_lock(&mutexcurses);
            	wtimeout(gamewin, 0);
            	choice = wgetch(gamewin);
            pthread_mutex_unlock(&mutexcurses);

            frog_coords.c = 'f';
            switch(choice){  //Inizio switch
                
                case KEY_LEFT:
                    if(frog_coords.y>=last_log_y&&frog_coords.y<= first_lane-1)
                        frog_coords.x -= 3;
                    else
                         frog_coords.x -=1;
                     if(frog_coords.x < 1)
                        frog_coords.x = 0;
                    break;

                case KEY_RIGHT:
                    if(frog_coords.y>=last_log_y&&frog_coords.y<= first_lane-1)
                        frog_coords.x += 3;
                    else
                        frog_coords.x += 1;

                    if(frog_coords.x > maxx-3)
                        frog_coords.x = maxx-3;
                    break;

                case KEY_UP:
                    frog_coords.y -= 2;    
                    break;

                case KEY_DOWN:
                    frog_coords.y +=2;
                    if(frog_coords.y > maxy-2)
                        frog_coords.y = maxy-2;
                    break;

                case SHOOT:
					//Se l'utente esprime la volontà di sparare, viene controllato il valore attuale del timer
                    current_timer = time(NULL);
					//La scelta del comando è permessa solo se è passato almeno un secondo dal precedente sparo
                    if(current_timer - start_timer > 0){
                        fprintf(fp, "%lu Inviato comando sparo\n", time(NULL));
						//la variabile c con valore 'b', letta da area gioco, permette di riconoscere il comando
                        frog_coords.c='b';
						//Il timer viene resettato
                        start_timer = time(NULL);	
                    }
                    break;

                case EXIT:
					//Tasto QUIT: l'uscita dal gioco è gestita dal thread rana
                    frog_coords.c = 'q';
                    break;
                /*default:
                    choice = 0
                    break;*/
                        
            } //termine switch --------------------------------------------------

			//Le coordinate condivise vengono aggiornate
            pthread_mutex_lock(&frog_mutex);
                frog_data = frog_coords;
            pthread_mutex_unlock(&frog_mutex);
            usleep(80000);

        }//Termine while true ----------------------------------------------------------------------

} // Termine funzione Thread rana ------------------------------------------------------------------------------------------------------------------------------------------------------------------
