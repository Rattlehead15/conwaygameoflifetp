#include <stdio.h>
#include "game.h"
#include "Board.h"

int main() {
    board_t tablero;
    board_init(&tablero, 3, 3);
    board_load(&tablero, "3X\n3O\n3X\0");
    char coso[300];
    board_show(tablero, coso);
    printf("%s", coso);
    board_destroy(&tablero);
    return 0;
}