ft3: fmain.o menu.o gameSupport.o collisions.o printSupport.o print_objects.o funcFrog.o funcLog.o funcVehicle.o funcBullet.o gameArea.o init_vec.o printVehicle.o screen_manager.o set_colors.o globals.o

	gcc fmain.o menu.o gameSupport.o collisions.o printSupport.o print_objects.o funcFrog.o funcLog.o funcVehicle.o funcBullet.o gameArea.o init_vec.o printVehicle.o screen_manager.o set_colors.o globals.o -lncursesw -lpthread -lSDL2 -lSDL2_mixer -o ft3

globals.o: globals.c fheader.h
	gcc -c globals.c -lpthread
	
fmain.o: fmain.c fheader.h
	gcc -c fmain.c -lncursesw -lpthread 

funcFrog.o: funcFrog.c fheader.h
	gcc -c funcFrog.c -lncursesw -lpthread 

funcVehicle.o: funcVehicle.c fheader.h
	gcc -c funcVehicle.c -lncursesw -lpthread

gameArea.o: gameArea.c fheader.h
	gcc -c gameArea.c -lncursesw -lpthread -lSDL2 -lSDL2_mixer

init_vec.o: init_vec.c fheader.h
	gcc -c init_vec.c -lncursesw -lpthread

printVehicle.o: printVehicle.c fheader.h
	gcc -c printVehicle.c -lncursesw -lpthread

screen_manager: screen_manager.c fheader.h
	gcc -c screen_manager -lncursesw -lpthread

set_colors.o: set_colors.c fheader.h
	gcc -c set_colors.c -lncursesw -lpthread

funcLogs.o: funcLog.c fheader.h
	gcc -c funcLog.c -lncursesw -lpthread

funcBullet.o: funcBullet.c fheader.h
	gcc -c funcBullet.c -lpthread

printSupport.o: printSupport.c fheader.h
	gcc -c printSupport.c -lncursesw

print_objects.o: print_objects.c fheader.h
	gcc -c print_objects.c -lncursesw

gameSupport.o: gameSupport.c fheader.h
	gcc -c gameSupport.c -lpthread

collisions.o: collisions.c fheader.h
		gcc -c collisions.c -lpthread
menu.o: menu.c fheader.h
	gcc -c menu.c -lncursesw -lSDL2 -lSDL2_mixer

		
clean:
	rm *.o







