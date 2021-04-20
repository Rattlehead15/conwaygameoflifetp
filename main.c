#include <stdio.h>
#include "game.h"
#include "Board.h"

int main() {
    game_t *juego = loadGame("Ejemplo.game");
    conwayGoL(&juego->tablero, juego->ciclos, 4);
    writeBoard(juego->tablero, "Salidarda.out");
    board_destroy(&juego->tablero);
    return 0;
}