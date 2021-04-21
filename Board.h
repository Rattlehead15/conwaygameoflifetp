#ifndef BOARD
#define BOARD

#include <stdlib.h>

/******************************************************************************/
/* Definición de la estructura de datos del tablero */

struct _board {
    char **cells;
    unsigned int cols;
    unsigned int rows;
};
typedef struct _board board_t;
/******************************************************************************/
/******************************************************************************/

/* Funciones sobre el tablero */

/* Creación del tablero */
int board_init(board_t *board, size_t col, size_t row);

/* Leer el tablero en una posición asumiendo que el tablero es 'redondo'.*/
char board_get_round(board_t board, int col, int row);

/* Asignarle el valor nuevo 'val' a la posición (col, row) del tablero*/
int board_set(board_t board, int col, int row, char val);

/* Leer de una lista de caracteres que codifican un tablero en formato RLE e
 * interpretarla como un tablero */
int board_load(board_t *board, char *str);

/* Función para mostrar el tablero */
/* La función 'board_show' asume que hay espacio suficiente en 'res' para alojar el tablero.*/
void board_show(board_t board, char *res);

/* Destroy board */
void board_destroy(board_t *board);

/* Funcion auxiliar */

/* Loopea modulo size */
int loop_around(int index, int size);
#endif
