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

    // char** gamezone_PG = (char**)calloc(matrix->gamezone_num_rows,
    //     matrix->gamezone_num_cols*sizeof(char));

    printf("DEBUG: Basegame is: \n");
    for (int i = 0; i < 10; i++) {
        printf("DEBUG: %i  %s\n", i, matrix->gamezone[i]);
    }
    for (int i = 10; i < matrix->gamezone_num_rows; i++) {
        printf("DEBUG: %i %s\n", i, matrix->gamezone[i]);
    }
    game_t* best_game[matrix->depth + 1];
    // Best game memory allocated
    for (int i = 0; i <= matrix->depth; i++) {
        best_game[i] = (game_t*)calloc(1, sizeof(game_t));
        best_game[i]->gamezone =
            (char**)create_matrix_value(matrix->gamezone_num_rows,
            matrix->gamezone_num_cols + 1, sizeof(char));
        best_game[i]->figures =
            (char**)create_matrix_value(matrix->num_figures,
            1, sizeof(char));
    }
    game_t *(*bg)[] = &best_game;  // Pointer to best_game init position

    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    // Start at level 0
    int bs = matrix->gamezone_num_rows;  // Worst score as init
    int* best_score = &bs;
    // bool save_bg = false;  // Worst score as init
    // bool* save_best_game = &save_bg;
    bool* save_best_game = (bool*)calloc(matrix->depth, sizeof(bool));
    // int score = 500;
    // Level 0
    find_best_score(matrix, 0, (*bg), best_score, save_best_game);
    printf("DEBUG: Best score is %d\n", *best_score);
    printf("DEBUG: Basegame\n");
    for (int row = 0; row < 10; row++) {
        printf("%i  %s\n", row, matrix->gamezone[row]);
    }
    for (int row = 10; row < matrix->gamezone_num_rows; row++) {
        printf("%i %s\n", row, matrix->gamezone[row]);
    }
    printf("\n\n");
    for (int depth = 0; depth <= matrix->depth; depth++) {
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
    for (int i = 0; i <= matrix->depth; i++) {
        printf("DEBUG: Destroying game %i\n", i);
        destroy_matrix((*bg)[i]);
    }

    // Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");

    return EXIT_SUCCESS;
}
