#include "fheader.h"
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk *enemyhit;
Mix_Chunk *frogriverhit;

/*Si occupa di controllare la presenza di collisioni con tutti gli oggetti che possono collidere con il veicolo. In questo caso;
	*Collisioni rana-veicolo - restituisce 1 in caso di collisione, il chiamante riceve il valore e inizializza la prossima manche
	*Collisioni proiettili-veicolo - se rileva la collisione termina il thread associato e aggiorna le variabili di controllo
*/
int vecsCollisions(vehicle *vecs, object frog, object bullets[], pthread_t bullets_id[]){
    int score = 0;
	
	//Ciclo sul numero di veicoli ----------------------------------------------------------------------------
    for(int i=0; i<(nlanes*nVehicles); i++)
	{

		//Gestione collisioni rana - veicolo -----------------------------------------------
		if(frog.y == vecs[i].y)
		{
			if( (frog.x >= vecs[i].x && frog.x <= vecs[i].x+vecs[i].type) ||
					(frog.x+2 >= vecs[i].x && frog.x <= vecs[i].x+vecs[i].type))
				return 1; //Collisione rilevata
		}
		//Termine gestione collisione rana-veicolo ----------------------------------------------------
	
		//Gestione collisione proiettile - veicolo -----------------------------------------------------
		for(int b = 0; b<MAX_NBULLETS; b++)
		{
			
			//Se nello slot specificato non c'è un proiettile si va alla prossima iterazione
			if((bullets[b].signal != -1) && (bullets[b].y == vecs[i].y) && 
				(bullets[b].x >= vecs[i].x && bullets[b].x <= vecs[i].x+vecs[i].type))
			{
				//COLLISIONE RILEVATA
			 	destroy_bullet(b, &bullets[b], &bullets_id[b]);
			}		     
		}
		//Termine gestione collisione proiettile - veicolo ------------------------------------------	
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------


/*BULLETS_COLLISION - Si occupa di controllare la presenza di collisioni con alcuni oggetti che possono collidere con un proiettile. In questo caso:
	*collisioni proiettile(nemico)-rana. Restituisce 1 in caso di collisione, il chiamante riceve il valore e inizializza la prossima manche;
	*collisioni proiettile(rana)-nemico. Il nemico viene eliminato se ha un solo hp (score aggiornato);
	*collisioni tra proiettili, proiettile(rana)-proiettile(nemico).
* OGNI COLLISIONE DETERMINA LA DISTRUZIONE DEL PROIETTILE
* Le collisioni proiettile - veicolo sono gestite dalla precedente funzione per motivi di efficienza.
* Le collisioni tra proiettili sparati da due nemici non possono collidere, poiché viaggiano alla stessa velocità.
*/
int bullets_collisions(object bullets[], pthread_t bullets_id[], object frog, int log_map[], int *score, int *n_enemies, time_t *start_enemy_clock, object logs[])
{
	enemyhit = Mix_LoadWAV("sounds/enemyhit.mp3");
	frogriverhit = Mix_LoadWAV("sounds/frogriverhit.mp3");
	//Ciclo principale sui proiettili ------------------------------------------------------------------------------------------------------------------------
	for(int i=0;i<MAX_NBULLETS; i++)
	{
		//Le collisioni possono avvenire soltanto se la casella specificata dall'indice i contiene effettivamente un proiettile
		if(bullets[i].signal != -1)
		{
			//Collisioni proiettili nemici(simbolo 'B') -rana -------------------------------------------------------------------------------------
			if(bullets[i].y == frog.y && (bullets[i].x >= frog.x && bullets[i].x <= frog.x+2) && bullets[i].c == 'B')
			{
				//Collisione rilevata

				//Il proiettile viene distrutto, il thread associato terminato -------
				destroy_bullet(i, &bullets[i], &bullets_id[i]);
				//--------------------------------------------------------------------

				//Rana perde una vita
				return 1;	
			}
			//Termine gestione collisioni proiettile nemico - rana ----------------------------------------------------------------------------------


			//Gestione collisioni proiettile rana(simbolo 'b') - nemico ------------------------------------------------------------------------------

			for(int l=0; l<nlogs; l++) //Ciclo sui nemici
			{		
				if(log_map[l] > 0) //Si controlla la possibile collisione solo se l'l-esimo tronco è ospitato da un nemico
				{
					if(bullets[i].c == 'b' &&(bullets[i].y == logs[l].y+1) &&
					((bullets[i].x >= logs[l].x+((LOGLEN/2)-1))) && (bullets[i].x <= logs[l].x+((LOGLEN/2)+2)))
					{ //Collisione
						//Il proiettile viene distrutto, il thread associato terminato -------
						destroy_bullet(i, &bullets[i], &bullets_id[i]);
						//--------------------------------------------------------------------
						Mix_PlayChannel(-1,enemyhit,0);
						//Il nemico perde una vita -------------------------------------------------------------------------------------

						/*il valore di log_map associato all'i-esimo tronco, ha valore maggiore di 0 se vi è presente un nemico, in cui:
							* valore 2 - il nemico ha due vite (solo per difficoltà HARD e superiore)
							* valore 1 - il nemico ha una vita rimanente
							* valore 0 - nessun nemico sul tronco
						*/
						log_map[l] --; //Il numero di vite del nemico viene decrementato
						//Se il nemico ha perso tutte le vite dopo esser stato colpito, viene decrementato il numero di nemici
						if(log_map[l]==0){
							*start_enemy_clock = time(NULL);
							*n_enemies = *n_enemies -1;
							*score = *score + 15 + diff-1; 
						}
						else *score+=5 + diff-1;
					}
				}
				//fprintf(fp, "Collisione con proiettile %d: Terminazione forzata con kill\n", b); 
			}
			//Termine gestione collisioni proiettile rana - nemico ---------------------------------------------------------------

			//Gestione collisioni proiettile nemico ('B') - proiettile rana ('b') -------------------------------------------------------------

			for(int b = i+1; b<MAX_NBULLETS; b++)
			{
				if(bullets[b].signal!=-1 && bullets[b].x == bullets[i].x && bullets[b].y == bullets[i].y){
					//Collisione rilevata
					*score = *score +10;

					//I proiettili viene distrutti, i threads associati terminati -------
						destroy_bullet(i, &bullets[i], &bullets_id[i]);
						destroy_bullet(b, &bullets[b], &bullets_id[b]);
					//--------------------------------------------------------------------
				}
					
			}
			//Termine gestione collisioni tra proiettili --------------------------------------------------------------------------------------


		} // Parentesi dell'if controllo sulla validità del proiettile
	}
	//Termine ciclo principale ------------------------------------------------------------------------
	return 0;
}

//Termina il thread identificato da 'bull_id', aggiorna le variabili globali e locali del proiettili ----------------------------------------
void destroy_bullet(int bull_id, object *bullet, pthread_t *bull_thread_id)
{
	//Aggiornamento variabili locali -------------------------------------
		bullet->y = -1; //Indicatore di terminazione forzata
		bullet->signal = -1; //Slot liberato per creazione di atri proiettili
	//-----------------------------------------------------------------------

	//Aggiornamento variabii globali ----------------------------------------
	pthread_mutex_lock(&bull_mutex[bull_id]);
		bullets_data[bull_id].y = -1; //Indicatore di terminazione forzata
		bullets_data[bull_id].signal = -1; //Slot liberato per creazione di atri proiettili
	pthread_mutex_unlock(&bull_mutex[bull_id]);
	//----------------------------------------------------------------------

	//Kill del thread associato
	pthread_cancel(*bull_thread_id);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

int river_collisions(object frog, object logs[], int log_map[], int *frog_on_log, int *previous_logx, int *idlog)
{
    for(int i=0; i<nlogs; i++) //Gestione collisione rana - tronchi -------------------------------------
    {
        if(frog.y == logs[i].y+1)
        {
            if(frog.x >= logs[i].x && frog.x <= logs[i].x+(LOGLEN-1))
            {	
                //Gestione collisione rana - nemico
                if(log_map[i]>0)
                {
                    return 2; //Rana perde una vita a causa di un nemico
                    
                }//Fine gestione collisione rana - nemico
                else
                {	//La rana si trova sopra un tronco				
                    *frog_on_log = logs[i].signal;
                    *idlog = i;
                    *previous_logx = logs[i].x;
                    log_map[i] = -1;
                    return 0;
                    break;
                }
            }
            else
            {
				Mix_PlayChannel(-1,frogriverhit,0);
                if(log_map[i]<0)					
                    log_map[i] = 0;
                return 1;     //Rana perde una vita affogando
            }
        }
        else {
            if(log_map[i]<0)					
                log_map[i] = 0;
            *frog_on_log = 0;
            //return 0;
        }
    } // Termine gestione collisione rana-tronchi ------------------------------------------
    return 0;
}

