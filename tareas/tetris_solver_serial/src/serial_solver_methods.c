// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "serial_solver_methods.h"

////////////////////////////////////////////////////////////////////////////////
int find_best_score(game_t* matrix, int current_level, game_t** best_game,
    int best_score) {
    int score = 0;
    if (current_level == matrix->depth + 1) {
        score = score_calculator(matrix);
    } else {
        printf("DEBUG: Current level is %i \n", current_level);
        for (int i = 0; i < matrix->gamezone_num_cols; i++) {
            printf("DEBUG: Actual column is %i\n", i);
            char* figure = matrix->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure[0]);
            for (int j = 0; j < num_rotations; j++) {
                printf("DEBUG: Actual rotation is %i\n", j);
                figure_allocator(matrix, figure, i, j);
                find_best_score(matrix, current_level+1, best_game, best_score);
                int score = find_best_score(matrix, current_level+1,
                    best_game, best_score);
                if (score < best_score) {
                    printf("DEBUG: New best score is %i \n", best_score);
                }
            }
        }
    }
    printf("Actual score is %i\n", score);
    // exit(0);
    return score;
}

////////////////////////////////////////////////////////////////////////////////
void figure_allocator(game_t* matrix, char* letter, int x_position,
    int rotation) {
    figure_t* fut = get_tetris_figure(letter[0], rotation);
    int starting_row = 0;
    bool abort = false;
    // Algorithm shall find a place where figure values over 0 are not filled
    // While figure into test boundaries
    while (starting_row + fut->height < matrix->gamezone_num_rows &&
        x_position + fut->width < matrix->gamezone_num_cols &&
        abort == false) {
        /* check matching between non zeros from figure with zeros in gamezone
        if non-0 from figure match with zeros from gamezone keep getting down
        if last condition fails rewind to last condition and place the figure */
        int q = 0;
        while (q < fut->width && abort == false) {
            int p = 0;
            while (p < fut->height && abort == false) {
                char figure_value = fut->value[p][q];
                char game_val = matrix->gamezone[starting_row+p][x_position+q];
                if (figure_value != '0' && game_val == '0') {
                    abort = false;
                // Collision condition, thus place the unit
                } else if (figure_value != '0' && game_val != '0' &&
                    starting_row >= fut->height-1 &&
                    starting_row + fut->height - 2 >= 0 ) {
                    printf("Figure value %c, gamezone value %c\n",
                        figure_value, game_val);
                    printf("DEBUG: Starting row is %i: ", starting_row);
                    // Place the figure in the row before
                    printf("Can place the figure in row %i, column %i. ",
                        starting_row + fut->height - 2, x_position);
                    printf("Figure is %c, rotation is %i\n",
                        letter[0], rotation);
                    figure_stamp(matrix, fut, starting_row + fut->height - 2,
                        x_position);

                    abort = true;
                }
                p++;
            }
            q++;
        }
        starting_row++;
    }
    // Position empty to the bottom, never reached a collision condition
    if (abort == false &&
        x_position + fut->width -1 < matrix->gamezone_num_cols) {
        printf("DEBUG: No collision happened\n");
        printf("DEBUG: Starting row is %i: ", starting_row);
        // Place the figure in the row before
        printf("Can place the figure in row %i, column %i. ",
            starting_row + fut->height - 1, x_position);
        printf("Figure is %c, rotation is %i\n",
            letter[0], rotation);
        figure_stamp(matrix, fut, starting_row + fut->height - 1,
            x_position);
    }
}

////////////////////////////////////////////////////////////////////////////////
game_t* figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col) {
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
    return matrix;
}

////////////////////////////////////////////////////////////////////////////////
int score_calculator(game_t* matrix) {
    int score = 0;
    bool abort = false;
    int i = 0;
    while (i < matrix->gamezone_num_rows && abort == false) {
        int a = 0;
        while (a < matrix->gamezone_num_cols && abort == false) {
            if (matrix->gamezone[i][a] != '0') {
                score = matrix->gamezone_num_cols - a;
                abort = true;
            }
            a++;
        }
        i++;
    }
    return score;
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
