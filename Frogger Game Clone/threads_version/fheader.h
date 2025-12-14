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

/* FROGGER 2.0 - Versione processi
* GRUPPO PROGETTO: FLORIS NICOLA (IADA/00035) | PILI FRANCESCO (IADA/00067)
*/

//definizione di costanti simboliche --------------------------------
#define MAXVEHICLE_LEN 8
#define LOGLEN 9
#define CHECKPOINT 19
#define MAXNUM_ENEMIES 4
#define MAX_NBULLETS 7
#define N_DENS 5
#define DENBORDER 6
#define BORDER_LENGTH 9
#define DEN_LENGTH 7
#define FIST_LOG_Y 7
#define COLOR_BROWN 9
#define COLOR_DARK 12
#define SIDEWALK_LENGTH 5
#define NUM_THREADS 10
#define FROGLEN 3
#define SHOOT 32
#define EXIT 113

//COLOR_PAIR codes per i veicoli
#define VEC1 16
#define VEC2 17
#define VEC3 18
#define VEC4 19
#define VEC5 20
#define VEC6 21
//-----------------------------------------------------------------------
extern Mix_Chunk *frogriverhit;
extern Mix_Chunk *enemyhit;
typedef enum { CAR=6, TRUCK=8 } v_type; //Tipo di veicolo (automobile/autocarro)
typedef enum { WIN, GAMEOVER, QUIT } interrup_type; //Tipo di interruzione della normale esecuzione dell'applicazione
//diffcoltà del gioco: si può scegliere all'avvio (default: NORMAL); incrementa automaticamente alla vittoria
typedef enum { EASY, NORMAL, HARD, GOF } difficulty;

//Spazio di dichiarazione delle strutture di dati ------------------------

//Struttura dati per i veicoli
typedef struct{	
	int direction; //Senso di marcia
	v_type type;  //Oltre ad indicare il tipo di veicolo ne definisce anche la lunghezza in caratteri
	int nChar_toPrint; //Individua il numero di caratteri da stampare ad ogni instante (Stampa graduale dei veicoli al raggiungimento del bordo dello schermo)
	int char_toStart; //Individua la posizione del carattere da cui fr partire la stampa (Stampa graduale dei veicoli al raggiungimento del bordo dello schermo)
	int lane; //Individua il numero identificante della corsia
	int y;  //Coordinate x ed y di stampa
	int x;
	int id; //Identificatore di veicolo 
	int color;
}vehicle;

//Struttura generale per gli oggetti (rana, tronchi, proiettili)
typedef struct {
	int x;
	int y;
	int signal; //variabile utilizzata per registrare alcuni segnali di sincronizzazione e/o stampa 
	int id;
	char c;
}object;

//Parametro dei thread veicolo
struct thread_data{
	int thread_id;
};
//------------------------------------------------

//Spazio delle dichiarazioni delle variabili globali ----------------------------------------------

extern int maxx, maxy;
extern int last_log_y, first_lane, last_lane;
extern int nlanes;
extern int nVehicles;
extern int nlogs;
extern int maxnum_enemies;

//Materiale di debugging
extern time_t t;
extern FILE *fp;
//---------------------

extern FILE *fph; //Puntatore al file contenente l'highscore
extern WINDOW *gamewin; //Schermata principale di gioco
extern difficulty diff;

//Strutture condivise  ----------------
extern vehicle *vehicles;
extern object frog_data;
extern object *logs_data;
extern object *bullets_data;
//------------------------------------
//Parametro dei thread veicoli
extern struct thread_data thread_data_array[NUM_THREADS];

//Mutex per la gestione delle corse critiche 
extern pthread_mutex_t frog_mutex;
extern pthread_mutex_t *vec_mutex;
extern pthread_mutex_t *log_mutex;
extern pthread_mutex_t *bull_mutex;
//Mutex utilizzato per la sincronizzazione delle funzioni di Ncurses
extern pthread_mutex_t mutexcurses;
//------------------------------------------------------------------------------

//Area prototipi delle funzioni-------------------------------------------------------------------------------------------------------------------------------------

//Menu principale di gioco
int menu();
//Il compito principale della seguente funzione è quello di assegnare i valori alle variabili globali in base alla difficoltà. Pone i limiti di schermo
void screen_manager();

//Motore grafico: riceve le coordinate. I compiti principali sono la stampa degli oggetti e la gestione delle collisioni tra gli stessi
int gameArea(int score);

//Legge da file dedicato il valore del punteggio più alto ottenuto: se il file non esiste ne viene creato uno nuovo
int read_highscore(FILE *fph);

//Aggiorna il valore dell'highscore se il punteggio ottenuto nella partita corrente è più alto
void write_highscore(FILE *fph, int highscore);

//Sottofunzione di gameArea: stampa a video in una finestra sottostante quella di gioco il punteggio attuale e l'highscore
void print_score(WINDOW *win_score, int score, int highscore);

//Sottofunzione di gameArea: stampa a video le vite rimanenti
void print_hps(int hp);

//Sottofunzione di gameArea: stampa a video il tempo di gioco rimanente
void print_time(int time_limit, int timer);

//Stampa a video delle tane: se queste sono già state 'conquistate' viene reso visibile al giocatore mostrando lo sprite della rana al suo interno
void print_dens(object *den);


//Stampa dell'oggetto rana: gesitsce inoltre la coordinate se la rana si trova sopra un tronco
int print_frog(object *frog, int frog_on_log, int *previous_logx, object logs[], int idlog);

//Cicla sui veicoli presenti: assegna lo sprite per ogni veicolo in base alla direzione e al tipo, successivamente richiama 'printVehicle'
void print_vehicles(vehicle vecs[], char sprites[][MAXVEHICLE_LEN+1]);

//Sottofunzione di gameArea: gestisce la stampa di un singolo veicolo
void printVehicle(vehicle v, char* s);

//Supporto di gameArea: stampa gli oggetti statici dello schermo di gioco
void print_gamefield();

//Si occupa di controllare la presenza di collisioni con tutti gli oggetti che possono collidere con il veicolo. In questo caso rana e proiettili
//Se rileva una collisione con un proiettile termina il thread associato e aggiorna le variabili di controllo
int vecsCollisions(vehicle *vecs, object frog, object bullets[], pthread_t bullets_id[]);

//Si occupa di controllare la presenza di collisioni con alcuni oggetti che possono collidere con un proiettile
int bullets_collisions(object bullets[], pthread_t bullets_id[], object frog, int log_map[], int *score, int *n_enemies, time_t *start_enemy_clock, object logs[]);

//Termina il thread identificato da 'bull_id', aggiorna le variabili globali e locali del proiettili ----------------------------------------
void destroy_bullet(int bull_id, object *bullet, pthread_t *bull_thread_id);

//Supporto di GameArea: si occupa di stabilire se avvengono collisioni tra la rana e gli oggetti che si trovano sul fiume: rana-tronco, rana-nemico, rana-fiume
int river_collisions(object frog, object logs[], int log_map[], int *frog_on_log, int *previous_logx, int *idlog);

//La funzione viene invocata ogni qualvolta il giocatore perde una vita: azzera il checkpoint, riposiziona la rana
int next_manche(object *frog, int *checkpoint);

//Sottofunzione di gameArea: inizializza gli oggetti di tipo 'tana' -------------------------------
void init_den(object den[]);

//Inizializzazione dei valori per ogni oggetto di tipo 'vehicle'
vehicle init_vec(int id, int direction, v_type type, int lane, int x, int y);

//Funzioni Thread (Rana, Veicolo, Tronco, Protiettile -----------------------------------
void *funcFrog(void *args);
void *funcVehicle(void *args);
void *funcLog(void *args);
void *funcBullet(void *args);
//----------------------------------------------------------------------------------

//Viene richiamata ogni qual volta il gioco termina, permette all'utente di decidere se continuare/riprovare il gioco oppure terminarne l'esecuzione
int intr_menu(interrup_type type);
//Contiene le definizioni dei colori
void set_colors();
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------