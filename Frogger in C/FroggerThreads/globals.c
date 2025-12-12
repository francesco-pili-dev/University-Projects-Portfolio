#include "fheader.h"

//Spazio di valorizzazione delle variabili globali ---------------------------------------------
int maxx=79, maxy = 36;
int nlanes = 4;
int nVehicles = 2;
int nlogs = 4;
int maxnum_enemies = 0;
int last_log_y=0, first_lane=0, last_lane=0;

vehicle *vehicles = NULL;
object *logs_data = NULL;
object *bullets_data = NULL;
object frog_data;

pthread_mutex_t frog_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexcurses = PTHREAD_MUTEX_INITIALIZER;
struct thread_data thread_data_array[NUM_THREADS];
pthread_mutex_t *vec_mutex;
pthread_mutex_t *log_mutex;
pthread_mutex_t *bull_mutex;
FILE *fp = NULL;
FILE *fph = NULL;
WINDOW *gamewin = NULL; 

//La difficoltà è di default normal
difficulty diff = NORMAL;
//--------------------------------------------------------------------------------------------------
