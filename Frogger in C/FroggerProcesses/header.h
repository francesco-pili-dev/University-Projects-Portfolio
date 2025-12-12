#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>

/* FROGGER 2.0 - Versione processi
* GRUPPO PROGETTO: FLORIS NICOLA (IADA/00035) | PILI FRANCESCO (IADA/00067)
*/

//definizione di costanti simboliche --------------------------------
#define MAXVEHICLE_LEN 8
#define LOGLEN 9
#define CHECKPOINT 19
#define MAXNUM_ENEMIES 4
#define MAX_NBULLETS 15
#define N_DENS 5
#define DENBORDER 6
#define BORDER_LENGTH 9
#define DEN_LENGTH 7
#define FIST_LOG_Y 7
#define COLOR_BROWN 9
#define COLOR_DARK 12
#define SIDEWALK_LENGTH 5

//COLOR_PAIR codes per i veicoli
#define VEC1 16
#define VEC2 17
#define VEC3 18
#define VEC4 19
#define VEC5 20
#define VEC6 21
//Termine definizione costanti simboliche ------------------------

//Dichiarazioni di variabili globali -------------------------------
extern int maxx, maxy;
extern int last_log_y, first_lane, last_lane;
extern int nlanes;
extern int nVehicles;
extern int nlogs;
extern int maxnum_enemies;

//Materiale di debugging
extern time_t t;
extern FILE *fp;
extern FILE *fph;
//-------------------
extern WINDOW *gamewin; //Schermata principale di gioco
//Termine dichiarazione variabili globali --------------------------

//Area dichiarazione delle strutture di dati -------------------------------------------------------------------------------------------------

typedef enum { CAR=6, TRUCK=8 } v_type; //Tipo di veicolo (automobile/autocarro)
typedef enum { WIN, GAMEOVER, QUIT } interrup_type; //Tipo di interruzione della normale esecuzione dell'applicazione

//diffcoltà del gioco: si può scegliere all'avvio (default: NORMAL); incrementa automaticamente alla vittoria
typedef enum { EASY, NORMAL, HARD, GOF } difficulty;
extern difficulty diff;

//Struttura dati per i veicoli
typedef struct{	
	int direction; //Senso di marcia
	v_type type;  //Oltre ad indicare il tipo di veicolo ne definisce anche la lunghezza in caratteri
	int nChar_toPrint;  //Individua il numero di caratteri da stampare ad ogni instante (Stampa graduale dei veicoli al raggiungimento del bordo dello schermo)
	int char_toStart;   //Individua la posizione del carattere da cui fr partire la stampa (Stampa graduale dei veicoli al raggiungimento del bordo dello schermo)
	int lane; //Individua il numero identificante della corsia
	int y; //Coordinate x ed y di stampa
	int x;
	int id; //Identificatore di veicolo 
	int color;
}vehicle;

//Struttura generale per gli oggetti(rana, tronchi, proiettili)
typedef struct {
	int x;
	int y;
	int signal; //variabile utilizzata per registrare alcuni segnali di sincronizzazione e/o stampa 
	char c;     //Specifica il comando, utilizzata anche per distinguere i vari oggetti
}object;


//Termine dichiarazione strutture dati ---------------------------------------------------------------------------------

//Area prototipi delle funzioni-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Menu principale di gioco
int menu();  
//Il compito principale della seguente funzione è quello di assegnare i valori alle variabili globali in base alla difficoltà. Pone i limiti di schermo
void screen_manager();

//Motore grafico: riceve le coordinate. I compiti principali sono la stampa degli oggetti e la gestione delle collisioni tra gli stessi
int gameArea(int *pipevec, int *pipefrog, int *pipeobj, int score);
//Restituisce il valore del più alto punteggio raggiunto letto all'interno di un file 
int read_highscore(FILE *fph);
//Aggiorna se necessario il valore dell'highscore
void write_highscore(FILE *fph, int highscore);
//Sottofunzione di gameArea: stampa a video in una finestra sottostante quella di gioco il punteggio attuale e l'highscore
void print_score(WINDOW *win_score, int score, int highscore);
//Sottofunzione di gameArea: supporto alla stampa dei veicoli
void printVehicle(vehicle v, char* s); 
//Sottofunzione di gameArea: inizializza gli oggetti di tipo 'tana'
void fun_den(object den[]);
//Sottofunzione di gameArea: gestisce il caso in cui si perda una vita
int next_manche(object *frog, int *checkpoint);
//Sottofunzione di gameArea: gestisce la stampa a video degli oggetti statici, del tempo e delle vite rimaste
void print_gamefield(int hp, int time_limit, int timer, time_t *start_game_clock, object *frog, object *den);
//Inizializza le strutture locali di gameArea
void init_structures(object *logs, vehicle *vehicles);

//Inizializzazione dei valori per ogni oggetto di tipo 'vehicle'
vehicle init_vec(int id, int direction, v_type type, int lane, int x, int y);
//Funzione il cui compito consiste nel generare le coordinate dei veicoli, gestendo i casi di raggiungimento dei limiti dello schermo.
void fun_vehicle(vehicle v , int *pipevec);
//Genera i movimenti dell'oggetto rana a seconda delle decisioni dell'utente. Gestisce il comando 'sparo' e il comando 'quit'
void fun_frog(int *pipefrog);
//Genera le coordinate dell'oggeto 'tronco'
void fun_log(int *pipelog, int y, int dir);
//Genera le coordinate dell'oggetto 'proiettile'. Separa i casi in cui lo sparo è causato dall'utente o dai nemici. Termina una volta raggiunti i limiti dello schermo
void fun_bullet(int *pipebullet, int startx, int starty, int id, char shooter);

//Viene richiamata ogni qual volta il gioco termina, permette all'utente di decidere se continuare/riprovare il gioco oppure terminarne l'esecuzione
int intr_menu(interrup_type type);
//Contiene le definizioni dei colori
void set_colors();
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
