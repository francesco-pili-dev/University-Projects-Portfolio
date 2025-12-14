#include "fheader.h"
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

//Gestione del menù di gioco -----------------------------------------------------------------------------------------------------------------
int menu()
{
	initscr();
	curs_set(0);
	noecho();
	cbreak();
	start_color();

	getmaxyx(stdscr, maxy, maxx);
	WINDOW *menuwin = newwin(36, 79, 0, 0);
	box(menuwin, ACS_VLINE, ACS_HLINE);
	keypad(menuwin, TRUE);

	WINDOW *boxmenu = newwin(14, 22, 15,29);
	
	keypad(boxmenu, TRUE);
	init_color(COLOR_DARK, 0, 0, 0);
	init_pair(1, COLOR_DARK, COLOR_GREEN);
	init_pair(2, COLOR_DARK, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_DARK);
	int displacement = 6;
	short start = 0;
	int index = 0;
	int times=0;
	int choice = -1;
	int position = 0;

	Mix_Music *menumusic;
	Mix_Chunk *slidesound;
	Mix_Chunk *selectsound;
	menumusic = Mix_LoadMUS("sounds/menumusic.mp3");
	slidesound = Mix_LoadWAV("sounds/slide.mpga");
	selectsound = Mix_LoadWAV("sounds/select.mpga");
	Mix_PlayMusic(menumusic,1);
	Mix_VolumeMusic(50);
	Mix_VolumeChunk(slidesound,128);
	Mix_VolumeChunk(selectsound,128);

	do{
		wattron(menuwin, COLOR_PAIR(1));
		if(times == 0){
			for(index = 36; index>=0; index --){
				wclear(menuwin);				
				mvwprintw(menuwin, 3+index, 3+displacement, "%s", "       ");       
				mvwprintw(menuwin, 4+index, 3+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 3+displacement, "%s", "     ");
				mvwprintw(menuwin, 6+index, 3+displacement, "%s", "  ");
				mvwprintw(menuwin, 7+index, 3+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 3+displacement, "%s", "  ");
				
				mvwprintw(menuwin, 3+index, 12+displacement, "%s", "       ");       
				mvwprintw(menuwin, 4+index, 12+displacement, "%s", "  "); mvwprintw(menuwin, 4+index, 17+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 12+displacement, "%s", "       ");
				mvwprintw(menuwin, 6+index, 12+displacement, "%s", "  "); mvwprintw(menuwin, 6+index, 15+displacement, "%s", "  ");
				mvwprintw(menuwin, 7+index, 12+displacement, "%s", "  "); mvwprintw(menuwin, 7+index, 16+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 12+displacement, "%s", "  "); mvwprintw(menuwin, 8+index, 17+displacement, "%s", "  ");

				mvwprintw(menuwin, 3+index, 21+displacement, "%s", "       ");
				mvwprintw(menuwin, 4+index, 21+displacement, "%s", "  "); mvwprintw(menuwin, 4+index, 26+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 21+displacement, "%s", "  "); mvwprintw(menuwin, 5+index, 26+displacement, "%s", "  ");
				mvwprintw(menuwin, 6+index, 21+displacement, "%s", "  "); mvwprintw(menuwin, 6+index, 26+displacement, "%s", "  ");
				mvwprintw(menuwin, 7+index, 21+displacement, "%s", "  "); mvwprintw(menuwin, 7+index, 26+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 21+displacement, "%s", "       "); 

				mvwprintw(menuwin, 3+index, 30+displacement, "%s", "       ");
				mvwprintw(menuwin, 4+index, 30+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 30+displacement, "%s", "  "); 
				mvwprintw(menuwin, 6+index, 30+displacement, "%s", "  "); mvwprintw(menuwin, 6+index, 34+displacement, "%s", "   ");
				mvwprintw(menuwin, 7+index, 30+displacement, "%s", "  "); mvwprintw(menuwin, 7+index, 35+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 30+displacement, "%s", "       "); 

				mvwprintw(menuwin, 3+index, 39+displacement, "%s", "       ");
				mvwprintw(menuwin, 4+index, 39+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 39+displacement, "%s", "  "); 
				mvwprintw(menuwin, 6+index, 39+displacement, "%s", "  "); mvwprintw(menuwin, 6+index, 43+displacement, "%s", "   ");
				mvwprintw(menuwin, 7+index, 39+displacement, "%s", "  "); mvwprintw(menuwin, 7+index, 44+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 39+displacement, "%s", "       "); 

				mvwprintw(menuwin, 3+index, 48+displacement, "%s", "       ");       
				mvwprintw(menuwin, 4+index, 48+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 48+displacement, "%s", "     ");
				mvwprintw(menuwin, 6+index, 48+displacement, "%s", "  ");
				mvwprintw(menuwin, 7+index, 48+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 48+displacement, "%s", "       ");
					
				mvwprintw(menuwin, 3+index, 57+displacement, "%s", "       ");       
				mvwprintw(menuwin, 4+index, 57+displacement, "%s", "  "); mvwprintw(menuwin, 4+index, 62+displacement, "%s", "  ");
				mvwprintw(menuwin, 5+index, 57+displacement, "%s", "       ");
				mvwprintw(menuwin, 6+index, 57+displacement, "%s", "  "); mvwprintw(menuwin, 6+index, 60+displacement, "%s", "  ");
				mvwprintw(menuwin, 7+index, 57+displacement, "%s", "  "); mvwprintw(menuwin, 7+index, 61+displacement, "%s", "  ");
				mvwprintw(menuwin, 8+index, 57+displacement, "%s", "  "); mvwprintw(menuwin, 8+index, 62+displacement, "%s", "  ");	
				wrefresh(menuwin);
				usleep(70000);
			}
			times++;
		}
		   
		wattroff(menuwin, COLOR_PAIR(1));
		wrefresh(menuwin);

	do
	{
		wclear(boxmenu);
		box(boxmenu, ACS_VLINE, ACS_HLINE);
		
		wattron(boxmenu, COLOR_PAIR((position==0)?2:3));
		mvwprintw(boxmenu, 1, 2,"                  ");
		mvwprintw(boxmenu, 2, 2,"      GIOCA       ");
		mvwprintw(boxmenu, 3, 2,"                  ");
		wattroff(boxmenu, COLOR_PAIR((position==0)?3:2));

		wattron(boxmenu, COLOR_PAIR((position==1)?2:3));
		mvwprintw(boxmenu, 4, 2,"                  ");
		mvwprintw(boxmenu, 5, 2,"   IMPOSTAZIONI   ");
		mvwprintw(boxmenu, 6, 2,"                  ");
		wattroff(boxmenu, COLOR_PAIR((position==1)?3:2));
		
		wattron(boxmenu, COLOR_PAIR((position==2)?2:3));
		mvwprintw(boxmenu, 7, 2,"                  ");
		mvwprintw(boxmenu, 8, 2,"   GAME CREDITS   ");
		mvwprintw(boxmenu, 9, 2,"                  ");
		wattroff(boxmenu, COLOR_PAIR((position==2)?3:2));

		wattron(boxmenu, COLOR_PAIR((position==3)?2:3));
		mvwprintw(boxmenu, 10, 2,"                  ");
		mvwprintw(boxmenu, 11, 2,"       ESCI       ");
		mvwprintw(boxmenu, 12, 2,"                  ");
		wattroff(boxmenu, COLOR_PAIR((position==3)?3:2));

		timeout(0);
		choice = wgetch(boxmenu);
		
		switch(choice)
		{
			case KEY_UP:
				Mix_PlayChannel(-1,slidesound,0);
				usleep(5000);
				
				position --;
				if(position<0)
					position = 3;

				break;
			
			case KEY_DOWN:
				Mix_PlayChannel(-1,slidesound,0);
				usleep(5000);
				position ++;
				if(position>3)
					position = 0;
				break;

			case 10:
				Mix_PlayChannel(-1,selectsound,0);
				sleep(1);
				choice = position;
				break;
			
			default:
				choice = -1;
				break;
		}
		if(choice == 2)
		{
			wclear(boxmenu);
			WINDOW *credits = newwin(11, 45, 15,10);
			keypad(credits, TRUE);
			keypad(boxmenu, FALSE);
			int goback = 0;
			do{
				wclear(credits);
				box(credits, ACS_VLINE, ACS_HLINE);
				mvwprintw(credits, 2, 5, "Frogger ver. threads ");
				mvwprintw(credits, 3, 5, "Floris Nicola - Pili francesco ");
				mvwprintw(credits, 4, 5, "IADA (00035 - 00067) corso SOPR ");
				mvwprintw(credits, 8, 5, "premi il tasto q per tornare indietro ");
				//wtimeout(credits, 0);
				goback = getchar();
				wrefresh(credits);
			}while(goback != 113);
			wclear(credits);
			wrefresh(credits);
			choice = -1;
			keypad(boxmenu, TRUE);
			delwin(credits);

		}
		if(choice == 1){
			position = 0;
			wclear(boxmenu);
			wrefresh(boxmenu);
			WINDOW *optmenu = newwin(11, 22, 15,29);
			keypad(optmenu, TRUE);
			keypad(boxmenu, FALSE);
			do{
				
				wclear(optmenu);
				box(optmenu, ACS_VLINE, ACS_HLINE);
				
				wattron(optmenu, COLOR_PAIR((position==0)?2:3));
				mvwprintw(optmenu, 1, 2,"                  ");
				mvwprintw(optmenu, 2, 2,"      VOLUMI      ");
				mvwprintw(optmenu, 3, 2,"                  ");
				wattroff(optmenu, COLOR_PAIR((position==0)?3:2));

				wattron(optmenu, COLOR_PAIR((position==1)?2:3));
				mvwprintw(optmenu, 4, 2,"                  ");
				mvwprintw(optmenu, 5, 2,"    DIFFICOLTA    ");
				mvwprintw(optmenu, 6, 2,"                  ");
				wattroff(optmenu, COLOR_PAIR((position==1)?3:2));
				
				wattron(optmenu, COLOR_PAIR((position==2)?2:3));
				mvwprintw(optmenu, 7, 2,"                  ");
				mvwprintw(optmenu, 8, 2,"     INDIETRO     ");
				mvwprintw(optmenu, 9, 2,"                  ");
				wattroff(optmenu, COLOR_PAIR((position==2)?3:2));

				choice = wgetch(optmenu);
			
			switch(choice)
			{
				case KEY_UP:
					Mix_PlayChannel(-1,slidesound,0);
					usleep(5000);
					
					position --;
					if(position<0)
						position = 2;

					break;
				
				case KEY_DOWN:
					Mix_PlayChannel(-1,slidesound,0);
					usleep(5000);
					position ++;
					if(position>2)
						position = 0;
					break;

				case 10:
					Mix_PlayChannel(-1,selectsound,0);
					sleep(1);
					choice = position;
					break;
				
				default:
					choice = -1;
					break;
			}

				if(choice==1)
				{
					wclear(optmenu);
					wrefresh(optmenu);
					WINDOW *diffmenu = newwin(14, 22, 15,29);
					keypad(optmenu, FALSE);
					keypad(diffmenu, TRUE);
					
					position = diff;
					do{
						wclear(diffmenu);
						box(diffmenu, ACS_VLINE, ACS_HLINE);
						wattron(diffmenu, COLOR_PAIR((position==0)?2:3));
						mvwprintw(diffmenu, 1, 2,"                  ");
						mvwprintw(diffmenu, 2, 2,"       EASY       ");
						mvwprintw(diffmenu, 3, 2,"                  ");
						wattroff(diffmenu, COLOR_PAIR((position==0)?3:2));

						wattron(diffmenu, COLOR_PAIR((position==1)?2:3));
						mvwprintw(diffmenu, 4, 2,"                  ");
						mvwprintw(diffmenu, 5, 2,"      NORMAL      ");
						mvwprintw(diffmenu, 6, 2,"                  ");
						wattroff(diffmenu, COLOR_PAIR((position==1)?3:2));
					
						wattron(diffmenu, COLOR_PAIR((position==2)?2:3));
						mvwprintw(diffmenu, 7, 2,"                  ");
						mvwprintw(diffmenu, 8, 2,"       HARD       ");
						mvwprintw(diffmenu, 9, 2,"                  ");
						wattroff(diffmenu, COLOR_PAIR((position==2)?3:2));

						wattron(diffmenu, COLOR_PAIR((position==3)?2:3));
						mvwprintw(diffmenu, 10, 2,"                  ");
						mvwprintw(diffmenu, 11, 2,"  GOD OF FROGGER  ");
						mvwprintw(diffmenu, 12, 2,"                  ");
						wattroff(diffmenu, COLOR_PAIR((position==3)?3:2));

						choice = wgetch(diffmenu);

						switch(choice)
						{
							case KEY_UP:
								Mix_PlayChannel(-1,slidesound,0);
								usleep(5000);
							
							position --;
							if(position<0)
								position = 3;

							break;
						
							case KEY_DOWN:
								Mix_PlayChannel(-1,slidesound,0);
								usleep(5000);

								position ++;
							if(position>3)
								position = 0;
							break;

							case 10:
								Mix_PlayChannel(-1,selectsound,0);
								sleep(1);	
								choice = position;
								diff = position;			
							break;
						
							default:
								choice = -1;
								break;
						}
						wrefresh(diffmenu);
					}while(choice != position);
					keypad(diffmenu,FALSE);
					wclear(diffmenu);
					wrefresh(diffmenu);
					delwin(diffmenu);
					position = 0;
				}
				keypad(optmenu, TRUE);
				wrefresh(optmenu);
			
			}while(choice != 2);

			keypad(boxmenu, TRUE);
			delwin(optmenu);
			position = 0;
		}
		wrefresh(boxmenu);
	}while(choice!=0 && choice!=3);
		
	Mix_FreeChunk(slidesound);
	Mix_FreeChunk(selectsound);
	Mix_FreeMusic(menumusic);	
		
	}while(choice!=0 && choice!=3);

	delwin(menuwin);
	delwin(boxmenu);
	endwin();
	return(choice);
	
}

//Termine gestione del menù di gioco  ----------------------------------------------------------------------------------------------------------
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
		//fprintf(fp, "c: %d choise: %d pos: %d\n",c,choice,position);
		wrefresh(intr_win);
	}while(choice!=0 && choice!=1);
	if(choice == 0 && itype == WIN)
		choice = 2;

	return(choice);
	endwin();
}