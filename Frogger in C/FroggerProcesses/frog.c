#include "header.h"
#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>

//Genera i movimenti dell'oggetto rana a seconda delle decisioni dell'utente. Gestisce il comando 'sparo' e il comando 'quit'
/*
    La funzione rana non genera delle coordinate che poi va ad incrementare; 
    genera invece dei movimenti, che vanno da 0 al massimo valore che la rana può compiere con un singolo spostamento.
    In questo caso la rana si sposta di una casella in orizzontale e di due caselle in verticale
    La funzione gameArea legge il contenuto della pipe; se c'è un movimento incrementa le coordinate della rana contenute localmente nel suo spazio 
    dei dati. GameArea si occupa inoltre di controllare la validità dei movimenti: se il movimento eccede i limiti dello schermo questo non è
    consentito.
    Si adotta questo meccanismo per poter semplificare la gestione del movimento della rana in caso di respawn e di trasporto sul tronco
*/
void fun_frog(int *pipefrog){ //Processo rana -----------------------------------------------------------------------------------------------------------------------------------------------------------
		keypad(gamewin, true);
	object movement;
	movement.x = 0;
	movement.y = 0;
	movement.signal = 0;
	int choise;
    //Sono inizializzati dei timer per la gestione degli spari multipli
    //La rana può sparare un proiettile una volta ogni secondo
	time_t start_timer;
	time_t current_timer;
	
	close(pipefrog[0]);
	start_timer = time(NULL);

	while(true){  //Inizio while true
		choise = wgetch(gamewin);
		movement.c = 'f';
		switch(choise){  //Inizio switch
			
			case KEY_LEFT:
				movement.x = -1;
				movement.y = 0;
				break;

			case KEY_RIGHT:
				movement.x = 1;
				movement.y = 0;
				break;

			case KEY_UP:
				movement.y = -2;
				movement.x = 0;
				break;

			case KEY_DOWN:
				movement.y = 2;
				movement.x = 0;
				break;

			case 32:  //Comando di sparo
				movement.y=0;
				movement.x=0;
				current_timer = time(NULL);
                //Viene concesso solo se il timer conta un secondo o più di un secondo
				if(current_timer - start_timer > 0){
                    //gameArea riconosce il comando di sparo tramite la variabile 'c'
					movement.c='b';   
					start_timer = time(NULL);	
				}
				break;

            //Si invia a gameArea il comando 'quit'
			case 113:
				movement.c = 'q';
				break;
            //Se nessun comando è premuto dall'utente la variabile di sceta è resettata a 0
			default:
				choise = 0;
					
		} //termine switch

        //I dati vengono scritti su pipe solo se viene premuto un comando tra quelli validi
		if(choise){
			//fprintf(fp, "Scritto su Pipe movimento in x: %d, movimento in y: %d\n", movement.x, movement.y);	
			write(pipefrog[1], &movement, sizeof(object));
		}
		movement.x=0;
		movement.y=0;
	}//Termine while true

} // Termine funzione processo rana ------------------------------------------------------------------------------------------------------------------------------------------------------------------
