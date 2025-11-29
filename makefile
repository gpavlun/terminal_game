# makefile for game
# Version: 1
#

gregquest :  Game2.o
	gcc Game2.o -o gregquest -lm

Game2 :  Game2.c
	gcc -c Game2.c -lm

clean:
	rm -f *.o gregquest
