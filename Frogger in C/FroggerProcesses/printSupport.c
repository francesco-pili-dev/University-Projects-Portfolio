#include "header.h"
#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>

//IL FILE CORRENTE CONTIENE ALCUNE FUNZIONI DI SUPPORTO ALLA STAMPA DEL CAMPO DI GIOCO

void print_gamefield(int hp, int time_limit, int timer, time_t *start_game_clock, object *frog, object *den)
{
    //Stampa a video degli HP: supporto di UNICODE -------------------------
	int cnt = 3;
		mvwprintw(gamewin, 1, 1, "HP");
		for(int i=1; i<=hp; i++)
		{
			wattron(gamewin, COLOR_PAIR(22));
			mvwprintw(gamewin, 1, i+cnt, "\U00002665\n");
			wattroff(gamewin, COLOR_PAIR(22));
			cnt = cnt+1;
		}
    //-----------------------------------------------------------
	
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

    //Gestione del cronometro ---------------------------------------------
	if(timer<20)
			wattron(gamewin, COLOR_PAIR(1));
		else if(timer >= 20 && timer < 30)
			wattron(gamewin, COLOR_PAIR(4));
		else if(timer >=30 && timer <=40)
			wattron(gamewin, COLOR_PAIR(3));
	
		if(time_limit - timer == 0)
		{
			hp--;
			frog->x = maxx/2;
			frog->y = maxy -2;
			*start_game_clock = time(NULL);
		}
		int count=0;	
		for(int i=time_limit-timer; i >=0; i--)
		{
			mvwprintw (gamewin, 1,34+count++," ");
		}
		wattroff(gamewin, COLOR_PAIR(1));
		wattroff(gamewin, COLOR_PAIR(4));
		wattroff(gamewin, COLOR_PAIR(3));
		//mvwprintw (gamewin, 1,34+time_limit,"%d",time_limit-timer);
		wattron(gamewin, COLOR_PAIR(14));
		mvwprintw (gamewin, 1,29,"TIME");
		wattroff(gamewin, COLOR_PAIR(14));
        //-------------------------------------------------------------------------------------------------

        //Termine e inizio autostrada -----------------------------------------------------------------------
		mvwprintw(gamewin, last_lane+1, 0,  "_______________________________________________________________________________");
		mvwprintw(gamewin, first_lane-2, 0, "_______________________________________________________________________________");
		//Separatori di corsia
    	for(int i=1; i<nlanes; i++)
        	mvwprintw(gamewin, first_lane + (i*2)-1, 0,  "-------------------------------------------------------------------------------");
        //--------------------------------------------------------------------------------------------------------------------------

        //Stampa dell'area relativa a fiume e tane ----------------------------------------------------
		wattron(gamewin, COLOR_PAIR(1));
		for(int k=3; k<FIST_LOG_Y; k++)
		{
			for(int j=0; j<maxx; j++)
				mvwaddch(gamewin, k, j, ' ');
		}

		wattroff(gamewin, COLOR_PAIR(1));	
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
        //------------------------------------------------------------------------

        //Gestione stampa tane ------------------------------------------------
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
    //---------------------------------------------------------------------------------
}

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

//Contiene le definizioni dei colori ------------------------------------------------------------------------------
void set_colors()
{
	 init_color(COLOR_BROWN, 500, 200, 100);
	init_color(COLOR_DARK, 0, 0, 0);
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_WHITE, COLOR_CYAN);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_YELLOW);
    init_pair(9, COLOR_WHITE, COLOR_BROWN);
	init_pair(12,COLOR_WHITE, COLOR_DARK);
	init_pair(13, COLOR_RED, COLOR_DARK);
	init_pair(14, COLOR_YELLOW, COLOR_BLACK);
	init_pair(15, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(VEC1, COLOR_WHITE, COLOR_CYAN);
	init_pair(VEC6, COLOR_WHITE, COLOR_YELLOW);
	init_pair(VEC2, COLOR_WHITE, COLOR_RED);
	init_pair(VEC3, COLOR_WHITE, COLOR_BLUE);
	init_pair(VEC4, COLOR_WHITE, COLOR_GREEN);
	init_pair(VEC5, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(22, COLOR_RED, COLOR_BLACK);
	init_pair(23, COLOR_DARK, COLOR_WHITE);
}
//-----------------------------------------------------------------------------------------------------------------