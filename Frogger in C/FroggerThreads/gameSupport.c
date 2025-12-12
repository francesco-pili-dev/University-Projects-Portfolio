#include "fheader.h"
#include<pthread.h>
/*
	*Il file 'gameSupport' include tutte le funzioni d'ausilio alla gestione delle meccaniche di gioco
*/

//La funzione viene invocata ogni qualvolta il giocatore perde una vita (collisione o tempo scaduto): azzera il checkpoint, riposiziona la rana

int next_manche(object *frog, int *checkpoint)
{
	//Aggiornamento della posizione della rana a livello globale
    pthread_mutex_lock(&frog_mutex);
		frog_data.x = maxx/2-1;
		frog_data.y = maxy-2;
    pthread_mutex_unlock(&frog_mutex);

	frog->x = maxx/2-1;
	frog->y = maxy-2;

	*checkpoint = 0;
	return -1; //Il valore restituito viene utilizzato per decrementare il numero degli hp
}
//----------------------------------------------------------------------------------------------------------------

//Sottofunzione di gameArea: inizializza gli oggetti di tipo 'tana' -------------------------------
void init_den(object den[])
{
	int startx = 2;
	int dendistance=BORDER_LENGTH+DEN_LENGTH+1;

	for(int i=0; i<N_DENS; i++)
	{	
		den[i].x = startx;
		startx = startx+dendistance;
		//In questo caso la variabile signal della struttura object è posta a 1 se la tana è stata conquistata
		den[i].signal = 0;	
	}
}
//----------------------------------------------------------------------------------------------


//Restituisce il valore del più alto punteggio raggiunto letto all'interno di un file -----------
int read_highscore(FILE *fph){
	int highscore = 0;
	//Se non esiste nessun file viene restituito 0: highscore assume 0
	if((fph = fopen("highscore.txt", "r"))==NULL)
			return 0;
	else {
		fscanf(fph, "%d", &highscore);
		fclose(fph);
		return highscore;
	}
}
//----------------------------------------------------------------------------------------------------

//Aggiorna se necessario il valore dell'highscore ------------------------------------------
void write_highscore(FILE *fph, int highscore)
{
	//Se ancora non è presente il file questo viene creato
	fph = fopen("highscore.txt", "w");
	if (fph == NULL) return;
	fprintf(fph, "%d", highscore);
	fclose(fph);
}
//---------------------------------------------------------------------------------------------