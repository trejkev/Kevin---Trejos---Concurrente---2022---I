// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code

#include "serial_solver_methods.h"


////////////////////////////////////////////////////////////////////////////////
int main() {
    FILE* fptr = stdin;

    fptr = fopen("test/Input_Tetris.txt", "r");  // Open the file in read mode

    // -- Check if  file opened correctly
    if (!fptr) {
        fprintf(stderr, "Invalid file \n");
        return EXIT_FAILURE;
    } else {
        printf("DEBUG: File opened \n");
    }

    game_t *matrix = read_matrix(fptr);  // Saves the document into record

    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    game_t* best_game[matrix->depth];
    // -- Getting memory for best games
    for (int i = 0; i < matrix->depth; i++) {
        best_game[i] = (game_t*)calloc(1, sizeof(game_t));
        best_game[i]->gamezone =
            (char**)create_matrix_value(matrix->gamezone_num_rows,
            matrix->gamezone_num_cols + 1, sizeof(char));
        best_game[i]->figures = (char*) calloc(matrix->num_figures, sizeof(char));
    }
    game_t *(*bg)[] = &best_game;  // Pointer to best_game init position

    int bs = matrix->gamezone_num_rows;  // Best score initialization
    int* best_score = &bs;

    // -- Variable to keep locking the best game savings per level
    bool* save_best_game = (bool*)calloc(matrix->depth, sizeof(bool));

    // Recursive method to get the best game
    find_best_score(matrix, 0, (*bg), best_score, save_best_game);


    // Print the results
    printf("DEBUG: Best score is %d\n", *best_score);
    printf("DEBUG: Basegame\n");
    for (int row = 0; row < 10; row++) {
        printf("%i  %s\n", row, matrix->gamezone[row]);
    }
    for (int row = 10; row < matrix->gamezone_num_rows; row++) {
        printf("%i %s\n", row, matrix->gamezone[row]);
    }
    printf("\n\n");
    for (int depth = 0; depth < matrix->depth; depth++) {
        printf("DEBUG: Best game level %i\n", depth);
        for (int row = 0; row < 10; row++) {
            printf("%i  %s\n", row, (*bg)[depth]->gamezone[row]);
        }
        for (int row = 10; row < matrix->gamezone_num_rows; row++) {
            printf("%i %s\n", row, (*bg)[depth]->gamezone[row]);
        }
        printf("\n\n");
    }

    // Destroy best games
    for (int i = 0; i < matrix->depth; i++) {
        printf("DEBUG: Destroying game %i\n", i);
        destroy_matrix((*bg)[i]);
    }

    // Destroy best game saving flags
    free(save_best_game);
    printf("DEBUG: Destroyed save_best_game flags\n");

    // Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");

    return EXIT_SUCCESS;
}
