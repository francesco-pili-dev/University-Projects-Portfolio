#include "fheader.h"
#include<ncurses.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<locale.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>

void *funcVehicle(void *args) {
    int id;
	struct thread_data *td;
	td = (struct thread_data *) args;
	id = td->thread_id;
	//fprintf(fp, "FunVehicle thread  numero %d \n", id);
    long usleep_time = 100000-15000*diff;

	//fprintf(fp, "\nLetto da globale veicolo %d direzione %d\n", vehicles[id].id, vehicles[id].direction);
    pthread_mutex_lock(&vec_mutex[id]);
	vehicle v = vehicles[id];
    pthread_mutex_unlock(&vec_mutex[id]);

	while(true){ //Inizio while true -----------------------------------------------

		if(v.direction)  // Senso di marcia positivo -----------------------------------------------------
		{
			if(v.x >= maxx-TRUCK)
				v.nChar_toPrint --;
			else {
				if(v.x <= 0)
				{
					v.nChar_toPrint++;
					if(!v.char_toStart)
						v.char_toStart = TRUCK-1;
					else v.char_toStart --;
				}
				else
				{
					v.nChar_toPrint = TRUCK;
					v.char_toStart = 0;
				}
			}
			if(!v.char_toStart)
				v.x++;
			if(v.x == maxx){
				srand(time(NULL)+v.y);
				int randlane = rand()%5+1;
				int randtype = rand()%3;
				v.color = rand()%5+VEC1;
				v.type = (randtype)?CAR:TRUCK;
				v.x = 0;
				if(randlane)
				{
					if(nlanes < 5){
						if(v.y <= first_lane + (nlanes/2))
						{
							v.y += nlanes + ((nlanes%2)?1:0);
						}
						else if(v.y >= first_lane + nlanes){
							v.y -= nlanes + ((nlanes%2)?1:0);
						}
					}
					else {
							if(v.y == first_lane)
							v.y = last_lane;
						else if(v.y == last_lane)
							v.y = first_lane;
						else if(v.y == first_lane + 2)
							v.y = last_lane-2;
						else if(v.y == first_lane - 2)
							v.y = last_lane+2;
					}
				}
			}
		}//Fine gestione senso di marcia positivo-----------------------------------------------------------------
		else
		{ //Inizio gestione senso di marcia inverso
			if(v.x <= maxx-1 && v.x > maxx-TRUCK)
				v.nChar_toPrint++;
				if(v.nChar_toPrint > TRUCK)
					v.nChar_toPrint = TRUCK;
			else
			{
				if(v.x<=0 && v.x<TRUCK)
				{
					v.nChar_toPrint--;
					v.char_toStart++;
				}
				else
				{
					v.char_toStart = 0;
					v.nChar_toPrint = TRUCK;
				}				
			}

		if(v.x<=0 && v.nChar_toPrint == 0){
			v.x=maxx;
			srand(time(NULL)+v.y);
			int randlane = rand()%5+1;
			int randtype = rand()%3;
			v.type = (randtype)?CAR:TRUCK;
		
			if(randlane)
			{
				if(nlanes < 5)
				{
					if(v.y <= first_lane + (nlanes/2))
					{
						v.y += nlanes + ((nlanes%2)?1:0);
					}
					else if(v.y >= first_lane + nlanes){
						v.y -= nlanes + ((nlanes%2)?1:0);
					}
				}
				else {
						if(v.y == first_lane)
							v.y = last_lane;
						else if(v.y == last_lane)
							v.y = first_lane;
						else if(v.y == first_lane + 2)
							v.y = last_lane-2;
						else if(v.y == first_lane - 2)
							v.y = last_lane+2;
					}

			}

			v.color = rand()%5+VEC1;
		}
		
		if(!v.char_toStart)
			v.x--;	

		} //Fine gestione senso di marcia inverso

		pthread_mutex_lock(&vec_mutex[id]);
	        vehicles[id] = v ;
        pthread_mutex_unlock(&vec_mutex[id]);
        //fprintf(fp, "%d scrittura Veicolo %d: x = %d,  y = %d direzione = %d\n", id, vehicles[id].id, vehicles[id].x, vehicles[id].y, vehicles[id].direction);
		usleep(usleep_time);
		
		
	} //Fine while true
} //Termine fun_vehicle----------------------------------------------
