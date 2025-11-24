# makefile for game
# Version: 1
#

gregquest :  Game2.o
	gcc Game2.o -o gregquest

Game2 :  Game2.c
	gcc -c Game2.c

clean:
	rm -f *.o gregquest
