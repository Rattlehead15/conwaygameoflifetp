#include <stdio.h>
#include <sys/sysinfo.h>
#include "game.h"
#include "Board.h"

int main() {
    game_t *juego = loadGame("Ejemplo.game");
    conwayGoL(&juego->tablero, juego->ciclos, 10);
    writeBoard(juego->tablero, "Salidarda.out");
    board_destroy(&juego->tablero);
    return 0;
}
