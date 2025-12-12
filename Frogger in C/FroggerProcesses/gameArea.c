#include "header.h"
#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<fcntl.h>
#include<locale.h>
#include<math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int gameArea(int *pipevec, int *pipefrog, int *pipeobj,int score) //Gestione delle stampe --------------------------------------------------------------------------------------------------------------------
{
	//Caricamento musiche di gioco ----------------------------------------------------
	Mix_Music *gamemusic[4];
	Mix_Chunk *enemyspawn;
	Mix_Chunk *enemyhit;
	Mix_Chunk *frogbullet;
	Mix_Chunk *enemybullet;
	Mix_Chunk *cars;
    Mix_Chunk *frogcarhit;
	Mix_Chunk *frogenemyhit;
	Mix_Chunk *frogriverhit;
	
	gamemusic[0] = Mix_LoadMUS("sounds/froggertheme.mp3");
	gamemusic[1] = Mix_LoadMUS("sounds/froggertheme.mp3");
	gamemusic[2] = Mix_LoadMUS("sounds/gamemusic.mp3");
	gamemusic[3] = Mix_LoadMUS("sounds/darksoultheme.mp3");
	enemyspawn = Mix_LoadWAV("sounds/enemyspawn.mp3");
	enemyhit = Mix_LoadWAV("sounds/enemyhit.mp3");
	frogbullet = Mix_LoadWAV("sounds/frogbullet.mp3");
	enemybullet = Mix_LoadWAV("sounds/enemybullet.mp3");
	frogcarhit = Mix_LoadWAV("sounds/frogcarhit.mp3");
	frogenemyhit = Mix_LoadWAV("sounds/frogenemyhit.mp3");
	frogriverhit = Mix_LoadWAV("sounds/frogriverhit.mp3");
	Mix_PlayMusic(gamemusic[diff],1);
	Mix_VolumeMusic(15);
	//--------------------------------------------------------------------------------------

	//Strutture e variabili per la gestione dei veicoli -------------------------------------
	vehicle vehicles[nlanes * nVehicles];
	vehicle tempVehicle; //Utilizzata per la lettura da pipe
	int vread = 0; //Tiene conto dei veicoli letti

    char sprites[4][MAXVEHICLE_LEN+1];  //Array di stringhe utilizzato per assegnare
	strcpy(sprites[0], "____))  ");     //gli sprites ai veicoli in base al
	strcpy(sprites[1], "  ((____");     //tipo di veicolo (automobile/camion) e 
	strcpy(sprites[2], "______|]");     //al senso di marcia
	strcpy(sprites[3],  "[|______");
    //--------------------------------
	//----------------------------------------------------------------------------------------

	//Strutture e variabili per la gestione dell'oggetto rana --------------------------------
	object frog;
	frog.x = maxx/2-1; //Inizializzazione (posizione centrale bassa dello schermo)
	frog.y = maxy-2;

	object frog_movement; //Utilizzata per la lettura da pipe: legge il movimento
	frog_movement.x = 0;  //inviato dal processo rana e lo somma alle proprie coorinate locali
	frog_movement.y = 0;

	int frog_on_log = 0; //Specifica se la rana sta sopra un qualsiasi tronco
	int idlog = -1; //Specifica l'identtificatore del tronco su cui la rana è eventualmente salita
	//------------------------------------------------------------------------------------------
	
	ssize_t rl = 0;

	//Strutture e variabili per la gestione degli oggetti tronco -------------------------------
	object logs[nlogs]; 
	object tempobj;       //Utilizzato per la lettura da pipe

	int log_map[nlogs];           //Mappa gli oggetti presenti sopra ciasun tronco:
	for(int i=0; i<nlogs; i++)       //log_map[indice] = 0 -> tronco libero
		log_map[i] = 0;	             //log_map[indice] > 0 -> tronco occupato da un nemico				
						 			 //log_map[indice] = -1 -> tronco occupato dalla rana

	int n_enemies = 0; //Specifica il numero di nemici presenti contemporaneamente sullo schermo
	int enemy_hps = (diff >= HARD)?2:1;
	int previous_logx = 0; //Registra la coordinata x del tronco prima di una lettura da pipe
	//------------------------------------------------------------------------------------------


	//Strutture e variabili per la gestione degli oggetti proiettile  --------------------------
	int nbullets = 0;
	object bullets[MAX_NBULLETS];

	for(int i=0; i<MAX_NBULLETS; i++){ //La costante simbolica MAX_NBULLETS indica il massimo numero di proiettili presenti contemporaneamente possibili

		//la variabile signal in questo caso inizializzata a -1 indica che in quella specifica casella non è al momento occupata da altri proiettili 
		bullets[i].signal = -1; 
	}

	pid_t pidbullets[MAX_NBULLETS];
	//------------------------------------------------------------------------------------------

	//Strutture e variabili per la gestione degli oggetti tana  --------------------------------
	object den[N_DENS]; //Contiene le informazioni sulle tane conquistate e le relative coordinate
	fun_den(den);	//Inizializzazione tane 
	int denstaken = 0; //conta il numero di tane conquistate
	//-----------------------------------------------------------------------------------------

	//Altre variabili e strutture utilizzate all'interno della funzione ---------------------------

	int checkpoint = 0; //Contiene valore 1 se la rana supera l'autostrada
	int collision = 0;
	int hp = 3; //Vite iniziali della rana

	//Gestisce le interruzioni (vittoria, sconfitta, quit)
	interrup_type itype = -1;

	//Gestione dei punteggi
	int highscore = read_highscore(fph);  //Legge da file l'highscore: se il file non esiste highscore->0
	WINDOW *win_score = newwin(4, maxx, maxy, 0);  //Creazione della finestra contenente i punteggi
	init_pair(11, COLOR_WHITE, COLOR_DARK);
	wbkgd(win_score, COLOR_PAIR(12));

	//Temporizzazione ------------------------------------------------------

		//Timer di gioco, utiizzato per la gestione del cronometro
		time_t start_game_clock = time(NULL);
		time_t current_game_clock;
		//Viene dato un limite di tempo di 40 secondi al giocatore per ogni manche
		int time_limit = 40;
		int timer = 0;

		//Timer dei nemici: utilizzato per controllare il tempo tra un'apparizione di un nemico e la successiva (in caso di più nemici)
		time_t start_enemy_clock;  
		time_t current_enemy_clock;
		int enemy_timer=0;
		//In base alla difficoltà il tempo che intercorre tra una generazione e l'altra è minore
		int enemy_spawn_timelaps = 10 - (diff*1.5);

		//Timer dei proiettili nemici: vi sono n timer per n nemici. Utilizzato per calcolare il lasso di tempo tra uno sparo nemico e il successivo
		time_t start_timer_bullets[nlogs];
		time_t current_timer_bullets[nlogs];
	//---------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------

	init_structures(logs, vehicles);

	close(pipefrog[1]);
	close(pipevec[1]);
	
	while(!collision)  //Inizio ciclo ---------------------------------------------------------------------------------------------------
	{				
		//Lettura da pipe dell'oggetto rana --------------------------------
		
		//la pipe della rana è non bloccante
		ssize_t r;
			 r = read(pipefrog[0], &frog_movement, sizeof(object));
		
		if(r>0)  //Se vi è stata rilevata una lettura
		{	
			frog.x += frog_movement.x;  //Le coordinate della rana locali vengono sommate ai movimenti letti
			if(frog.x < 1) frog.x=1;
			if(frog.x > maxx-3) frog.x = maxx-3;  //Viene effettuato il controllo sulla validità delle coordinate (limiti di schermo)
			frog.y += frog_movement.y;
			if(frog.y < 1) frog.y = 1;
			if(frog.y > maxy-2) frog.y = maxy-2;
			//La variabile c rileva il comando premuto: 'f'->movimento semplice, 'b'->sparo, 'q'->quit
			frog.c = frog_movement.c;
		}
		//----------------------------------------------------------------
		
		//Lettura veicoli da pipe ---------------------------------------------
		ssize_t vl = read(pipevec[0], &tempVehicle, sizeof(vehicle));
		if(vl > 0)
			vehicles[tempVehicle.id] = tempVehicle;	//tempVehicle.id specifica la posizione della cella all'interno
													//dell'array locale di veicoli su cui andare a sovrascrivere
		if(vread < (nlanes*nVehicles))
			vread++;
		//---------------------------------------------------------

		//Lettura tronchi e proiettili --------------------------------------
		rl = read(pipeobj[0], &tempobj, sizeof(object));
		if(rl > 0)
		{
			if(tempobj.c == 'l')  //Comando tronco
				logs[(tempobj.y-(FIST_LOG_Y-1))/2] = tempobj;
			else {
				
				/*Signal = -2 -> La lettura non deve portare ad un aggiornamento:
				   test effettuato per gestire il caso in cui un proiettile venga distrutto ma
				   ci sia ancora un dato in memoria all'interno della pipe*/
				if(bullets[tempobj.signal].y != -2) 
					bullets[tempobj.signal] = tempobj;
				else
					bullets[tempobj.signal].y = 0;  //Se signal = -2 viene portato a 0 così da abilitare la lettura 
					                                //all'iterazione successiva
			}	
		}	
		//--------------------------------------------------------
		
		current_game_clock = time(NULL); //Viene calcolato il timer di gioco
		timer = current_game_clock - start_game_clock;

		wclear(gamewin);
		print_score(win_score,score,highscore);
		print_gamefield(hp, time_limit, timer, &start_game_clock, &frog, den);

		if(itype == WIN)
		{
			sleep(2);
			//Musica di vittoria
			break;
		}
		// Stampa e gestione collisioni ----------------------------------------------------------------------------
		
		//Si effettua un controllo sul checkpoint
		if(!checkpoint && frog.y < CHECKPOINT){
			checkpoint = 1;
			//I punteggi ottenuti sono più alti all'aumentare della difficoltà
			score = score+30+3*diff;
			start_enemy_clock = time(NULL); 
			//current_enemy_clock = time(NULL); //Parte il cronometro
			enemy_spawn_timelaps = 1; //La prima volta un nemico appare di default
		}

		if(vread == (nlanes*nVehicles)){  //Se tutti i veicoli sono stati letti, si può procedere alla stampa
			
			//Stampa a video dei veicoli ------------------------------------------------------------------
			int cat;
			for(int i=0; i<nlanes*nVehicles; i++)
			{
				//Assegnamento degli sprite
				if(vehicles[i].direction && vehicles[i].type == CAR) cat=0;
				if(!vehicles[i].direction && vehicles[i].type == CAR) cat=1;
				if(vehicles[i].direction && vehicles[i].type == TRUCK) cat=2;
				if(!vehicles[i].direction && vehicles[i].type == TRUCK) cat=3;
				//fprintf(fp, "x = %d y = %d id = %d\n", vehicles[i].x, vehicles[i].y, vehicles[i].id);
				printVehicle(vehicles[i],sprites[cat]); //Stampa veicolo
			}
			wattron(gamewin, COLOR_PAIR(2));
		    //Termine stampa dei veicoli ---------------------------------------------------------------


			//Ciclo sui tronchi: stampa e gestione collisione con i nemici ----------------------------
			for(int i=0; i<nlogs; i++)
			{
				wattron(gamewin, COLOR_PAIR(9));
				mvwprintw(gamewin, logs[i].y+1, logs[i].x, (logs[i].signal!=1)?"        @":"@        ");	
				wattroff(gamewin, COLOR_PAIR(9));	
				wattron(gamewin, COLOR_PAIR(2));
				if(log_map[i] > 0)
				{
					wattron(gamewin, COLOR_PAIR(3+log_map[i]-1));
					if(current_timer_bullets[i] - start_timer_bullets[i] == 3 && checkpoint)
						mvwprintw(gamewin, logs[i].y+1, logs[i].x+(LOGLEN/2)-1, "7o7");  
						//I nemici durante il secondo antecedente allo sparo fanno intendere la loro mossa aprendo la bocca
					else mvwprintw(gamewin, logs[i].y+1, logs[i].x+(LOGLEN/2)-1, "7_7");
					wattroff(gamewin, COLOR_PAIR(3+log_map[i]-1));
					wattron(gamewin, COLOR_PAIR(2));
				
						current_timer_bullets[i] = time(NULL);
							
							//Un nemico può sparare un proiettile ogni 3 secondi
							if(current_timer_bullets[i] - start_timer_bullets[i] > 3)
							{
								
								start_timer_bullets[i] = time(NULL);
								//Ad ogni proiettile è assegnata una cella dell'array di proiettile: 
									//Le prime 3 posizioni sono dedicate alla rana
									//Le posizioni dalla quarta alla settima sono dedicate ai nemici
								if(bullets[i+nlogs].signal == -1 && checkpoint)
								{
									Mix_PlayChannel(-1,enemybullet,0);
									pidbullets[i+nlogs] = fork();
									if(pidbullets[i+nlogs] == 0)
									{	
										//Creazione del processo proiettile
										fun_bullet(pipeobj,logs[i].x+
												(LOGLEN/2),logs[i].y,i+nlogs,'B');
										_exit(0);
									}
									break;
								}			
							}
						//}	
					//}// Fine generazione priettile nemici -------------------------------------

					//Collisione proiettile rana - nemico ----------------------------
					for(int b=0; b<MAX_NBULLETS; b++)
					{
						if(bullets[b].signal != -1) //Se vi è un proiettile in quella posizione si controllano le collisioni
						{
							if( bullets[b].c == 'b' &&(bullets[b].y == logs[i].y+1) &&
								((bullets[b].x >= logs[i].x+((LOGLEN/2)-1))) && (bullets[b].x <= logs[i].x+((LOGLEN/2)+2)))
							{ //Collisione
								Mix_PlayChannel(-1,enemyhit,0);
								bullets[b].y = -2; //Indicatore di terminazione forzata
								bullets[b].signal = -1; //Slot liberato per creazione di atri proiettili
								//Il nemico perde una vita -------------------------------------------------------------------------------------

								/*il valore di log_map associato all'i-esimo tronco, ha valore maggiore di 0 se vi è presente un nemico, in cui:
									* valore 2 - il nemico ha due vite (solo per difficoltà HARD e superiore)
									* valore 1 - il nemico ha una vita rimanente
									* valore 0 - nessun nemico sul tronco
								*/
							    log_map[i] --; //Il numero di vite del nemico viene decrementato
								//Se il nemico ha perso tutte le vite dopo esser stato colpito, viene decrementato il numero di nemici
								if(log_map[i]==0){
									start_enemy_clock = time(NULL);
									n_enemies--;
									score = score +15 +diff-1;
								}
								else score+=5 + diff-1;
								kill(pidbullets[b], 1);
							}
						}

					}
					//-----------------------------------------------------------------
				}
			}// TERMINE Ciclo sui tronchi -----------------------------------------------------------------------------------------------

			wattroff(gamewin, COLOR_PAIR(2));
			wattron(gamewin, COLOR_PAIR(1));
			
			//Stampa rana e gestione coordinate in caso in cui la rana si trovi su un tronco -------------------------
			if(previous_logx != logs[idlog].x)
			{
				/*'frog_on_log' è una variabile che assume:
					-valore 0 se la rana non è sopra un tronco
					-valore 1 se la rana è sopra il tronco e il tronco si sta muovendo da sinistra verso destra
					-valore -1 se la rana è sopra il tronco e il tronco si sta muovendo da destra verso sinistra

					*Se frog_on_log ha valore diverso da 0, questo viene sommato alle coordinate della rana globali e locali;
					* ma solo se la coordinata del tronco in x salvata su 'previous_logx' al momento in cui la rana è salita sul tronco è
					* diversa da quella appena letta dalle coordinate condivise, per sincronizzarne il movimento

        		*idlog indica l'identificatore del tronco su cui la rana è salita
    			*/
				frog.x += frog_on_log;
				//Se c'è stato un movimento del tronco previous_logx viene aggiornato con quella coordinata 
				previous_logx = logs[idlog].x;
			}
			mvwprintw(gamewin, frog.y, frog.x, "\U00000398-\U00000398");

			wattroff(gamewin, COLOR_PAIR(1));
			//Termine stampa rana --------------------------------------------------------------------------------------------
			
			// Gestione sparo rana ----------------------------------------------------------------------------------------
			if(r>0 && frog.c == 'b' && nbullets<MAX_NBULLETS) //Si spara se si è ricevuto il comando sparo da pipe (se vi è stata la lettura - r>0)
			{	
				Mix_PlayChannel(-1,frogbullet,0);
				for(int i=0; i<MAX_NBULLETS; i++)
				{
					if(bullets[i].signal == -1) //La rana cicla finchè non trova una casella libera
					{
						//Creazione processo proiettile
						pidbullets[i] = fork();
						if(pidbullets[i] == 0)
						{
							fun_bullet(pipeobj,frog.x+1,frog.y,i,'b');
							_exit(0);
						}
						break;
					}						
				}		
			}
			//Termine gestione sparo rana --------------------------------------------------------------------------------------------------------
			
			//Generazione nemici ------------------------------------------------------------------------------------------------------------------
			if(checkpoint) // I nemici vengono generati solamente una volta superata l'autostrada
			{
				int counter = 0;
				int rand_id=0; //I nemici vengono generati randomicamente tra le posizioni libere del tronco

				//Calcolo del timer
				current_enemy_clock = time(NULL);
				enemy_timer = current_enemy_clock - start_enemy_clock;
				
				/*Un nemico può inoltre essere generato solamente vengono rispettati i seguenti criteri:
                * sono passati 'enemy_spawn_timelaps' dall'ultima generazione (la prima avviene di default dopo un secondo)
                * il numero di nemici corrente è minore del massimo numero di nemici generabili contemporaneamente ('maxnum_enemies', che dipende dalla diff.)
                * se il numero corrente di nemici aggiunta la rana, se si trova su un tronco, è minore del numero di tronchi (almeno un tronco libero)
            	*/
				if(enemy_timer == enemy_spawn_timelaps && (n_enemies < maxnum_enemies) && (n_enemies + abs(frog_on_log)) < nlogs)
				{
						srand(time(NULL)+(counter++));
						rand_id = rand()%nlogs;

					//Il nemico viene generato su quel determinato tronco solo se questo è libero
					if(log_map[rand_id] == 0){
						//Mix_PlayChannel(-1,enemyspawn,0);
						n_enemies ++;
						//Il valore della log_map associata a quel determinato tronco viene aggiornata assegnandole il numero corrente di hp del nemico
						log_map[rand_id] = enemy_hps;
						start_enemy_clock = time(NULL);
						//L'intervallo di tempo tra uno spawn e l'altro varia in base alla difficoltà
						enemy_spawn_timelaps = 10 - diff*2;
						start_timer_bullets[rand_id] = time(NULL);
					}	
				}
				if(enemy_timer == enemy_spawn_timelaps)
					start_enemy_clock = time(NULL);
			}
			// Termine generazione nemici -----------------------------------------------------------------------------------------					
				
			if(itype == WIN)
				break;

			//Gestione Collisione Rana-Tane ------------------------------------------------------------------
			if(frog.y <= DENBORDER) //Il controllo sulle collisioni viene effettuato solo se la rana si trova sopra lo spazio delle tane
			{
				int goal=0;
				for(int i=0; i < N_DENS; i++)
					if(frog.x >= den[i].x && frog.x < den[i].x+DEN_LENGTH-2 && den[i].signal != 1)
					{
						//La rana ha conquistato una tana libera: viene aggiornato il numero di tane prese
						den[i].signal = 1;
						goal = 1;
						denstaken++;
						//Il punteggio ottenuto al raggiungimento dell'obiettivo è proporzionale alla difficoltà e al tempo impiegato
						score = score + 100+(timer*2.5) + 30*diff;
						start_game_clock = time(NULL);
						//Se ha conquisato tutte le tane viene comunicato il segnale di vittoria
						if(denstaken == N_DENS)
							itype = WIN;
					}	
				if(!goal){
					//La rana ha tentato di conquistare una tana già presa in precedenza o ha fallito nel centrarla
					hp --;
					start_game_clock = time(NULL);
				}
				
				frog.x = maxx/2;
				frog.y = maxy-2;
				checkpoint = 0;
			}
			//Termine gestione collisione rana-tane ------------------------------------------------------------


			//Gestione collisione veicoli ----------------------------------------------------------------------
			for(int i=0; i<(nlanes*nVehicles); i++)
			{
				//Gestione collisioni rana - veicoli ---------------------------------------------------------
				if(frog.y == vehicles[i].y)
				{
					if( (frog.x >= vehicles[i].x && frog.x <= vehicles[i].x+vehicles[i].type) ||
					     (frog.x+2 >= vehicles[i].x && frog.x <= vehicles[i].x+vehicles[i].type)) //Collisione rilevata
					 {
						//la rana perde una vita: riparte una nuova manche
						hp += next_manche(&frog, &checkpoint);
						start_game_clock = time(NULL);
						score = score-10; //Venire investiti comporta un malus sul punteggio
						if(score < 0)
							score = 0;
						Mix_PlayChannel(-1,frogcarhit,0);
					}
				}
				//Termine gestione collisioni rana - veicolo 
				//-----------------------------------------------------------------------------------------
				//Gestione collisione proiettile - veicolo ---------------------------------------------

				for(int b = 0; b<MAX_NBULLETS; b++) //Ciclo sui proiettili
				{
				
					if(bullets[b].signal != -1) {
						if((bullets[b].y == vehicles[i].y) && 
							(bullets[b].x >= vehicles[i].x && bullets[b].x <= vehicles[i].x+vehicles[i].type))
						{
							//Collisione rilevata
							bullets[b].y = -2; //Indicatore di terminazione forzata
							bullets[b].signal = -1; //Slot liberato per creazione di atri proiettili
							kill(pidbullets[b], 1);		
						}
						//Collisione Proiettili - proiettili ---------------------------------------------------------
						
							for(int B = b+1; B<MAX_NBULLETS; B++)
							{
								if(bullets[B].signal!=-1 && bullets[B].x == bullets[b].x && bullets[B].y == bullets[b].y){
									//Collisione rilevata
									score = score +10;
									bullets[b].y = -2; //Indicatore di terminazione forzata
									bullets[b].signal = -1; //Slot liberato per creazione di atri proiettili
									//Il processo viene terminato
									kill(pidbullets[b], 1);	
									bullets[B].y = -2; //Indicatore di terminazione forzata
									bullets[B].signal = -1; //Slot liberato per creazione di atri proiettili
									//Il processo viene terminato
									kill(pidbullets[B], 1);	
								}		
							}
					}
					// Termine gestione collisione tra proiettili ------------------------------------------------		     
				}	
				
					
			} // Fine gestione collisione dei veicoli -------------------------------
			
			//Stampa proiettile e Collisione rana-proiettili -------------------------------------------------------------------
			for(int i=0;i<MAX_NBULLETS; i++){
				
				if(bullets[i].signal !=-1 && bullets[i].y == frog.y && (bullets[i].x >= frog.x && bullets[i].x <= frog.x+2)&& bullets[i].c == 'B')
				{
					//La rana perde una vita
					hp += next_manche(&frog, &checkpoint);
					start_game_clock = time(NULL);
					bullets[i].y = -2; //Indicatore di terminazione forzata
					bullets[i].signal = -1; //Slot liberato per creazione di atri proiettili
					//Il processo viene terminato
					kill(pidbullets[i], 1);	
				}
				//Stampa del proiettile .......................................................................
				if(bullets[i].signal !=-1)
					mvwprintw(gamewin, bullets[i].y,bullets[i].x,(bullets[i].c == 'b')?"\U00002022":"\U0000226C");
				if(bullets[i].y == -1)
					bullets[i].signal = -1;	
				//..............................................................................................	
			}
			//Termine stampa proiettili e gestione collisione rana-proiettili --------------------------------------------------------------

			//Gestione collisione rana - tronchi -----------------------------------------------------------------------------------------
			for(int i=0; i<nlogs; i++)
			{
				if(frog.y == logs[i].y+1) //Ciclo sui tronchi ----------------------------
				{
					if(frog.x >= logs[i].x && frog.x <= logs[i].x+(LOGLEN-1))
					{	
						//Gestione collisione rana - nemico
						if(log_map[i]>0)
						{
							hp += next_manche(&frog, &checkpoint);
							start_game_clock = time(NULL);
							checkpoint = 0;
							start_enemy_clock = time(NULL);	
							
						}//Fine gestione collisione rana - nemico
						else
						{		//La rana si trova sopra un tronco				
							frog_on_log = logs[i].signal;
							idlog = i;
							previous_logx = logs[i].x;
							log_map[i] = -1;
							break;
						}
					}
					else
					{
						//Rana perde una vita
						Mix_PlayChannel(-1,frogriverhit,0);
						hp += next_manche(&frog, &checkpoint); 
						start_game_clock = time(NULL);
						if(log_map[i]<0)					
							log_map[i] = 0;
					}
				}
				else {
					//La rana è scesa dal tronco
					if(log_map[i]<0)					
						log_map[i] = 0;
					frog_on_log = 0; 
				}
			} // Termine gestione collisione rana-tronchi ------------------------------------------

		} // Termine stampa e generazione collisioni  -----------------------------------------------------------------------------

		//--------------------------------------------------------
		if(frog.c == 'q')
		{
			itype = QUIT;
			break;	
		}
		if(hp <= 0)
		{
			itype = GAMEOVER;
			break;	
		}
			
		wrefresh(gamewin);
	}//Termine while true ---------------------------------------------
	if(score > highscore)
				write_highscore(fph, score);

	//Aggiornamento dell'highscore se necessario
	if(itype == WIN)
		return(score);
    else if (itype == QUIT)
        return -1;
	else
		return 0;
} //Termine gestione stampa ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
