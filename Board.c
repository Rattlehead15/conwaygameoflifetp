#include "Board.h"
#include <string.h>
#include <stdio.h>

int board_init(board_t *board, size_t col, size_t row) {
    board->cols = col;
    board->rows = row;
    board->cells = malloc(sizeof(char*) * col);
    for (int i=0; i<col; i++)
        board->cells[i] = malloc(sizeof(char) * row);
    return 0;
}

int loop_around(int index, int size) {
    int mod = index % size;
    return mod >= 0 ? mod : (size + mod);
}

char board_get_round(board_t board, int col, int row) {
    return board.cells[loop_around(col, board.cols)][loop_around(row, board.rows)];
}

int board_set(board_t board, int col, int row, char val) {
    return board.cells[loop_around(col, board.cols)][loop_around(row, board.rows)] = val;
}

int board_load(board_t *board, char *str) {
    int amount, largo;
    char letter, aux[10];
    int x = 0, y = 0;
    while(sscanf(str, " %d%c", &amount, &letter) == 2) {
        // char aliveOrDead = (letter == 'X') << 1;
        for(int i = 0; i < amount; i++) {
            board_set(*board, x, y + i, letter/*aliveOrDead*/);
        }
        
        sprintf(aux,"%d", amount);
        str += strlen(aux)+2;
        y += amount;
        x += y / board->rows;
        y = y % board->rows;
    }
    return 0;
}

void board_show(board_t board, char *res) {
    for(int row = 0; row < board.rows; row++) {
        for(int col = 0; col < board.cols; col++) {
            res[row * (board.cols + 1) + col] = board.cells[row][col]; //? 'X' : 'O';
        }
        res[row * (board.cols + 1) + board.cols] = '\n';
    }
    res[board.rows * (board.cols + 1)] = '\0';
}

void board_destroy(board_t *board) {
    for(int i=0; i < board->cols; i++) {
        free(board->cells[i]);
    }
    free(board->cells);
}