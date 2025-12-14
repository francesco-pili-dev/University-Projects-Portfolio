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


int main()
{

    //Inizializzazione mixer dei suoni -----------------------------
    SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,32);
    //--------------------------------------------------------------

    //Setting UNICODE ----------------------------------------------
    setlocale(LC_ALL, "en_US.utf8");
    //--------------------------------------------------------------

    /*Variabile 'choice'. Il valore è dato in input dal giocatore grazie all'ausilio delle funzioni menu, può assumere:
        *Valore 0 se è stata iniziata una nuova partita
        *Valore < 0 se il giocatore intende non iniziare una nuova partita
        * valore 2 se il giocatore vince la partita e vuole continuare a giocare a difficoltà maggiore
    */
    int choice = 0;
    int score = 0;

    choice = menu();
    if(choice){
	    endwin();
	return 0;
    }
    screen_manager();
    
    do{    
        screen_manager();
        fp = fopen("log1.txt", "w+");
        setbuf(fp, NULL);

        frog_data.x = maxx/2-1; 
        frog_data.y = maxy-2;
        
        initscr();
        noecho();
        start_color();
        curs_set(0);
        cbreak();
        set_colors();

        gamewin = newwin(maxy, maxx, 0, 0);
        keypad(gamewin, true);

        //Creazione e inizializzazione del thread rana ---------------------------------------
        pthread_t frog_id;
        pthread_create(&frog_id, NULL, funcFrog, NULL);
        //-----------------------------------------------------------------------------------

        //Creazione e inizializzazione dei thread di tipo veicolo -----------------------------
            srand(time(NULL));
            int dir =rand()%2;           //Variabili di supporto all'inizializzazione
            int xvec = MAXVEHICLE_LEN;
            int yvec = last_lane;
            int lane = 0;
            int j=1;
            int type = rand()%3;

        //Allocazione dello spazio di memoria per mutex e strutture associate ai veicoli
        vec_mutex = malloc(sizeof(pthread_mutex_t)*(nlanes*nVehicles));
        vehicles = malloc(sizeof(vehicle)*(nlanes*nVehicles));
        //------------------------------------------------------------------------------
        pthread_t vehicles_id[nlanes*nVehicles];
        
        for(int i=0; i<nlanes*nVehicles; i++)
        {
            pthread_mutex_init(&vec_mutex[i], NULL);
            //Inizializzazione dei veicoli ---------------------------------
            vehicles[i] = init_vec(i, dir, (rand()%3)?CAR:TRUCK, lane, xvec, yvec);

            //Nuova corsia-----------------------------------------------
            if(j == nVehicles) {  
                lane++;
                yvec -= 2; //2: altezza in caratteri di una corsia
                dir = (dir)?0:1; //Si inverte la direzione dei veicoli
                j=0;
                xvec = MAXVEHICLE_LEN;
            }
            //----------------------------------------------------------
            j++;
            srand(time(NULL)+i);
            xvec+=(20+(rand()%maxx/2));     //Generazione della coordinata in x del successivo veicolo
            xvec = xvec%maxx;
            if(xvec < MAXVEHICLE_LEN) xvec = MAXVEHICLE_LEN;
        }
        //Creazione dei Thread: ricevono come parametro l'id del veicolo: in tal modo ciascun veicolo sarà in grado di conoscere 
        //la posizione della propria cella all'interno dell'array globale dei veicoli su cui operare
        int rc, t;
        for(t=0; t<(nlanes*nVehicles); t++){
            thread_data_array[t].thread_id = t;
            if(thread_data_array[t].thread_id < 8){
                rc = pthread_create(&vehicles_id[t], NULL, funcVehicle, (void *) &thread_data_array[t]);
                if(rc)
                    exit(-1);
            }
        }
        //Termine generazione Thread veicolo ---------------------------------------------------------------------------

        //Gestione e creazione Thread tronco ---------------------------------------------------------------------------

        int ylog = FIST_LOG_Y;
        int dirlog = (rand()%2)?1:-1;

        object logs[nlogs];

        //Allocazione dello spazio di memoria per mutex e strutture associate ai tronchi
        log_mutex = malloc(sizeof(pthread_mutex_t)*(nlogs));
        logs_data = malloc(sizeof(object)*(nlogs));
        //------------------------------------------------------------------------------
        pthread_t logs_id[nlogs];

        //Inizializzazione dei parametei e creazione dei threads -----------------------
        for(int i=0; i < nlogs; i++)
        {
            pthread_mutex_init(&log_mutex[i], NULL);
            logs[i].y = ylog;
            logs[i].signal = dirlog;
            logs[i].id = i;      
            //Ciascun Thread riceve come parametro un puntatore ad una struttura di tipo object contenente i dati necessari per l'inizializzazione
            rc = pthread_create(&logs_id[i], NULL, funcLog, (void *) &logs[i]);
            if(rc)
                exit(-1);
            //Successivo flusso: ciascun flusso ha la direzione opposta al precedente, se esiste
            ylog += 2;
            dirlog = (dirlog==-1)?1:-1;
        }
        //-----------------------------------------------------------------------------

        //Termine generazione Thread tronco ----------------------------------------------------------------------------

        //Richiamo del gestore della grafica e delle collisioni --------------------------------------------------------
        score = gameArea(score);
        //--------------------------------------------------------------------------------------------------------------
        /*gameArea restituisce il punteggio ottenuto dal giocatore in caso di vittoria. 
          Il punteggio viene eventualmente utilizzato (se il giocatore intende proseguire il gioco a difficoltà maggiorata)
          per passarlo alla successiva chiamata di gameArea e proseguire partendo dal punteggio precedente.
          In caso di sconfitta o quit restituisce 0.
          Il punteggio ottenuto dal giocatore viene in ogni caso scritto su un file se superiore al massimo punteggio ottenuto.
        */
        if(score>0){
            if(diff!=GOF)
                diff++;
                choice = intr_menu(WIN);
                //Il giocatore, grazie alla chiamata a 'intr_menu' può scegliere se continuare o terminare il gioco
            }
            else if(score == 0)
                choice = intr_menu(GAMEOVER);
                //Il giocatore, grazie alla chiamata a 'intr_menu' può scegliere se riprovare o terminare il gioco
            else choice = intr_menu(QUIT);

        //pthread_cleanup_push(3);

        //Terminazione dei Thread: (non viene utilizzata per questo scopo una pthread_join poichè tutti i thread in questione non cessano mai la propria esecuzione)
        pthread_cancel(frog_id);

        for(int i=0; i<nVehicles*nlanes; i++)
            pthread_cancel(vehicles_id[i]);

        for(int i=0; i<nlogs; i++)
            pthread_cancel(logs_id[i]);
        //-----------------------------------------------

        //Viene liberata la memoria allocata per mutex e strutture ----
        free(vehicles);
        free(vec_mutex);
        free(logs_data);
        free(log_mutex);
        //-------------------------------------------------------------

        delwin(gamewin);
        endwin();

    }while(choice == 0 || choice == 2);

    return 0;
}