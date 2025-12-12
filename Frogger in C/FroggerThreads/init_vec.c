#include "fheader.h"
#include<stdlib.h>

vehicle init_vec(int id, int direction, v_type type, int lane, int x, int y) 
{
	//fprintf(fp, "Inizializzato il veicolo %d con x = %d y = %d\n", id, x, y);
	vehicle v;
	v.id = id;
	v.direction = direction;
	v.type = type;
	v.lane = lane;
	v.x=x;
	v.y=y;
	srand(time(NULL)+id);
	v.color = rand()%5+VEC1;		
	v.nChar_toPrint = 0;
	v.char_toStart = 0;
	//fprintf(fp, "Inizializzato il veicolo %d con direzione = %d x = %d y = %d \n", id, v.direction, v.x, v.y);
	return v;
} 
//Termine inizializzazione veicolo
