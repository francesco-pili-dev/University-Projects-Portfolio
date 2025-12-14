#include "header.h"
//Il compito principale della seguente funzione è quello di assegnare i valori alle variabili globali in base alla difficoltà. Pone i limiti di schermo
void screen_manager()
{
    //Calcolo dell'ordinata dell'ultimo flusso del fiume in base al numero dei tronchi
	last_log_y = nlogs*2 + FIST_LOG_Y; 
    //Calcolo dell'ordinata della prima corsia dell'autostrada
	first_lane = last_log_y + SIDEWALK_LENGTH+2;
    //Calcolo dell'ordinata dell'ultima corsia dell'autostrada in base al numero di corsie
	last_lane = first_lane+(nlanes*2)-2;
    //In relazione ai calcoli precedenti si calcola la dimensione in altezza dello schermo (MAXX rimane fissa come costante simbolica).
	maxy = last_lane + SIDEWALK_LENGTH+3;

    //La difficoltà facile prevede un solo nemico alla volta: il valore viene incrementato secondo la difficoltà
	maxnum_enemies = diff+1;
    //Se la dificoltà è impostata su 'facile' le corsie da superare sono 3; viceversa 4
	if(diff==EASY)
		nlanes = 3;
	else nlanes = 4;
}
