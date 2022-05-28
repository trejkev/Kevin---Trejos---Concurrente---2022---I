// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Base extracted from Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct game {
    uint64_t id;
    int depth;
    int gamezone_num_rows;
    int gamezone_num_cols;
    char** gamezone;
    int num_figures;
    char* figures;
} game_t;

/**
 * @brief Method to read the file with the basegame.
 * @details Given a txt with the basegame, extract the variables and put them into a game_t* record.
 * @param fptr Pointer to the file.
 * @return game_t* Record with the base game.
 *
 */
game_t* read_matrix(FILE* fptr);

/**
 * @brief Method to destroy a game_t* record and its internal heap memory-allocated variables.
 * @details Destroys the game_t* record, but first frees the memory used by the gamezone and the figures.
 * @param game_record Record to be destroyed.
 * @param rows_qty Number of rows that game_record->gamezone contains.
 *
 */
void destroy_matrix(game_t* game_record, int rows_qty);

/**
 * @brief Method to create a matrix of row_count times col_count, each cell of type element_size.
 * @param row_count Number of rows of the matrix.
 * @param col_count Number of columns of the matrix.
 * @param element_size Size of the elements of each cell.
 * @return Memory allocation for the matrix.
 *
 */
void** create_matrix_value(size_t row_count, size_t col_count,
    size_t element_size);

#endif
