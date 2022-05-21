// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "serial_solver_methods.h"

////////////////////////////////////////////////////////////////////////////////
int find_best_score(game_t* matrix, int current_level, game_t** best_game,
    int* best_score, bool* save_bg) {
    int score = 0; // worst score is zero, all columns filled

    // Getting memory space for the clone
    game_t* clone = calloc(1, sizeof(game_t)); 
    clone->gamezone = (char**)create_matrix_value(matrix->gamezone_num_rows,
        matrix->gamezone_num_cols + 1, sizeof(char));
    clone->figures = (char**)create_matrix_value(matrix->num_figures, 
        1, sizeof(char)); 

    // Cloning the values
    clone->id = matrix->id;
    clone->depth = matrix->depth;
    clone->gamezone_num_rows = matrix->gamezone_num_rows;
    clone->gamezone_num_cols = matrix->gamezone_num_cols;
    for (int i = 0; i < clone->gamezone_num_rows; i++) {
        for (int k = 0; k < clone->gamezone_num_cols; k++) {
            clone->gamezone[i][k] = matrix->gamezone[i][k];
        }
    }
    clone->num_figures = matrix->num_figures;
    for (int i = 0; i < clone->num_figures; i++) {
        // *clone->figures[i] = *matrix->figures[i];
        *clone->figures[i] = *matrix->figures[i];
    }

    /* Last level + 1, it will realize it reached deepest position requested,
     * thus, it will compute the metric, and if greater than last best game
     * (defaulted to max value of the metric), it will replace the best score
     * and spread a directive to save the last best game found.
    */
    if (current_level == matrix->depth + 1) {
        score = score_calculator(clone);
        if (score < *best_score) {
            for(int level = 0; level <= clone->depth; level++) {
                save_bg[level] = true;
            }
            *best_score = score;
        } 
    } else {
        printf("DEBUG: Current level is %i \n", current_level);
        for (int col = 0; col < clone->gamezone_num_cols; col++) {
            printf("DEBUG: Actual column is %i\n", col);
            char* figure = clone->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure[0]);
            for (int rot = 0; rot < num_rotations; rot++) {
                printf("DEBUG: Actual rotation is %i\n", rot);
                printf("DEBUG: Figure is %c\n",figure[0]);
                int row = figure_allocator(clone, figure, col, rot);
                score = find_best_score(clone, current_level + 1, best_game, 
                    best_score, save_bg);
                // Save best game from level, as directed from deepest level
                if (save_bg[current_level] == true) {
                    // Save clone from level into best game for this level
                    best_game[current_level]->id = clone->id;
                    best_game[current_level]->depth = clone->depth;
                    best_game[current_level]->gamezone_num_rows = 
                        clone->gamezone_num_rows;
                    best_game[current_level]->gamezone_num_cols =
                        clone->gamezone_num_cols;
                    for (int row = 0; row < clone->gamezone_num_rows; row++) {
                        for (int col = 0; col < clone->gamezone_num_cols; col++) {
                            best_game[current_level]->gamezone[row][col] =
                                clone->gamezone[row][col];
                        }
                    }
                    best_game[current_level]->num_figures = clone->num_figures;
                    for (int fig = 0; fig < clone->num_figures; fig++) {
                        best_game[current_level]->figures[fig] =
                            clone->figures[fig];
                    }
                    save_bg[current_level] = false; // Block the current saving
                    
                    int testInteger;
                    printf("DEBUG: Best game level %i, score %i, save flag is  %i\n", current_level, score, save_bg[current_level]);
                    for(int row = 0; row < 10; row++) {
                        printf("%i  %s\n", row, best_game[current_level]->gamezone[row]);
                    }
                    for(int row = 10; row < matrix->gamezone_num_rows; row++) {
                        printf("%i %s\n", row, best_game[current_level]->gamezone[row]);
                    }
                    printf("\n\n");
                    scanf("%d", &testInteger);
                }
                if (row != -1) {
                    // printf("Actual level is %i. ", current_level);
                    figure_remover(clone, figure, col, row, rot);
                }
            }
        }
        printf("DEBUG: Destroying clone from level %i\n", current_level);
        destroy_matrix(clone);
    }
    // printf("Actual score is %i\n", score);
    // exit(0);
    return score;
    // return *best_score;
}

////////////////////////////////////////////////////////////////////////////////
int figure_allocator(game_t* matrix, char* letter, int x_position,
    int rotation) {
    figure_t* fut = get_tetris_figure(letter[0], rotation);
    int floor_row = -1;
    int starting_row = 0;
    bool place_figure = false;
    /* Algorithm shall find a place where figure values over 0 are not filled
     * While figure into test boundaries
     */
    while (starting_row + fut->height - 1 < matrix->gamezone_num_rows &&
        x_position + fut->width - 1 < matrix->gamezone_num_cols &&
        place_figure == false) {
        /* check matching between non zeros from figure with zeros in gamezone
        if non-0 from figure match with zeros from gamezone keep getting down
        if last condition fails rewind to last condition and place the figure */
        int fig_width = 0;
        while (fig_width < fut->width && place_figure == false) {
            int fig_height = 0;
            while (fig_height < fut->height && place_figure == false) {
                char figure_value = fut->value[fig_height][fig_width];
                // printf("DEBUG: gamezone pos row %i, col %i\n", starting_row + fig_height, x_position + fig_width);
                char game_val = matrix->gamezone[starting_row + fig_height]
                    [x_position + fig_width];
                if (figure_value != '0' && game_val != '0' &&
                    starting_row >= fut->height-1 &&
                    starting_row + fut->height - 2 >= 0 ) {
                    // printf("DEBUG: Starting row is %i: ", starting_row);
                    // Place the figure in the row before
                    printf("Can place the figure in row %i, column %i. ",
                        starting_row + fut->height - 2, x_position);
                    printf("Figure is %c, rotation is %i\n",
                        letter[0], rotation);
                    figure_stamp(matrix, fut, starting_row + fut->height - 2,
                        x_position);
                    floor_row = starting_row + fut->height - 2;
                    place_figure = true;
                }
                fig_height++;
            }
            fig_width++;
        }
        starting_row++;
    }
    starting_row--;
    // Position empty to the bottom, never reached a collision condition
    if (place_figure == false &&
        x_position + fut->width - 1 < matrix->gamezone_num_cols &&
        starting_row + fut->height - 2 < matrix->gamezone_num_rows) {
        printf("DEBUG: No collision happened\n");
        printf("DEBUG: Starting row is %i: ", starting_row);
        // Place the figure in the row before
        printf("Can place the figure in row %i, column %i. ",
            starting_row + fut->height - 1, x_position);
        printf("Figure is %c, rotation is %i\n",
            letter[0], rotation);
        figure_stamp(matrix, fut, starting_row + fut->height - 1,
            x_position);
        floor_row = starting_row + fut->height - 1;
        // exit(0);
    } 
    return floor_row;
}

////////////////////////////////////////////////////////////////////////////////
void figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col) {
// game_t* figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col) {
    for (int q = 0; q < figure->width; q++) {
        for (int p = 0; p < figure->height; p++) {
            if (figure->value[p][q] != '0') {
                printf("DEBUG: Placed figure row %i col %i\n",
                    d_row-figure->height+1+p, l_col+q);
                matrix->gamezone[d_row-figure->height+1+p][l_col+q] =
                    figure->value[p][q];
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        printf("DEBUG: %i  %s\n", i, matrix->gamezone[i]);
    }
    for (int i = 10; i < matrix->gamezone_num_rows; i++) {
        printf("DEBUG: %i %s\n", i, matrix->gamezone[i]);
    }
    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////
int score_calculator(game_t* matrix) {
    int score;
    bool abort = false;
    int row_scan = 0;
    while (row_scan < matrix->gamezone_num_rows && abort == false) {
        int col_scan = 0;
        while (col_scan < matrix->gamezone_num_cols && abort == false) {
            // Looks for last zero per row
            if (matrix->gamezone[row_scan][col_scan] != '0') {
                score = matrix->gamezone_num_rows - row_scan;
                abort = true;
            }
            col_scan++;
        }
        row_scan++;
    }
    return score;
}

////////////////////////////////////////////////////////////////////////////////
void figure_remover(game_t* matrix, char* letter, int x_position, 
    int y_position, int rotation) {
    figure_t* figure = get_tetris_figure(letter[0], rotation);
    printf("DEBUG: Removing figure %c, col %i, row %i, rot %i\n", letter[0],
        x_position, y_position, rotation);
    for (int width = 0; width < figure->width; width++) {
        for (int height = 0; height < figure->height; height++) {
            if (figure->value[height][width] != '0') {
                matrix->gamezone[y_position - figure->height + 1 + height]
                    [x_position + width] = '0';
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        printf("DEBUG: %i  %s\n", i, matrix->gamezone[i]);
    }
    for (int i = 10; i < matrix->gamezone_num_rows; i++) {
        printf("DEBUG: %i %s\n", i, matrix->gamezone[i]);
    }
    printf("\n\n");
}
