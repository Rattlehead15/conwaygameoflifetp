#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include "game.h"
#include "Board.h"

int main() {
    game_t *juego = loadGame("Ejemplo.game");
    conwayGoL(&juego->tablero, juego->ciclos, 10);
    writeBoard(juego->tablero, "Salida.out");
    board_destroy(&juego->tablero);
    free(juego);
    return 0;
}
