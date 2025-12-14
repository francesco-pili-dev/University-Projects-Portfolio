#include "fheader.h"
#include<ncurses.h>
#include<unistd.h>
#include<locale.h>

/*
    *Il file in questione contiene diverse funzioni di supporto alla stampa a video
*/

//Supporto di gameArea: stampa gli oggetti statici dello schermo di gioco ------------------------------
void print_gamefield(){    
    //Stampa a video del prato ------------------------------
    wattron(gamewin, COLOR_PAIR(1));

    for(int k=3; k<FIST_LOG_Y; k++)
    {
        for(int j=0; j<maxx; j++)
            mvwaddch(gamewin, k, j, ' ');
    }
    wattroff(gamewin, COLOR_PAIR(1));
    //---------------------------------------------------

    //Stampa a video del fiume -----------------------
    wattron(gamewin, COLOR_PAIR(2));
    int riverRandomizer=0;
    for(int k=FIST_LOG_Y; k<last_log_y+1; k++)
    {
        for(int j=0; j<maxx; j++)
        {
            //srand(timer+riverRandomizer);
            //riverRandomizer++;
                //mvaddch(k, j, (rand()%15)?' ':'~');
            mvwaddch(gamewin, k, j,' ');
        }
    }
    wattroff(gamewin, COLOR_PAIR(2));
    //------------------------------------------------------

    //Stampa a video del marciapiede ----------------------------------
	wattron(gamewin, COLOR_PAIR(15));
	for(int k=last_log_y+1; k<maxy; k++)
	{
		if(k==first_lane-1)
				k=first_lane+nlanes*2+1;
				for(int j=0; j<maxx; j++)
					mvwaddch(gamewin, k, j, ' ');	
	}
	wattroff(gamewin, COLOR_PAIR(15));
    //----------------------------------------------------------------------

    //Bordi autostrada
    mvwprintw(gamewin, first_lane-2, 0,              "_______________________________________________________________________________");
    mvwprintw(gamewin, last_lane+1, 0,               "_______________________________________________________________________________");

    //Separatori di corsia
    for(int i=1; i<nlanes; i++)
        mvwprintw(gamewin, first_lane + (i*2)-1, 0,  "-------------------------------------------------------------------------------");
}
//------------------------------------------------------------------------------------------------------------------------------------

//Sottofunzione di gameArea: stampa a video in una finestra sottostante quella di gioco il punteggio attuale e l'highscore
void print_score(WINDOW *win_score, int score, int highscore)
{
	//viene passata come parametro la finestra su cui stampare 'score' e 'highscore'
    //La finestra è collocata nella parte bassa della schermo
	wclear(win_score);

	wattron(win_score, COLOR_PAIR(11));
	mvwprintw(win_score, 1, 3, "SCORE"); 
	mvwprintw(win_score, 2, 3, "%d",score); 
	wattroff(win_score, COLOR_PAIR(11));
	wattron(win_score, COLOR_PAIR(13));
	mvwprintw(win_score, 1, 10, "HIGH-SCORE"); 
	mvwprintw(win_score, 2, 10, "%d",highscore); 
	wattron(win_score, COLOR_PAIR(13));
	
	wrefresh(win_score);
}
//--------------------------------------------------------------------------------------------------------------------------

//Stampa a video degli HP: supporto di UNICODE -------------------------------------------------------------------------
void print_hps(int hp)
{
	int cnt = 3;
		mvwprintw(gamewin, 1, 1, "HP");
		for(int i=1; i<=hp; i++)
		{
			wattron(gamewin, COLOR_PAIR(22));
			mvwprintw(gamewin, 1, i+cnt, "\U00002665\n");
			wattroff(gamewin, COLOR_PAIR(22));
			cnt = cnt+1;
		}
}
//----------------------------------------------------------------------------------------------------------------------------

//Gestisce il cronometro e stampa a video il tempo di gioco rimanente --------------------------------------------------------
void print_time(int time_limit, int timer)
{
    //Il timer cambia colore a seconda del tempo rimanente
	if(timer<20)
			wattron(gamewin, COLOR_PAIR(1));
	else if(timer >= 20 && timer < 30)
			wattron(gamewin, COLOR_PAIR(4));
	else if(timer >=30 && timer <=40)
			wattron(gamewin, COLOR_PAIR(3));

    //Stampa della barra del tempo
    int count=0;	
    for(int i=time_limit-timer; i >=0; i--)
        mvwprintw (gamewin, 1,34+count++," ");

    wattroff(gamewin, COLOR_PAIR(1));
    wattroff(gamewin, COLOR_PAIR(4));
    wattroff(gamewin, COLOR_PAIR(3));

    wattron(gamewin, COLOR_PAIR(14));
    mvwprintw (gamewin, 1,29,"TIME");
    wattroff(gamewin, COLOR_PAIR(14));

        //-------------------------------------------------------------------------------------------------
}
//---------------------------------------------------------------------------------------------------------

//Stampa a video delle tane ------------------------------------------------------------------------------
void print_dens(object *den)
{
    wattron(gamewin, COLOR_PAIR(2));
    for(int k=4; k<FIST_LOG_Y; k++)
		{
			for(int x=0; x<maxx; x++)
				for(int i=0; i<N_DENS; i++)
				{
					if(x >= den[i].x && x < den[i].x+DEN_LENGTH)
						mvwaddch(gamewin, k, x, ' ');
					if(den[i].signal == 1){
						wattron(gamewin, COLOR_PAIR(1));
                        //Una rana viene stampata all'intern dalla tana se questa è stata conquistata
						mvwprintw(gamewin, 5, den[i].x+2, "*^*");
						wattroff(gamewin, COLOR_PAIR(1));
						wattron(gamewin, COLOR_PAIR(2));
					}
					
				}
		}
		wattroff(gamewin, COLOR_PAIR(2));	
}
//----------------------------------------------------------------------------------------------------------