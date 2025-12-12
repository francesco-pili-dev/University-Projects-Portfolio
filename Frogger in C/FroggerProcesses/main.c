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

int main() //Inizio funzione main------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{
    int choice = -1;
    int score = 0;
    setlocale(LC_ALL, "en_US.utf8");
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,32);
    //Richiamo del menu principale: si seleziona a difficoltà, restituisce 0 se l'utente preme il tasto 'GIOCA'
    choice = menu();
    if(choice){ 
        endwin();
        return 0;
    }
    screen_manager();  //Inizializzazione delle variabili globali
    do {
        fp=fopen("log.txt","w+");
        setbuf(fp, NULL);
        
	    screen_manager();
	
        //Inizializzazione pipes: pipe rana(non bloccante), pipe veicolo e pipe general-------------------
        int pipevec[2];
        int pipefrog[2];
        int pipeobj[2];

        if(pipe(pipevec)==-1){
            printf("Pipe Error\n");
            _exit(1);
        }

        if(pipe(pipeobj)==-1){
            printf("Pipe Error\n");
            _exit(1);
        }
        fcntl(pipeobj[0], F_SETFL, fcntl(pipeobj[0], F_GETFL) | O_NONBLOCK);

        if(pipe(pipefrog)==-1){
            printf("Pipe Error\n");
            _exit(1);
        }
        fcntl(pipefrog[0], F_SETFL, fcntl(pipefrog[0], F_GETFL) | O_NONBLOCK);

        //Termine creazione pipes --------------------------------------------------------------------------------
        
        //Inizializzazione schermo e finestra di gioco
        initscr();
        noecho();
        start_color();
        curs_set(0);
        cbreak();
        gamewin = newwin(maxy, maxx, 0, 0);
        getmaxyx(gamewin, maxy, maxx);
        set_colors();
        //------------------------------------------------------------------------------------------------

        //Inizializzazione dei pid dei processi ---------------------------------------------------------
        pid_t pidv[nlanes * nVehicles];
        pid_t pidfrog;
        pid_t pidlogs[nlogs];
        //-----------------------------------------------------------------------------------------------------

        //Spazio delle variabili di supporto all'inizializzazione degli oggetti -----------------------------
        srand(time(NULL));
        int dir = rand()%2;
        int xvec = MAXVEHICLE_LEN;
        int yvec = last_lane;
        int lane = 0;
        int j=1;
        int type = rand()%3;
        vehicle v[nVehicles*nlanes];
        object logs[nlogs];
        int ylog = FIST_LOG_Y;
        int dirlog = (rand()%2)?1:-1;
        //-------------------------------------------------------------------------------------------------
        
        //for per la generazione dei processi di tipo veicolo----------------------------------------------------
        for(int i=0; i<(nlanes*nVehicles); i++)
        {
            pidv[i] = fork();
            if(pidv[i] < 0)
                perror("Errore fork veicoli");		

            if(pidv[i] == 0)
            {	
                //Processo figlio: veicolo
                srand(time(NULL)+i);
                v[i] = init_vec(i, dir, (rand()%3)?CAR:TRUCK, lane, xvec, yvec);
                fprintf(fp, "Corsia %d | id = %d |  x = %d | y = %d\n",v[i].lane, v[i].id, v[i].x, v[i].y);
                fun_vehicle(v[i], pipevec);
                _exit(0);
            }
            else {
                //Processo padre
                if(j == nVehicles) {  // Nuova corsia
                    lane++;
                    yvec -= 2; //2: altezza in caratteri di una corsia
                    dir = (dir)?0:1; //Si inverte la direzione dei veicoli
                    j=0;
                    xvec = MAXVEHICLE_LEN;
                }
                j++;
                srand(time(NULL)+i);
                xvec+=(20+(rand()%maxx/2));
                xvec = xvec%maxx; //Si evita che i veicoli vengano generati fuori dai limiti della finestra
                if(xvec < MAXVEHICLE_LEN) xvec = MAXVEHICLE_LEN;
            }	
        } //Termine gestione creazione processi veicolo------------------------------------------------
        
        //Inizio gestione processi tronco ------------------------------------------------------------
        for(int i=0; i<nlogs; i++)
        {
            pidlogs[i] = fork();

            if(pidlogs[i]<0)
                perror("Errore fork");

            if(pidlogs[i]==0) // Nyovo processo tronco
                fun_log(pipeobj, ylog, dirlog);
            else {
                //Nuovo flusso
                ylog += 2;
                dirlog = (dirlog==-1)?1:-1; //Si inverte la direzione
            }
        } //Termine creazione processi tronco -------------------------------------------------------

        //Generazione processo rana------------------------------------------------------------------
        pidfrog = fork();

        if(pidfrog<0)
            perror("Errore fork");

        else if(pidfrog==0)
        {
            fun_frog(pipefrog);
            _exit(0);
        }
        //Termine generazione processo rana----------------------------------------------

        //Richiamo della funzione 'area di gioco': se la sua esecuzione termina riceve come output il punteggio ottenuto
        else score = gameArea(pipevec, pipefrog, pipeobj, score);
        //Se la partita viene vinta score assume il valore del punteggio ottenuto, per poterlo incrementare eventualmente nella prossima partita
        //Se viene interrotta per GAME OVER o quit il punteggio non viene restituito, sovrascritto dal valore 0. 

        //Terminazione dei processi-------------------------------------------------------
        for(int i=0; i<(nVehicles*nlanes); i++)
            kill(pidv[i], 1);

        for(int i=0; i<(nlogs); i++)
            kill(pidlogs[i], 1);

        kill(pidfrog, 1);
        
        //-------------------------------------------------------------------------------
        if(score>0){  //Se la partita è vinta viene rigenerata una nuova partendo dal vecchio punteggio e con difficoltà incrementata
            if(diff!=GOF)
                diff++;
            choice = intr_menu(WIN); //L'utente può scegliere di non continuare
        }
        else if(score == 0)
                choice = intr_menu(GAMEOVER);
                //Il giocatore, grazie alla chiamata a 'intr_menu' può scegliere se riprovare o terminare il gioco
        else  
            choice = intr_menu(QUIT);

        //Chiusura dei descrittori di file e delle finestre
        close(pipefrog[0]);
        close(pipeobj[0]);
        close(pipevec[0]);
        close(pipefrog[1]);
        close(pipeobj[1]);
        close(pipevec[1]);	
        delwin(gamewin);
        endwin();
        //---------------------------------------------
		
	}while(choice == 0 || choice == 2);
	return 0;

}//Termine della funzione main-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
