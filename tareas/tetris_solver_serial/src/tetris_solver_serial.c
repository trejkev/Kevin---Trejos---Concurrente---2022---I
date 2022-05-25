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
        best_game[i]->figures =
            (char*)calloc(matrix->num_figures, sizeof(char));
    }
    game_t *(*bg)[] = &best_game;  // Pointer to best_game init position

    int bs = matrix->gamezone_num_rows;  // Best score initialization
    int* best_score = &bs;

    // -- Variable to keep locking the best game savings per level
    bool* save_best_game = (bool*)calloc(matrix->depth, sizeof(bool));


    // -- Best game lookup level zero extracted
    int current_level = 0;
    int score = 0;
    game_t* clone = game_cloner(matrix);

    if (current_level == matrix->depth) {
        score = score_calculator(clone);
        if (score < *best_score) {
            for (int level = 0; level < clone->depth; level++) {
                save_best_game[level] = true;
            }
            *best_score = score;
        }
    } else {
        for (int col = 0; col < clone->gamezone_num_cols; col++) {
            char figure = clone->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure);
            for (int rot = 0; rot < num_rotations; rot++) {
                int row = figure_allocator(clone, figure, col, rot);
                score = find_best_score(clone, current_level + 1, (*bg),
                    best_score, save_best_game);
                // Save best game from level, as directed from deepest level
                if (save_best_game[current_level] == true) {
                    best_game_saver((*bg), clone, current_level,
                        save_best_game);
                }
                if (row != -1) {
                    figure_remover(clone, figure, col, row, rot);
                }
            }
        }
    }
    destroy_matrix(clone);


    // -- Print the basegame
    printf("DEBUG: Best score is %d\n", *best_score);
    printf("DEBUG: Basegame\n");
    for (int row = 0; row < 10; row++) {
        printf("%i  %s\n", row, matrix->gamezone[row]);
    }
    for (int row = 10; row < matrix->gamezone_num_rows; row++) {
        printf("%i %s\n", row, matrix->gamezone[row]);
    }
    printf("\n\n");

    // -- Print the best games
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


    // -- Saves the best games into files
    for (int level = 0; level < matrix->depth; level++) {
        char game_result_path[24];
        snprintf(game_result_path, sizeof(game_result_path), "%s%d%s",
            "test/tetris_play_", level, ".txt");
        FILE *fptr;
        fptr = fopen(game_result_path, "w");
        // -- Check if  file opened correctly
        if (!fptr) {
            fprintf(stderr, "Invalid file \n");
            return EXIT_FAILURE;
        } else {
            printf("DEBUG: File opened \n");
        }
        printf("Saving game %i\n", level);
        write_bestgame(fptr, (*bg)[level]);
        fclose(fptr);
    }


    // -- Destroy best games
    for (int i = 0; i < matrix->depth; i++) {
        printf("DEBUG: Destroying best game %i\n", i);
        destroy_matrix((*bg)[i]);
    }
    // -- Destroy best game saving flags
    free(save_best_game);
    printf("DEBUG: Destroyed save_best_game flags\n");

    // -- Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");


    return EXIT_SUCCESS;
}
