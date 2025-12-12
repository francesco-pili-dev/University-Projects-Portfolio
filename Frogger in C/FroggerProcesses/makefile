frogger_processes: main.o screen_manager.o gameArea.o gameSupport.o printSupport.o vehicles.o bullet.o frog.o log.o menu.o interruptions.o globals.o
	gcc main.o screen_manager.o gameArea.o gameSupport.o printSupport.o vehicles.o bullet.o frog.o log.o menu.o interruptions.o globals.o -lncursesw -lSDL2 -lSDL2_mixer -o frogger_processes

globals.o: globals.c header.h
	gcc -c globals.c -lncursesw

main.o: main.c header.h
	gcc -c main.c -lncursesw

screen_manager.o: screen_manager.c header.h
	gcc -c screen_manager.c

gameArea.o: gameArea.c header.h
	gcc -c gameArea.c -lncursesw -lSDL2 -lSDL2_mixer

gameSupport.o: gameSupport.c header.h
	gcc -c gameSupport.c -lncursesw

printSupport.o: printSupport.c header.h
	gcc -c printSupport.c -lncursesw

vehicles.o: vehicles.c header.h
	gcc -c vehicles.c -lncursesw

bullet.o: bullet.c header.h
	gcc -c bullet.c

frog.o: frog.c header.h
	gcc -c frog.c -lncursesw

log.o: log.c header.h
	gcc -c log.c

menu.o: menu.c header.h
	gcc -c menu.c -lncursesw -lSDL2 -lSDL2_mixer

interruptions.o: interruptions.c header.h
	gcc -c interruptions.c -lncursesw -lSDL2 -lSDL2_mixer

clean: 
	rm *.o
