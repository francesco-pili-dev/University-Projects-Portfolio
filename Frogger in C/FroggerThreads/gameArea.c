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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

    Mix_Music *gamemusic[4];
	Mix_Chunk *enemyspawn;
	Mix_Chunk *frogbullet;
	Mix_Chunk *enemybullet;
	Mix_Chunk *cars;
    Mix_Chunk *frogcarhit;
	Mix_Chunk *frogenemyhit;

int gameArea(int score)
{
    //Caricamento musiche di gioco ------------------------------
	gamemusic[0] = Mix_LoadMUS("sounds/froggertheme.mp3");
	gamemusic[1] = Mix_LoadMUS("sounds/froggertheme.mp3");
	gamemusic[2] = Mix_LoadMUS("sounds/gamemusic.mp3");
	gamemusic[3] = Mix_LoadMUS("sounds/darksoultheme.mp3");
	enemyspawn = Mix_LoadWAV("sounds/enemyspawn.mp3");
	frogbullet = Mix_LoadWAV("sounds/frogbullet.mp3");
	enemybullet = Mix_LoadWAV("sounds/enemybullet.mp3");
	frogcarhit = Mix_LoadWAV("sounds/frogcarhit.mp3");
	frogenemyhit = Mix_LoadWAV("sounds/frogenemyhit.mp3");
	Mix_PlayMusic(gamemusic[diff],-1);
	Mix_VolumeMusic(15);
    //-------------------------------------------------------------------

    //Dichiarazione e inizializzazione della struttura locale rana
    object frog;
    frog.x = maxx/2-1;
    frog.y = maxy-2;
    frog.c = 'f';
    //----------------------------------------------------------

    //Dichiarazione e inizializzazione delle strutture locali dei veicoli
    vehicle vecs[nlanes*nVehicles];
    for(int i=0; i<(nlanes*nVehicles); i++)
    {
        vecs[i].x = -1;
        vecs[i].y = -1;
        vecs[i].direction = -1;
        vecs[i].nChar_toPrint = 0;
        vecs[i].char_toStart = 0;
    }
    //Utilizzato per evitare errori di segmentazione
    int vehicles_read = 0;

    char sprites[4][MAXVEHICLE_LEN+1];
	strcpy(sprites[0], "____))  ");  
	strcpy(sprites[1], "  ((____");
	strcpy(sprites[2], "______|]");
	strcpy(sprites[3],  "[|______");
    //-----------------------------------------------

    //Dichiarazione e inizializzazione delle struttura locale tronco
    object logs[nlogs];
    //Array di interi log_map: mappa la presenza di nemici sul tronco
    int log_map[nlogs];               //Se log_map[i] ha valore 0 sul tronco non vi è presente nessuno
	for(int i=0; i<nlogs; i++)        //Se log_map[i] ha valore -1 sul tronco vi è sopra la rana
		log_map[i] = 0;               //Se log_map[i] ha valore > 0 sul tronco vi è presente un nemico
    //---------------------------------------------------------

    //Dichiarazione e inizializzazione strutture di dati dei proiettili
	int nbullets = 0; //Registra il numero di proiettili presenti contemporaneamente sullo schermo
	object bullets[MAX_NBULLETS]; //La costante simbolica MAX_NBULLETS indica il massimo numero di proiettili presenti contemporaneamente possibili 
	for(int i=0; i<MAX_NBULLETS; i++){
        //la variabile signal in questo caso inizializzata a -1 indica che in quella specifica casella non è al momento occupata da altri proiettili 
		bullets[i].signal = -1;
        bullets[i].id = -1;
	}
    //Dichiarazione dei thread id per i proiettili
    pthread_t bullets_id[MAX_NBULLETS];

    bull_mutex = malloc(sizeof(pthread_mutex_t)*MAX_NBULLETS);
    bullets_data = malloc(sizeof(object)*MAX_NBULLETS);
    for(int i=0; i<MAX_NBULLETS; i++){
		bullets_data[i].signal = -1;
        bullets_data[i].id = -1;
	}
	//---------------------------------------------------------------


    //Creazione finestra di gioco secondaria il cui scopo sarà quello di stampare a video il punteggio a tempo reale del giocatore 
    //e un highscore, letto da un file che lo contiene
    WINDOW *win_score = newwin(4, maxx, maxy, 0);
	init_pair(11, COLOR_WHITE, COLOR_DARK);
    //Setting del background-color
	wbkgd(win_score, COLOR_PAIR(12));

    //Lettura dell'highscore da file
    int highscore = read_highscore(fph);
    //----------------------------------------------

    //SPAZIO DELLE VARIABILI LOCALI -------------------------------------------------------------------------------------------------------------------

    //Variabile checkpoint: viene incrementata a 1 quando la rana supera l'autostrada. Una volta superato il checkpoint i nemici sono abilitati ad apparire
    int checkpoint = 0;
    //Vite iniziali della rana
    int hp=3;

    //Timer di gioco, utiizzato per la gestione del cronometro
    time_t start_game_clock = time(NULL);
	time_t current_game_clock;
    int timer = 0;
    //Viene dato un limite di tempo di 40 secondi al giocatore per ogni manche
    int time_limit = 40;

    //Timer dei nemici: utilizzato per controllare il tempo tra un'apparizione di un nemico e la successiva (in caso di più nemici)
	time_t start_enemy_clock;
	time_t current_enemy_clock;
	int enemy_timer=0;
    //In base alla difficoltà il tempo che intercorre tra una generazione e l'altra è minore
	int enemy_spawn_timelaps = 10 - (diff*1.5);
    int n_enemies = 0;
    int enemy_hps = 1;
    if(diff >= HARD)
        enemy_hps = 2;
	
	//Timer dei proiettili nemici: vi sono n timer per n nemici. Utilizzato per calcolare il lasso di tempo tra uno sparo nemico e il successivo
	time_t start_timer_bullets[nlogs];
	time_t current_timer_bullets[nlogs];

    time_t iframes = time(NULL);
    int invicibility = 0;

    //Array di tane
    object dens[N_DENS];
    init_den(dens);
    int denstaken = 0;

    //Variabili utilizzate per far in modo che la rana venga trasortata dal tronco
    int frog_on_log = 0; //Determina se la rana è sopra un tronco
    int previous_logx=-1; //Registra la coordinata x del tronco nel momento in cui la rana ci sale sopra
    int idlog=-1; //Registra l'identificatore del tronco su cui la rana sta sopra
    //---------------------------------------------------------------------------
    //Gestisce le interruzioni (vittoria, sconfitta, quit)
    interrup_type itype = -1;

    //---------------------------------------------------------------------------------------------------------------------------------------------------

    //Inizio del ciclo di stampa e controllo ----------------------------------------------------------
    while(true)
    {
        usleep(50000);

        //Aggiornamento cronometro tempo di gioco
        current_game_clock = time(NULL);
        timer = current_game_clock - start_game_clock;

        //Lettura coordinate rana da variabile condivisa--------------------
        pthread_mutex_lock(&frog_mutex);
            frog = frog_data;
        pthread_mutex_unlock(&frog_mutex);
        //---------------------------------------------------------

        //Lettura dei veicoli da variabili condivise -------------------------------
            for(int i=0; i<(nlanes*nVehicles); i++)
            {
                pthread_mutex_lock(&vec_mutex[i]);
                    vecs[i] = vehicles[i];
                    if(vehicles_read<=(nlanes*nVehicles))
                        vehicles_read ++;
                pthread_mutex_unlock(&vec_mutex[i]);
            }
        //Termine lettura veicoli----------------------------------------------
        //Lettura dei tronchi da varibili condivise -------------------------
        for(int i=0; i<nlogs; i++)
        {
            pthread_mutex_lock(&log_mutex[i]);
                logs[i] = logs_data[i];
            pthread_mutex_unlock(&log_mutex[i]);
        }
        //Termine lettura dei tronchi --------------------------------------

        //Lettura coordinate dei proiettili da varibili condivise -------------------------
        for(int i=0; i<MAX_NBULLETS; i++)
        {
            if(i!=5){
            pthread_mutex_lock(&bull_mutex[i]);
                int signal = bullets_data[i].signal;
                //Per evitare di sovrascrivere tutte le posizioni anche se non necessario, si effettua prima un controllo per determinare se 
                //tale posizione contiene un proiettile
                if(signal != -1)
                    bullets[i] = bullets_data[i];
            pthread_mutex_unlock(&bull_mutex[i]);
            }
        }
        //Termine lettura coordinate proiettili -------------------------------------------

        //Si effettua un controllo sul checkpoint
        if(!checkpoint && frog.y < CHECKPOINT){
			checkpoint = 1;
            //I punteggi ottenuti sono più alti all'aumentare della difficoltà
			score = score+30+3*diff;
			start_enemy_clock = time(NULL); 
			//current_enemy_clock = time(NULL); //Parte il cronometro
			enemy_spawn_timelaps = 1; //La prima volta un nemico appare di default dopo un secondo
		}

        // Generazione dei nemici ---------------------------------------------------------
        if(checkpoint) // I nemici vengono generati solamente una volta superata l'autostrada
        {
            int counter = 0;
            int rand_id=0; 

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
                //Il nemico viene generato su un tronco casuale, specificato da 'rand_id'
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
        // Termine generazione nemici -------------------------------------------------------------------

        //Gestione comando 'sparo' della rana: creazione del thread proiettile -------------------------------------------------------
        if(frog.c == 'b')
        {	
            Mix_PlayChannel(-1,frogbullet,0);
            for(int i=0; i<MAX_NBULLETS; i++)
            {
                if(bullets[i].signal == -1) //Casella libera 
                {
                    bullets[i].x = frog.x +1;
                    bullets[i].y = frog.y;
                    bullets[i].id = i;
                    bullets[i].signal = 1;
                    bullets[i].c = 'b';
                    
                    pthread_create(&bullets_id[i], NULL, funcBullet, (void*) &bullets[i]);
                    pthread_mutex_lock(&frog_mutex);
                    frog_data.c = 'f';
                    pthread_mutex_unlock(&frog_mutex);
                    frog.c = 'f';
                    break;
                }						
            }		
        }
        //------------------------------------------------------------------------------------------
        //Generazione proiettili nemici -------------------------------------------------------------
        int bul_index;
        for(int i=0; i < nlogs; i++){
            if(log_map[i]>0){
                current_timer_bullets[i] = time(NULL);
                if(current_timer_bullets[i] - start_timer_bullets[i] > 3)
                {
                    
                    start_timer_bullets[i] = time(NULL);
                    bul_index = i+nlogs-1;
                    if(bul_index ==5)
                        bul_index =2;
                    if(bullets[bul_index].signal == -1 && checkpoint)
                    {
                        Mix_PlayChannel(-1,enemybullet,0);
                        bullets[bul_index].x = logs[i].x+((LOGLEN/2)-1);
                        bullets[bul_index].y = logs[i].y+1;
                        bullets[bul_index].id = bul_index;
                        bullets[bul_index].signal = 1;
                        bullets[bul_index].c = 'B';
                    
                        pthread_create(&bullets_id[bul_index], NULL, funcBullet, (void*) &bullets[bul_index]);
                        break;
                    }			
                }
            }
        }
        //----------------------------------------------------------------------------------------------
        //GESTIONE STAMPA: viene sincronizzata dal mutex dedicato alle funzioni di ncurses  --------------------------------------
        if(vehicles_read > (nlanes*nVehicles)){
            pthread_mutex_lock(&mutexcurses);
                wclear(gamewin);

                print_gamefield();
                print_score(win_score, score, highscore);
                print_hps(hp);
                print_time(time_limit, timer); 
                print_dens(dens);

                 //Stampa dei proiettili ................................................................
                for(int i=0; i<MAX_NBULLETS; i++)
                {
                    if(bullets[i].signal !=-1)
                        mvwprintw(gamewin, bullets[i].y,bullets[i].x,(bullets[i].c == 'b')?"\U00002022":"\U0000226C");
                    if(bullets[i].y == -1)
                        bullets[i].signal = -1;
                }
                //Termine stampa proiettili ..........................................................

                //Stampa dei veicoli -------------------------------
                print_vehicles(vecs, sprites);
                //Termine stampa dei veicoli -------------------------

                //Stampa tronchi ---------------------------------------------------------------------------

                for(int i=0; i < nlogs; i++)
                {
                    wattron(gamewin, COLOR_PAIR(9));
                    mvwprintw(gamewin, logs[i].y+1, logs[i].x, (logs[i].signal!=1)?"        @":"@        ");	
                    wattroff(gamewin, COLOR_PAIR(9));	
                    wattron(gamewin, COLOR_PAIR(2));
                    //Stampa dei nemici sui tronchi 

                    if(log_map[i] > 0)
				    {
                        wattron(gamewin, COLOR_PAIR(3+log_map[i]-1));
                        if(current_timer_bullets[i] - start_timer_bullets[i] == 3 && checkpoint)
                            mvwprintw(gamewin, logs[i].y+1, logs[i].x+(LOGLEN/2)-1, "7o7");
                        else mvwprintw(gamewin, logs[i].y+1, logs[i].x+(LOGLEN/2)-1, "7_7");
                        wattroff(gamewin, COLOR_PAIR(3+log_map[i]-1));
                        wattron(gamewin, COLOR_PAIR(2));
                    }
                }
                //Termine stampa tronchi -------------------------------------------------------------------    
                
                //Stampa della rana ------------------------------
                print_frog(&frog, frog_on_log, &previous_logx, logs, idlog);
                //Termime stampa rana ---------------------------

                wrefresh(gamewin);
            pthread_mutex_unlock(&mutexcurses);
        }
        // TERMINE GESTIONE STAMPA-------------------------------------------------------

        //GESTIONE COLLISIONI -------------------------------------------------------------
        //current_game_clock = time(NULL);
        if(current_game_clock-iframes > 0)
            invicibility = 0;

        if(!invicibility){
        //Controllo collisioni con i veicoli: veicolo-rana; veicolo-proiettile
            if(vecsCollisions(vecs,frog, bullets, bullets_id)==1){
                //Se la funzione restituisce 1 allora è stata rilevata una collisione rana-veicolo: la rana perde la manche
                hp += next_manche(&frog, &checkpoint); //Gli hp sono decrementati
                //Il timer di gioco è resettato
                start_game_clock = time(NULL);
                //Unico malus sul punteggio del gioco
                score = score-10;
                if(score < 0)
                    score = 0;
                Mix_PlayChannel(-1,frogcarhit,0);
                beep();
                invicibility = 1;
                iframes = time(NULL);
            }

        }
        //Termine controllo collisioni veicoli ------------------------------------------

        //Gestione collisioni rana-fiume -------------------------------------------------

        //FrogRiverCollision
        int frc = river_collisions(frog, logs, log_map, &frog_on_log, &previous_logx, &idlog);
        if(frc > 0)
        {
            //la rana ha perso una vita
            hp += next_manche(&frog, &checkpoint);
            start_game_clock = time(NULL);
            if(frc == 2)
                start_enemy_clock = time(NULL);
                //La rana ha perso una vita a causa del nemico: il timer viene azzerato
        }
        //-------------------------------------------------------------------------------

        //Gestione collisioni degli oggetti proiettile -------------------------------
            int bc = bullets_collisions(bullets, bullets_id, frog, log_map, &score, &n_enemies, &start_enemy_clock, logs);
            if(bc > 0)
            {
                //la rana ha perso una vita
                hp += next_manche(&frog, &checkpoint);
                start_game_clock = time(NULL);
            }
        //-------------------------------------------------------------------------

        //Gestione Collisione Rana-Tane ------------------------------------------------------------------
			
			if(itype == WIN)
				break;

			if(frog.y <= DENBORDER) //Il controllo sulle collisioni viene effettuato solo se la rana si trova sopra lo spazio delle tane
			{
				int goal=0;
				for(int i=0; i < N_DENS; i++)
					if(frog.x >= dens[i].x && frog.x < dens[i].x+DEN_LENGTH-2 && dens[i].signal != 1)
					{ 
                        //La rana ha conquistato una tana libera: viene aggiornato il numero di tane prese 
						dens[i].signal = 1;
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
				
				next_manche(&frog, &checkpoint);
			}
			//Termine gestione collisione rana-tane ------------------------------------------------------------


        //TERMINE GESTIONE COLLISIONI -----------------------------------------------

        //gestione cronometro --------------------------------
        if(time_limit - timer == 0)
		{
            //Se il tempo è scaduto la rana perde una vita; viene iniziata una nuova manche
            hp += next_manche(&frog, &checkpoint);
            start_game_clock = time(NULL);
        }
        //----------------------------------------------------
        
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

    }// TERMINE WHILE TRUE ---------------------------------------------------------------------
    
    //Si eliminano eventuali proiettili rimasti pendendi ---------------------------------------
    for(int i=0; i<MAX_NBULLETS; i++){
        if(bullets[i].signal!=-1){
            destroy_bullet(i,&bullets[i],&bullets_id[i]);
        }   
    }
    free(bull_mutex);
    free(bullets_data);
    //------------------------------------------------------------------------------------------
   
    //Aggiornamento dell'highscore se necessario
    if(score > highscore)
		write_highscore(fph, score);

    if(itype == WIN)
		return(score);
    else if (itype == QUIT)
        return -1;
	else
		return 0;
    //-------------------------------------------

} //Termine funzione GAMEAREA --------------------------------------------------------------------------------------------------------------------------------