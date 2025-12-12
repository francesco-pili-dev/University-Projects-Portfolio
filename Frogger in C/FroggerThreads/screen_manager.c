#include "fheader.h"
void screen_manager()
{
	last_log_y = nlogs*2+FIST_LOG_Y;
	first_lane = last_log_y+SIDEWALK_LENGTH+2;
	last_lane = first_lane+(nlanes*2)-2;
	maxy = last_lane + SIDEWALK_LENGTH+3;

	maxnum_enemies = diff+1;
	if(diff==EASY)
		nlanes = 3;
	else nlanes = 4;
}