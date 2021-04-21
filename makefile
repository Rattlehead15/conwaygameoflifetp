FLAGS = -g

all: main.exe

clean:
	rm *.o *.exe *.out

main.exe: Game.o Board.o
	gcc-9 $(FLAGS) -o main.exe main.c Game.o Board.o -lm -lpthread

Game.o: Game.c Game.h
	gcc-9 -c $(FLAGS) Game.c

Board.o: Board.c Board.h
	gcc-9 -c $(FLAGS) Board.c