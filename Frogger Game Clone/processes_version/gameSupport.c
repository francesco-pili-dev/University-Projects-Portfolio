#include "header.h"
#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>


//IL FILE IN QUESTIONE ALCUNE SOTTOFUNZIONI DI SUPPORTO DI GAMEAREA


//Sottofunzione di gameArea: gestisce il caso in cui si perda una vita ------------------------
int next_manche(object *frog, int *checkpoint)
{
    //La rana viene riportata alla posizione iniziale
	frog->x = maxx/2-1;
	frog->y = maxy-2;
    //----------------------------------------------
    //Il checkpoint viene azzerato
	*checkpoint = 0;
    //Il valore restituito viene utilizzato dalla variabile 'hp' di gameArea per decrementare il numero di vite
	return -1;
}
//---------------------------------------------------------------------------------------

//Inizializza le strutture locali di gameArea ----------------------------------------------------
void init_structures(object *logs, vehicle *vehicles)
{
	for(int i=0; i<nlogs; i++)
	{
		logs[i].x=-1;
		logs[i].y = -1;	
		logs[i].signal = 0;
		logs[i].c = 'l';
	}
	for(int i=0; i<nVehicles; i++)
	{
		vehicles[i].x = -1;
		vehicles[i].y = -1;
		vehicles[i].nChar_toPrint=0;
		vehicles[i].char_toStart=0;
		vehicles[i].direction = 0;
	}

}
//---------------------------------------------------------------------------------------------

//Sottofunzione di gameArea: inizializza gli oggetti di tipo 'tana' -------------------------------
void fun_den(object den[])
{
int startx = 2;
int dendistance=BORDER_LENGTH+DEN_LENGTH+1;

	for(int i=0; i<N_DENS; i++)
	{	
		den[i].x = startx;
		startx = startx+dendistance;
		den[i].signal = 0;	
	}
}
//----------------------------------------------------------------------------------------------

//Restituisce il valore del più alto punteggio raggiunto letto all'interno di un file -----------
int read_highscore(FILE *fph){
	int highscore = 0;
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
	fph = fopen("highscore.txt", "w");
	if (fph == NULL) return;
	fprintf(fph, "%d", highscore);
	fclose(fph);
}
//---------------------------------------------------------------------------------------------