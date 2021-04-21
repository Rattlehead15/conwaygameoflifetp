#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "Game.h"

#define SIZE 300

struct _cubiertos {
    pthread_mutex_t tenedor;
    pthread_mutex_t cuchillo;
};

typedef struct _cubiertos cubiertos;

struct _iteracion_args {
    board_t *board;
    int left;
    int right;
    int n;
    cubiertos *comensales;
    int n_comensales;
    pthread_barrier_t *barrier;
    int cycles;
};

typedef struct _iteracion_args iteracion_args;

char new_value(board_t board, int col, int row) {
    int is_alive = board.cells[col][row] == 'O';
    int neighbors = 0;
    for(int col_offset = -1; col_offset < 2; col_offset++) {
        for(int row_offset = -1; row_offset < 2; row_offset++) {
            neighbors += board_get_round(board, col + col_offset, row + row_offset) == 'O';
        }
    }
    if (is_alive) {
        neighbors--;
        return (neighbors == 2 || neighbors == 3) ? 'O' : 'X';
    } else {
        return (neighbors == 3) ? 'O' : 'X';
    }
}

void *iteracion(void *args) {
    iteracion_args *data = (iteracion_args*) args;
    for(int i=0; i < data->cycles; i++) {
        board_t *new = malloc(sizeof(board_t));
        board_init(new, data->right - data->left, data->board->rows);
        // printf("THREAD %d TIENE %d COLUMNAS\n", data->n, data->right - data->left);

        int izquierdo = loop_around(data->n - 1, data->n_comensales);
        int derecho = loop_around(data->n + 1, data->n_comensales);

        // Le robo los cubiertos a los de al lado
        pthread_mutex_lock(&data->comensales[izquierdo].cuchillo);
        pthread_mutex_lock(&data->comensales[derecho].tenedor);

        //printf("THREAD %d ROBO CUBIERTOS\n", data->n);

        pthread_barrier_wait(data->barrier);

        //printf("THREAD %d CALCULANDO CICLO %d\n", data->n, i);

        for(int col = data->left; col < data->right; col++) {
            for(int row = 0; row < data->board->rows; row++) {
                new->cells[col - data->left][row] = new_value(*data->board, col, row);
            }
        }

        // Les devuelvo los cubiertos porque ya les leí todo
        pthread_mutex_unlock(&data->comensales[izquierdo].cuchillo);
        pthread_mutex_unlock(&data->comensales[derecho].tenedor);

        //printf("THREAD %d DEJO CUBIERTOS\n", data->n);

        // Agarro mis cubiertos para escribir
        pthread_mutex_lock(&data->comensales[data->n].tenedor);
        pthread_mutex_lock(&data->comensales[data->n].cuchillo);

        //printf("THREAD %d ESCRIBIENDO RESULTADOS DE CICLO %d\n", data->n, i);

        for(int col = data->left; col < data->right; col++) {
            free(data->board->cells[col]);
            data->board->cells[col] = new->cells[col - data->left];
        }

        pthread_mutex_unlock(&data->comensales[data->n].tenedor);
        pthread_mutex_unlock(&data->comensales[data->n].cuchillo);

        //printf("THREAD %d TERMINO CICLO %d\n", data->n, i);

        free(new->cells);
        free(new);

        pthread_barrier_wait(data->barrier);
    }

    free(data);
    pthread_exit(NULL);
}

game_t *loadGame(const char *filename) {
    FILE * entrada = fopen(filename, "r+");
    char str[SIZE], aux[SIZE];
    str[0] = '\0';
    int ciclos, col, row;
    fscanf(entrada," %d %d %d ", &ciclos, &col, &row);
    while (fscanf(entrada,"%[^\n]\n", aux) != EOF) {
        strcat(str, aux);
    }
    str[strlen(str)] = '\0';
    game_t *jueguito = malloc(sizeof(game_t));
    jueguito->ciclos = ciclos;
    board_init(&jueguito->tablero, col, row);
    board_load(&jueguito->tablero, str);
    fclose(entrada);

    return jueguito;
}

void writeBoard(board_t board, const char *filename) {
    char str[SIZE];
    FILE * salida = fopen(filename,"w+");
    board_show(board,str);

    // Convertir str a RLE
    int amount = 0;
    char current = str[0];
    for(int i = 0; str[i]; i++) {
        if(str[i] == current) {
            amount++;
        } else {
            fprintf(salida, "%d%c", amount, current);
            amount = str[i] != '\n';
            if(str[i] == '\n')
                fputc(str[i], salida);
            else
                current = str[i];
        }
    }
    fclose(salida);
}

board_t *conwayGoL(board_t *board, unsigned int cycles, const int nuproc) {
    int cols_por_proceso = board->cols / nuproc;
    int resto = board->cols % nuproc;
    cubiertos *comensales = malloc(sizeof(cubiertos) * nuproc);

    pthread_barrier_t barrier;
    int procesos = cols_por_proceso ? nuproc : board->cols;
    
    for(int i=0; i < procesos; i++) {
        pthread_mutex_init(&comensales[i].tenedor, NULL);
        pthread_mutex_init(&comensales[i].cuchillo, NULL);
    }
    
    pthread_barrier_init(&barrier, NULL, procesos);

    int left_offset = 0;
    pthread_t *threads = malloc(sizeof(pthread_t) * procesos);
    for(int proc = 0; proc < procesos; proc++) {
        iteracion_args *args = malloc(sizeof(iteracion_args));
        args->board = board;
        args->left = left_offset;
        args->right = left_offset + cols_por_proceso;
        if(resto > 0) {
            args->right++;
            resto--;
        }
        args->n = proc;
        args->comensales = comensales;
        args->n_comensales = procesos;
        args->barrier = &barrier;
        args->cycles = cycles;
        left_offset += args->right - args->left;
        pthread_create(&threads[proc], NULL, iteracion, args);
    }
    
    for(int proc = 0; proc < procesos; proc++) {
        pthread_join(threads[proc], NULL);
    }

    free(comensales);
    free(threads);
    return board;
}
