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

//Viene richiamata ogni qual volta il gioco termina, permette all'utente di decidere se continuare/riprovare il gioco oppure terminarne l'esecuzione
int intr_menu(interrup_type itype)
{
	WINDOW *intr_win = newwin(10,30, 13, 25);
	
	keypad(intr_win,true);
	int choice = -1;
	int c;
	int position=0;
	
	do
	{
		wclear(intr_win);
		box(intr_win, ACS_VLINE, ACS_HLINE);
		
		if(itype == QUIT){
			mvwprintw(intr_win, 3, 10,"  Q U I T ");
			wattron(intr_win, COLOR_PAIR((position==0)?23:12));
			mvwprintw(intr_win, 6, 5, " Riprova ");
			wattroff(intr_win, COLOR_PAIR((position==0)?23:12));
		}
		else{
			mvwprintw(intr_win, 3, 10,(itype==GAMEOVER)?"GAME  OVER":" YOU  WIN ");
			wattron(intr_win, COLOR_PAIR((position==0)?23:12));
			mvwprintw(intr_win, 6, 5,(itype==GAMEOVER)?" Riprova ":"Continua");
			wattroff(intr_win, COLOR_PAIR((position==0)?23:12));
		}
		
		wattron(intr_win, COLOR_PAIR((position==0)?12:23));
		mvwprintw(intr_win, 6, 16," Termina ");
		wattroff(intr_win, COLOR_PAIR((position==0)?12:23));

		timeout(0);
		c = wgetch(intr_win);
		
		
		Mix_Chunk *slidesound;
		Mix_Chunk *selectsound;
		
		slidesound = Mix_LoadWAV("sounds/slide.mpga");
		selectsound = Mix_LoadWAV("sounds/select.mpga");

		switch(c)
		{
			case KEY_LEFT:
				Mix_PlayChannel(-1,slidesound,0);
				usleep(5000);
				position = 0;
				break;
			
			case KEY_RIGHT:
				Mix_PlayChannel(-1,slidesound,0);
				usleep(5000);
				position = 1;
				break;

			case 10:
				Mix_PlayChannel(-1,selectsound,0);
				usleep(1);
				choice = position;
				break;
			
			default:
				choice = -1;
				break;
		}
		fprintf(fp, "c: %d choise: %d pos: %d\n",c,choice,position);
		wrefresh(intr_win);
	}while(choice!=0 && choice!=1);
	if(choice == 0 && itype == WIN)
		choice = 2;

	return(choice);
	endwin();
}