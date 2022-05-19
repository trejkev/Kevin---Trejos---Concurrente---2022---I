// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

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
    char** figures;
} game_t;

game_t* read_matrix(FILE* fptr);
void destroy_matrix(game_t* matrix);
void** create_matrix_value(size_t row_count,
            size_t col_count, size_t element_size);

#endif
