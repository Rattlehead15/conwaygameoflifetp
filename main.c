#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include "game.h"
#include "Board.h"

int main(int argc, char *argv[]) {

    game_t *juego =  loadGame( argc > 2 ? argv[1] : "Ejemplo.game") ;
    if(juego == -1) {
        printf("Error: No se pudo crear el juego\n");
        return 0;
    }
    
    conwayGoL(&juego->tablero, juego->ciclos, get_nprocs());
    writeBoard(juego->tablero, argc > 3 ? argv[2] : "Salida.out");
    board_destroy(&juego->tablero);
    free(juego);

    return 0;
}
