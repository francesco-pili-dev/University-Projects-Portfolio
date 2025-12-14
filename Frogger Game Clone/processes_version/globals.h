#include "header.h"

//Inizializzazione delle variabili globali -------------------------------------------
int maxx=79, maxy = 36;
int last_log_y=0, first_lane=0, last_lane=0;
int nlanes = 4;
int nVehicles = 2;
int nlogs = 4;
int maxnum_enemies=0;

time_t t = NULL;
FILE *fp = NULL;
FILE *fph = NULL;
WINDOW *gamewin = NULL;

difficulty diff = NORMAL;

//------------------------------------------