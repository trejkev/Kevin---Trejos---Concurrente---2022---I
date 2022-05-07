// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code
#include "reader.h"
#include "tetris_figure_factory.h"

int find_best_score(game_t* matrix, int current_level, int hor_position, 
    int rotation, game_t** best_game, int best_score);

int main() {
    FILE* fptr = stdin;

    fptr = fopen("src/Input_Tetris.txt", "r");  // Open the file in read mode

    // -- Check if  file opened correctly
    if (!fptr) {
        fprintf(stderr, "Invalid file \n");
        return EXIT_FAILURE;
    } else {
        printf("DEBUG: File opened \n");
    }

    game_t *matrix = read_matrix(fptr);  // Saves the document into record
    
    game_t* best_game[matrix->depth + 1];
    // Best game memory allocated
    for (int i = 0; i <= matrix->depth; i++) {
        best_game[i] = (game_t*)calloc(1, sizeof(game_t));
    }
    game_t *(*bg)[] = &best_game;  // Pointer to best_game init position
    // Fill out of best game matrices to give them shape
    // for(int i = 0; i<= matrix->depth;i++) {
    //     (*bg)[i]->id = matrix->id;
    //     (*bg)[i]->depth = matrix->depth;
    //     (*bg)[i]->gamezone_num_rows = matrix->gamezone_num_rows;
    //     (*bg)[i]->gamezone_num_cols = matrix->gamezone_num_cols;
    //     (*bg)[i]->gamezone = matrix->gamezone;
    //     (*bg)[i]->num_figures = matrix->num_figures;
    //     (*bg)[i]->figures = matrix->figures;
    //     // printf("Gamezone line 12 is %s\n",(*bg)[i]->gamezone[12]);
    //     printf("DEBUG: Filled level %i best game\n", i);
    // }
    // (*bg)[1]->id = 3;
    // (*best_game)->id = 3;
    // printf("Survived game creation\n");
    // printf("(*bg)[1]->id is %zu \n",(*bg)[1]->id);

    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    // Start at  level 0
    int best_score = matrix->gamezone_num_rows;  // Worst score as init
    int score = 500;
    for (int i = 0; i < matrix->gamezone_num_cols; i++) {
        char* figure = matrix->figures[0];
        int num_rotations = get_tetris_figure_num_rotations(figure[0]);
        for(int j = 0; j < num_rotations; j++) {
            score = find_best_score(matrix,0,i,j,best_game,best_score);
        }
        if (score < best_score) {
            best_score = score;
            printf("DEBUG: Best score is %i \n", best_score);
        }
    }

    //Destroy best games
    for (int i = 0; i <= matrix->depth; i++) {
        printf("DEBUG: Destroying game %i\n", i);
        destroy_matrix((*bg)[i]);
    }
    printf("DEBUG: Destroyed best games\n");

    // Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");

    return EXIT_SUCCESS;
}

int find_best_score(game_t* matrix, int current_level, int hor_position, 
    int rotation, game_t** best_game, int best_score) {
    printf("DEBUG: Current level is %i \n", current_level);
    return 11;
    }
