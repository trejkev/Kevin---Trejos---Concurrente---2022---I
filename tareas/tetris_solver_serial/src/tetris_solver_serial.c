// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code
#include "reader.h"
#include "tetris_figure_factory.h"
#include <stdbool.h>

int find_best_score(game_t* matrix, int current_level, int hor_position, 
    int rotation, game_t** best_game, int best_score);
game_t* game_cloner(game_t* original_game);
// void figure_allocator(int row_count, int col_count, char* letter, 
//     char** gamezone, int position, int rotation);
void figure_allocator(game_t* matrix, char* letter, int position, int rotation);
int score_calculator(char** gamezone);

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

    // Start at level 0
    int best_score = matrix->gamezone_num_rows;  // Worst score as init
    int score = 500;
    for (int i = 0; i < matrix->gamezone_num_cols; i++) {
        char* figure = matrix->figures[0];
        int num_rotations = get_tetris_figure_num_rotations(figure[0]);
        // Level 0
        for(int j = 0; j < num_rotations; j++) {
            // printf("DEBUG: col %i, figure %c, rotation %i\n",i,figure[0],j);
            score = find_best_score(matrix,0,i,j,(*bg),best_score);
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

    // Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");

    return EXIT_SUCCESS;
}





int find_best_score(game_t* matrix, int current_level, int hor_position, 
    int rotation, game_t** best_game, int best_score) {
    printf("DEBUG: Current level is %i \n", current_level);
    // game_t* clone = game_cloner(matrix);
    int score = 0;
    if(current_level == matrix->depth + 1) {
        score = score_calculator(matrix->gamezone);
    } else {
        for (int i = 0; i < matrix->gamezone_num_cols; i++) {
            char* figure = matrix->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure[0]);
            for(int j = 0; j < num_rotations; j++) {
                // figure_allocator(matrix, i, j, figure[0]);
                // figure_allocator(matrix->gamezone_num_rows,
                //     matrix->gamezone_num_cols, figure[0], matrix->gamezone,
                //     hor_position, rotation); 
                figure_allocator(matrix, figure, hor_position, rotation);
                int score = find_best_score(matrix,current_level+1,i,j,
                    (*best_game),best_score);
                if(score < best_score) {
                    printf("New best score is %i \n", best_score);
                }
            }
        }
    }
    exit(0);
    return 11;
    }







game_t* game_cloner(game_t* original_game) {
    game_t* cloned_game = (game_t*)calloc(1,sizeof(game_t));
    cloned_game->id = original_game->id;
    cloned_game->depth = original_game->depth;
    cloned_game->gamezone_num_rows = original_game->gamezone_num_rows;
    cloned_game->gamezone_num_cols = original_game->gamezone_num_cols;
    cloned_game->gamezone = original_game->gamezone;
    cloned_game->num_figures = original_game->num_figures;
    cloned_game->figures = original_game->figures;
    return cloned_game;
}

// void figure_allocator(game_t* matrix, int column, int rotation, char* letter) {

// void figure_allocator(int row_count, int col_count, char* letter, 
//     char** gamezone, int position, int rotation) {
void figure_allocator(game_t* matrix, char* letter, int position, int rotation) {
    int max_rotations = get_tetris_figure_num_rotations(letter[0]);
    figure_t* fut = get_tetris_figure(letter[0], rotation);
    char* game_intersection[fut->width];
    int starting_row = 0; 
    bool abbort = false;
    // Algorithm shall find whatever place where figure values over 0 are not filled
    while (starting_row + fut->height < matrix->gamezone_num_rows && 
        position + fut->width < matrix->gamezone_num_cols - 1 && 
        abbort == false) {
        // check matching between non zeros from figure with zeros in gamezone
        // if non-zeros from figure match with zeros from gamezone keep getting down
        // if last condition fails rewind to last condition and place the figure
        int q = 0;
        while (q < fut->width && abbort == false) {
            int p = 0;
            while (p < fut->height && abbort == false) {
                // printf("Hor position is %i\n",position);
                char figure_value = fut->value[p][q];
                char gamezone_value = matrix->gamezone[starting_row+p][position+q];
                // printf("figure value is %c, gamezone value is %c, ", 
                    // figure_value, gamezone_value);
                // printf("gamezone row x col is %i x %i ",
                        // p + fut->height,q+position);
                if (figure_value != '0' && gamezone_value == '0') {
                    // printf("All good\n");
                    abbort = false;
                } else {
                    if (starting_row > 0) {  // Is not the first trial placing
                        // Place the figure in the row before
                        // for(int i = 0; i< matrix->gamezone_num_rows; i++) {
                        //     printf("DEBUG: %s\n",matrix->gamezone[i]);
                        // }
                        printf("Can place the figure in line %i, column %i. ", starting_row+fut->height-2, position);
                        printf("Figure is %c, rotation is %i\n", letter[0], rotation);
                        abbort = true;
                    } else {
                        printf("Can't place the figure\n");
                    }
                }
                p++;
            }
            q++;
        }
        starting_row++;
        // while(non-zero from figure match with zeros in gamezone area taken) { // try with one level below
        //     for(int p = 0; p < fut->height; p++) {
        //     }
        // }
    }
    // Prints the figure to be placed
    // for(int p = 0; p < fut->height; p++) {
    //     printf("%s\n",fut->value[p]);
    // }
}

int score_calculator(char** gamezone) {
    return 10;
}


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