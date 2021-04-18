#include <stdio.h>
#include "Game.h"

#define SIZE 300

game_t *loadGame(const char *filename) {
    FILE * entrada = fopen(filename, "r+");
    char str[SIZE];
    int procesos, col, row;
    fscanf(entrada,"%d %d %d %[^\0]", procesos, row, col, str);
    game_t jueguito;
    board_init(&jueguito.tablero, col, row);
    board_load(&jueguito.tablero, str);
    fclose(entrada);
}

void writeBoard(board_t board, const char *filename) {
    char str[300];
    FILE * salida = fopen(salida,"w+");
    board_show(board,str);
    fprintf(salida,"%s", str);
    fclose(salida);
}

board_t *conwayGoL(board_t *board, unsigned int cycles, const int nuproc) {
    board_t * nuevoTablero;
    for(int i=0;i < cycles; i++) {
        
    }
    return nuevoTablero;
}

int main() {
    
    return 0;
}