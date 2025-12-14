#include "header.h"
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

//Genera le coordinate dell'oggetto 'proiettile'. Separa i casi in cui lo sparo è causato dall'utente o dai nemici. 
//Termina una volta raggiunti i limiti dello schermo

/*PARAMETRI:
    *La funzione 'bullet' viene richiamata da un processo figlio di gameArea. I parametri passati sono:
        1. Pipe su cui scrivere le coordinate
        2. coordinata iniziale x in cui il proiettile è stato generato
        3. coordinata iniziale y in cui il proiettile è stato generato
        4: 'id' viene utilizzata per distinguere i vari proiettili generarti durante lo stesso lasso di tempo
        5. 'shooter' contiene 'b' se il proiettile è stato sparato dalla rana, 'B' se dal nemico
*/
void fun_bullet(int *pipebullet, int startx, int starty, int id, char shooter)
{

	object bullet;	
	bullet.y = starty;
	bullet.x = startx;
	bullet.c = shooter;
	bullet.signal = id;

	close(pipebullet[0]);
	
    //La velocità del proiettile varia in base a chi l'ha generato
	long time = (shooter == 'b')?50000:100000;
	
	while(bullet.y != -1)
	{
		
		if(shooter == 'b')
		{
            //Il proiettile sparato dalla rana viaggia dal basso verso l'alto
			bullet.y--;
			if(bullet.y==0)
				bullet.y = -1;
		}
		else
		{
            //Il proiettile sparato dal nemico viaggia dall'alto verso il basso
			bullet.y++;
			if(bullet.y==maxy)
				bullet.y = -1;			
		}	
        //La traiettoria del proiettile è sempre verticale come da specifiche
        //Scrittura delle nuove coordinate su pipe
		write(pipebullet[1],&bullet,sizeof(object));
		
		usleep(time);
	}	

}
