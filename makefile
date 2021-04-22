FLAGS = -g

all: main.exe

clean:
	rm *.o *.exe *.out valgrind-out.txt

main.exe: Game.o Board.o
	gcc $(FLAGS) -o main.exe main.c Game.o Board.o -lm -lpthread

Game.o: Game.c Game.h
	gcc -c $(FLAGS) Game.c

Board.o: Board.c Board.h
	gcc -c $(FLAGS) Board.c