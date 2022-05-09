// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code
#include <stdbool.h>
#include "reader.h"
#include "tetris_figure_factory.h"

int find_best_score(game_t* matrix, int current_level, game_t** best_game, 
    int best_score);
game_t* game_cloner(game_t* original_game);
// void figure_allocator(int row_count, int col_count, char* letter,
//     char** gamezone, int position, int rotation);
void figure_allocator(game_t* matrix, char* letter, int x_position,
    int rotation);
int score_calculator(char** gamezone);


////////////////////////////////////////////////////////////////////////////////
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

    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    // Start at level 0
    int best_score = matrix->gamezone_num_rows;  // Worst score as init
    int score = 500;
    char* figure = matrix->figures[0];
    // Level 0
    score = find_best_score(matrix, 0, (*bg), best_score);
    if (score < best_score) {
        best_score = score;
        printf("DEBUG: Best score is %i \n", best_score);
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

////////////////////////////////////////////////////////////////////////////////
int find_best_score(game_t* matrix, int current_level, game_t** best_game, 
    int best_score) {
    int score = 0;
    if (current_level == matrix->depth + 1) {
        score = score_calculator(matrix->gamezone);
    } else {
        printf("DEBUG: Current level is %i \n", current_level);
        for (int i = 0; i < matrix->gamezone_num_cols; i++) {
            printf("Actual column is %i\n", i);
            char* figure = matrix->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure[0]);
            for (int j = 0; j < num_rotations; j++) {
                printf("Actual rotation is %i\n", j);
                figure_allocator(matrix, figure, i, j);
                int score = find_best_score(matrix, current_level+1,
                    (*best_game), best_score);
                if (score < best_score) {
                    printf("New best score is %i \n", best_score);
                }
            }
        }
    }
    // exit(0);
    return 11;
}

////////////////////////////////////////////////////////////////////////////////
void figure_allocator(game_t* matrix, char* letter, int x_position,
    int rotation) {
    int max_rotations = get_tetris_figure_num_rotations(letter[0]);
    figure_t* fut = get_tetris_figure(letter[0], rotation);
    char* game_intersection[fut->width];
    int starting_row = 0;
    bool abbort = false;
    // Algorithm shall find a place where figure values over 0 are not filled
    // While figure into test boundaries
    while (starting_row + fut->height <= matrix->gamezone_num_rows &&
        x_position + fut->width <= matrix->gamezone_num_cols &&
        abbort == false) {
        // check matching between non zeros from figure with zeros in gamezone
        // if non-0 from figure match with zeros from gamezone keep getting down
        // if last condition fails rewind to last condition and place the figure
        int q = 0;
        // while (q < fut->width && abbort == false) {
        while (q < fut->width && abbort == false) {
            int p = 0;
            // while (p < fut->height && abbort == false) {
            while (p < fut->height && abbort == false) {
                char figure_value = fut->value[p][q];
                char game_val = matrix->gamezone[starting_row+p][x_position+q];
                if (figure_value != '0' && game_val == '0') {
                    abbort = false;
                // Collision condition, thus place the unit
                } else if (figure_value != '0' && game_val != '0') {  
                    printf("Starting row is %i: ", starting_row);
                    // Place the figure in the row before
                    printf("Can place the figure in row %i, column %i. ",
                        starting_row + fut->height - 2, x_position);
                    printf("Figure is %c, rotation is %i\n",
                        letter[0], rotation);
                    abbort = true;
                }
                p++;
            }
            q++;
        }
        starting_row++;
    }
    // Position empty to the bottom, never reached a collision condition
    if (abbort == false && 
        x_position + fut->width <= matrix->gamezone_num_cols) {
        printf("No collision happened\n");
        printf("Starting row is %i: ", starting_row);
        // Place the figure in the row before
        printf("Can place the figure in row %i, column %i. ",
            starting_row + fut->height - 1, x_position);
        printf("Figure is %c, rotation is %i\n",
            letter[0], rotation);

    }
}

int score_calculator(char** gamezone) {
    return 10;
}

////////////////////////////////////////////////////////////////////////////////
game_t* game_cloner(game_t* original_game) {
    game_t* cloned_game = (game_t*)calloc(1, sizeof(game_t));
    cloned_game->id = original_game->id;
    cloned_game->depth = original_game->depth;
    cloned_game->gamezone_num_rows = original_game->gamezone_num_rows;
    cloned_game->gamezone_num_cols = original_game->gamezone_num_cols;
    cloned_game->gamezone = original_game->gamezone;
    cloned_game->num_figures = original_game->num_figures;
    cloned_game->figures = original_game->figures;
    return cloned_game;
}

////////////////////////////////////////////////////////////////////////////////
// Taken from https://www.techiedelight.com/implement-substr-function-c
char* substr(const char *src, int m, int n) {
    // get the length of the destination string
    int len = n - m;
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    // start with m'th char and copy `len` chars into the destination
    strncpy(dest, (src + m), len);
    // return the destination string
    return dest;
}
