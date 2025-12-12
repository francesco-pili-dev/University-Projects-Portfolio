#include "header.h"
#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>

//Inizializzazione dei valori per ogni oggetto di tipo 'vehicle'
vehicle init_vec(int id, int direction, v_type type, int lane, int x, int y) //Inizializzazione veicolo----------------------------------------------------
{
	vehicle v;
	v.id = id;
	v.direction = direction;
	v.type = type;
	v.lane = lane;
	v.x=x;
	v.y=y;
	srand(time(NULL)+id);
	v.color = rand()%5+VEC1;		
	v.nChar_toPrint = 0;
	v.char_toStart = 0;
		
	return v;
} //--------------------------------------------------------------------------------------------------------------------------------------------------

//Funzione il cui compito consiste nel generare le coordinate dei veicoli, gestendo i casi di raggiungimento dei limiti dello schermo.
/*PARAMETRI:	
	*1. Oggetto di tipo veicolo già inizializzato nel main con valori di ascisse casuali nel range di MAXX
	*2. Oggetto pipe per comunicare i dati dell'elaborazione con il motore grafico
*/
void fun_vehicle(vehicle v, int *pipevec)
{
	long usleep_time = 100000-15000*diff;  //Il tempo in cui il processo è sospeso dipende dalla difficoltà: più è alta più i veicoli sono veloci
	close(pipevec[0]); //Chiusura pipe in lettura
	while(true){ //Inizio while true

		/*
			I veicoli hanno lo stesso numero di caratteri a prescindere che siano automobili o camion (in caso di macchine si aggiungono due spazi)
			Questo per evitare durante la stampa graduale ai bordi dello schermo che i camion vengano raggiunti delle auto
			(essendo più lunghi di queste)
			A lvello di collisioni i due spazi aggiuntivi non vengono considerati
		*/
		if(v.direction)  // Gestione del senso di marcia positivo -----------------------------------------------------
		{
			if(v.x >= maxx-TRUCK) // Se si raggiunge il bordo di destra il numero di caratteri da stampare viene decrementato
				v.nChar_toPrint --;
			else {
				if(v.x <= 0) //IL veicolo viene rigenerato all'inizio della corsia (x = 0)
				{
					v.nChar_toPrint++; //Si incremente il numero di caratteri da stampare
					if(!v.char_toStart)
						v.char_toStart = TRUCK-1; // Il carattere che viene stampato per primo è quello in ultima posizione
					else v.char_toStart --; //Viene aggiornato il carattere da cui far partire la stampa man mano che si avanza
				}
				else   //Situazione standard: il veicolo non si trova in nessuna froniera
				{
					v.nChar_toPrint = TRUCK; //Il numero di caratteri da stampare è uguale alla lunghezza del veicolo
					v.char_toStart = 0; //La stapa parte in ordine dal primo carattere
				}
			}
			if(!v.char_toStart) //Se il primo carattere da stampare è il primo (non ci si trova in nessuna frontiera) 
				v.x++;          // la coordinata x può essere incrementata

			//Ci si trova al bordo destro dello schermo: il veicolo può essere generato in una corsia differente
			if(v.x == maxx){
				srand(time(NULL)+v.y);
				int randlane = rand()%5+1;
				int randtype = rand()%3;
				v.color = rand()%5+VEC1; 
				v.type = (randtype)?CAR:TRUCK;  //Eventualmente viene cambiato il tipo di veicolo: auto 66% | camion 33%
				v.x = 0;
				if(randlane)  //Se si deve cambiare corsia
				{
					/* ALGORITMO DI CAMBIO CORSIA EVITANDO COLLISIONI:
							*IL veicolo viene rigenerato in una corsia avente stesso senso di marcia
							*Il modulo controlla se il numero di corsie è dispari o pari:
									se è pari possono essere considerate tutte le corsie;
									se è dispari la corsia centrale non viene considerata per la rigenerazione
							*La gestione è differente nel caso in cui il numero di corsie sia 5 (NON PREVISTO - problemi di prestazioni) 
					*/
					if(nlanes < 5){
						if(v.y <= first_lane + (nlanes/2))
						{
							v.y += nlanes + ((nlanes%2)?1:0);
						}
						else if(v.y >= first_lane + nlanes){
							v.y -= nlanes + ((nlanes%2)?1:0);
						}
					}
					else {
							if(v.y == first_lane)
							v.y = last_lane;
						else if(v.y == last_lane)
							v.y = first_lane;
						else if(v.y == first_lane + 2)
							v.y = last_lane-2;
						else if(v.y == first_lane - 2)
							v.y = last_lane+2;
					}
					
				}
			}
		}//Fine gestione senso di marcia positivo-----------------------------------------------------------------
		else
		{ //Inizio gestione senso di marcia inverso----------------------------------------------------------------
			if(v.x <= maxx-1 && v.x > maxx-TRUCK) // Il veicolo è appena stato generato all'inizio della corsia (bordo destro)
				v.nChar_toPrint++;
				if(v.nChar_toPrint > TRUCK)
					v.nChar_toPrint = TRUCK;

			else
			{
				if(v.x<=0 && v.x<TRUCK)   //Il veicolo sta uscendo dallo schermo (bordo sinistro)
				{
					v.nChar_toPrint--;   //Il numero di caratteri da stampare viene decrementato  
					v.char_toStart++;    // Il primo carattere da cui far partire la stampa viene aggiornato
				}
				else  //Situazione standard
				{
					v.char_toStart = 0;
					v.nChar_toPrint = TRUCK;
				}				
			}


		if(v.x<=0 && v.nChar_toPrint == 0){
			v.x=maxx;
			srand(time(NULL)+v.y);
			int randlane = rand()%5+1;
			int randtype = rand()%3;
			v.type = (randtype)?CAR:TRUCK;

			if(randlane)
			{
				if(nlanes < 5)
				{
					if(v.y <= first_lane + (nlanes/2))
					{
						v.y += nlanes + ((nlanes%2)?1:0);
					}
					else if(v.y >= first_lane + nlanes){
						v.y -= nlanes + ((nlanes%2)?1:0);
					}
				}
				else {
						if(v.y == first_lane)
							v.y = last_lane;
						else if(v.y == last_lane)
							v.y = first_lane;
						else if(v.y == first_lane + 2)
							v.y = last_lane-2;
						else if(v.y == first_lane - 2)
							v.y = last_lane+2;
					}
			}

			v.color = rand()%5+VEC1;
		}
		
		if(!v.char_toStart)
			v.x--;	

		} //Fine gestione senso di marcia inverso

		//Le nuove coordinate del veicolo vengono scritte su pipe
		write(pipevec[1], &v, sizeof(vehicle));
		usleep(usleep_time);
		
	} //Fine while true
} //Termine fun_vehicle---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Sottofunzione di gameArea: supporto alla stampa dei veicoli ---------------------------------------------------------------
void printVehicle(vehicle v, char* s)  
{
	/*STAMPA:
		* Viene effettuato un for per ogni carattere dello sprite passato come parametro, che dipende da: tipo di veicolo, direzione
		* Il numero di caratteri da stampare viene indicato dalla variabile propria di ogni veicolo 'nChar_toPrint
		* La variabile 'char_toStart' identifica la posizione del primo carattere da cui far partire la stampa in modo progressivo
	*/
	for(int i=0; i<v.nChar_toPrint; i++)
	{
		if(s[(v.char_toStart)?v.char_toStart:i] == '_'){  //Selezione dei caratteri ca colorare
			wattron(gamewin, COLOR_PAIR(v.color));
			mvwaddch(gamewin, v.y, v.x + i, ' ');
			wattroff(gamewin, COLOR_PAIR(v.color));
		}
		else
			mvwaddch(gamewin, v.y, v.x + i, s[(v.char_toStart)?v.char_toStart:i]);
		if(v.char_toStart) v.char_toStart++;
	}
}//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


