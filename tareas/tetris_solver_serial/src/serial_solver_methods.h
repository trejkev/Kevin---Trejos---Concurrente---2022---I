// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#ifndef T_SERIAL_PG_H
#define T_SERIAL_PG_H

#include <stdbool.h>
#include "reader.h"
#include "tetris_figure_factory.h"


int find_best_score(game_t* matrix, int current_level, game_t** best_game,
    int best_score);
// void figure_allocator(int row_count, int col_count, char* letter,
//     char** gamezone, int position, int rotation);
void figure_allocator(game_t* matrix, char* letter, int x_position,
    int rotation);
int score_calculator(game_t* matrix);
game_t* figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col);

#endif
