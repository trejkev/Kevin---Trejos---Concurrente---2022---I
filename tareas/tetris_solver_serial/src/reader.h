// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
    int num_rows;
    int num_cols;
    char** value;
} matrix_t;

matrix_t* read_matrix(FILE* fptr);
void destroy_matrix(matrix_t* matrix);

#endif