// // #include "reader.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct matrices {
//     size_t num_rows;
//     char** value;
// } matrix_type;

// //void print_matrix(matrix_t* matrix);

// int main() {
//     FILE* fptr = stdin;
//     size_t read;
//     char* line = NULL;
//     size_t len = 0;
    
//     fptr = fopen("src/Input_Tetris.txt", "r");

//     if (!fptr) {
//         fprintf(stderr, "Invalid file\n");
//         return EXIT_FAILURE;
//     } else {
//         printf("DEBUG: Valid file \n");
//     }

//     matrix_type *matrix1 = malloc(sizeof(matrix_type));
//     matrix1->num_rows = 0;

//     while ((read = getline(&line, &len, fptr)) < 50) {
//         sscanf("line", "%s", matrix1->value[matrix1->num_rows]);
//         // matrix1->value[matrix1->num_rows] = line;
//     //     // strcpy(matrix1->value[matrix1->num_rows], line);
//     //     // sscanf(line, "%s", matrix1->value[matrix1->num_rows]);
//         printf("Retrieved line of length %zu:\n", read);
//         printf("%s", line);
//         printf("counter is %zu\n", matrix1->num_rows);
//         matrix1->num_rows++;

//     }
//     free(matrix1);
//     fclose(fptr);

//     // for (int i = 0; i < matrix->num_rows; ++i) {
//     //     printf("%",matrix->value[i]);
//     //     fscanf(fptr, "%s", matrix->value[i]);
//     // }

//     return EXIT_SUCCESS;
// }







// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>
/*
* Reads a matrix from an input file with the format:
* - Line 1 contains an integer N with the number of rows
* - Line 2 contains an integer M with the number of columns
* - The next N lines contain strings of M characters representing the matrix value
* You can send the input file as argument. Example:
*   $ ./bin/matrix-reader ./test/input00.txt
* Or you can send it using standard input. Example:
*   $ ./bin/matrix-reader < ./test/input00.txt
*/
#include "reader.h"

void print_matrix(matrix_t* matrix);

int main() {
    FILE* fptr = stdin;

    fptr = fopen("src/Input_Tetris.txt", "r");

    if (!fptr) {
        fprintf(stderr, "Invalid file\n");
        return EXIT_FAILURE;
    } else {
        printf("File opened \n");
    }

    matrix_t *matrix = read_matrix(fptr);

    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    print_matrix(matrix);

    destroy_matrix(matrix);

    return EXIT_SUCCESS;
}

void print_matrix(matrix_t* matrix) {
    for (int i = 0; i < matrix->num_rows; ++i) {
        printf("%s\n", matrix->value[i]);
    }
}