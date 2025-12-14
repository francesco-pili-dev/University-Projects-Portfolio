#include "fheader.h"
#include<ncurses.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>

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
