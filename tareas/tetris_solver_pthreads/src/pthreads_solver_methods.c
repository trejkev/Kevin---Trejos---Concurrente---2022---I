// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "pthreads_solver_methods.h"

////////////////////////////////////////////////////////////////////////////////
int find_best_score(game_t* matrix, int current_level, private_data_t* data) {
    int score = 0;
    game_t* clone = game_cloner(matrix);

    if (current_level > matrix->depth) {
        score = score_calculator(clone);
        if (score < data->best_score) {
            for (int level = 0; level <= clone->depth; level++) {
                data->save_best_game[level] = true;
            }
            data->best_score = score;
        }
    } else if (current_level <= matrix->depth) {
        for (int col = 0; col < clone->gamezone_num_cols; col++) {
            char figure = clone->figures[current_level];
            int num_rotations = get_tetris_figure_num_rotations(figure);
            for (int rot = 0; rot < num_rotations; rot++) {
                int row = figure_allocator(clone, figure, col, rot);
                find_best_score(clone, current_level + 1, data);
                // Save best game from level, as directed from deepest level
                if (data->save_best_game[current_level] == true) {
                    best_game_saver(data->shared_data->bg_matrix, clone,
                        current_level, data->save_best_game, data->thread_num);
                }
                if (row != -1) {
                    figure_remover(clone, figure, col, row, rot);
                }
            }
        }
    }
    destroy_matrix(clone, clone->gamezone_num_rows);
    return score;
}

////////////////////////////////////////////////////////////////////////////////
int figure_allocator(game_t* matrix, char letter, int x_position,
    int rotation) {
    // figure_t* fut = get_tetris_figure(letter[0], rotation);
    figure_t* fut = get_tetris_figure(letter, rotation);
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
         * if non-0 from figure match with zeros from gamezone keep getting down
         * if last condition fails rewind to last condition and place the figure
         */
        int fig_width = 0;
        while (fig_width < fut->width && place_figure == false) {
            int fig_height = 0;
            while (fig_height < fut->height && place_figure == false) {
                char figure_value = fut->value[fig_height][fig_width];
                char game_val = matrix->gamezone[starting_row + fig_height]
                    [x_position + fig_width];
                // Found condition of figure clash with another figure
                if (figure_value != '0' && game_val != '0' &&
                    starting_row >= fut->height-1 &&
                    starting_row + fut->height - 2 >= 0 ) {
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
    /* Position empty to the bottom, never reached a collision condition.
     * Thus, place the figure at the bottom.
     */
    if (place_figure == false &&
        x_position + fut->width - 1 < matrix->gamezone_num_cols &&
        starting_row + fut->height - 2 < matrix->gamezone_num_rows) {
        figure_stamp(matrix, fut, starting_row + fut->height - 1,
            x_position);
        floor_row = starting_row + fut->height - 1;
    }
    return floor_row;
}

////////////////////////////////////////////////////////////////////////////////
void figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col) {
    for (int q = 0; q < figure->width; q++) {
        for (int p = 0; p < figure->height; p++) {
            if (figure->value[p][q] != '0') {
                matrix->gamezone[d_row-figure->height+1+p][l_col+q] =
                    figure->value[p][q];
            }
        }
    }
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
void figure_remover(game_t* matrix, char letter, int x_position,
    int y_position, int rotation) {
    // figure_t* figure = get_tetris_figure(letter[0], rotation);
    figure_t* figure = get_tetris_figure(letter, rotation);
    for (int width = 0; width < figure->width; width++) {
        for (int height = 0; height < figure->height; height++) {
            if (figure->value[height][width] != '0') {
                matrix->gamezone[y_position - figure->height + 1 + height]
                    [x_position + width] = '0';
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
game_t* game_cloner(game_t* matrix) {
    // Getting memory space for the clone
    game_t* clone = calloc(1, sizeof(game_t));
    clone->gamezone = (char**)create_matrix_value(matrix->gamezone_num_rows,
        matrix->gamezone_num_cols + 1, sizeof(char));
    clone->figures = (char*) calloc(matrix->num_figures, sizeof(char));

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
        clone->figures[i] = matrix->figures[i];
    }

    return clone;
}

////////////////////////////////////////////////////////////////////////////////
void best_game_saver(game_t** best_game, game_t* clone,
    int current_level, bool* save_bg, size_t thread_num) {
    int actual_bg = current_level + thread_num*(clone->depth + 1);
    // Save clone from level into best game for this level
    best_game[actual_bg]->id = clone->id;
    best_game[actual_bg]->depth = clone->depth;
    best_game[actual_bg]->gamezone_num_rows = clone->gamezone_num_rows;
    best_game[actual_bg]->gamezone_num_cols = clone->gamezone_num_cols;
    for (int row = 0; row < clone->gamezone_num_rows; ++row) {
        int gzcols = clone->gamezone_num_cols;
        for (int col = 0; col < gzcols; ++col) {
            best_game[actual_bg]->gamezone[row][col] =
                clone->gamezone[row][col];
        }
    }
    best_game[actual_bg]->num_figures = clone->num_figures;
    for (int fig = 0; fig < clone->num_figures; fig++) {
        best_game[actual_bg]->figures[fig] = clone->figures[fig];
    }
    save_bg[current_level] = false;  // Block the current saving
}

////////////////////////////////////////////////////////////////////////////////
void write_bestgame(FILE* file_pointer, game_t* best_game) {
    fprintf(file_pointer, "%lu\n", best_game->id);
    fprintf(file_pointer, "%i\n", best_game->depth);
    fprintf(file_pointer, "%i\n", best_game->gamezone_num_rows);
    fprintf(file_pointer, "%i\n", best_game->gamezone_num_cols);
    for (int row = 0; row < best_game->gamezone_num_rows; row++) {
        fprintf(file_pointer, "%s\n", best_game->gamezone[row]);
    }
    fprintf(file_pointer, "%i\n", best_game->num_figures);
    for (int fig_num = 0; fig_num < best_game->num_figures; fig_num++) {
        fprintf(file_pointer, "%c\n", best_game->figures[fig_num]);
    }
}

////////////////////////////////////////////////////////////////////////////////
void* run_threads(void *params) {
    private_data_t* data = (private_data_t*)params;
    // -- Best game lookup level zero extracted
    int current_level = 0;
    game_t* clone = game_cloner(data->basegame);

    // -- Blocks mapping computing
    int init = block_start(data->thread_num,
        data->basegame->gamezone_num_cols,
        data->num_threads);
    int end = block_finish(data->thread_num,
        data->basegame->gamezone_num_cols,
        data->num_threads);

    // -- Blocks mapping for columns
    for (int col = init; col < end; col++) {
        char figure = data->basegame->figures[current_level];
        int num_rotations = get_tetris_figure_num_rotations(figure);
        for (int rot = 0; rot < num_rotations; rot++) {
            int row = figure_allocator(clone, figure, col, rot);
            find_best_score(clone, current_level + 1, data);
            if (data->save_best_game[current_level] == true) {
                best_game_saver(data->shared_data->bg_matrix, clone,
                current_level, data->save_best_game, data->thread_num);
            }
            if (row != -1) {
                figure_remover(data->basegame, figure, col, row, rot);
            }
        }
    }
    destroy_matrix(clone, clone->gamezone_num_rows);
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
int block_start(size_t i, int D, size_t w) {
    /* i = Thread number
     * D = Columns
     * w = Threads qty
    */
    int min = i;
    if (i > D%w) {
        min = D%w;
    }
    return i*(D/w) + min;
}

////////////////////////////////////////////////////////////////////////////////
int block_finish(size_t i, int D, size_t w) {
    return block_start(i + 1, D, w);
}
