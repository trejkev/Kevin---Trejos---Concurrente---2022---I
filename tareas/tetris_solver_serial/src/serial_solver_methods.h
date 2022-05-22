// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#ifndef T_SERIAL_PG_H
#define T_SERIAL_PG_H

#include <stdbool.h>
#include "reader.h"
#include "tetris_figure_factory.h"


/**
 * @brief Method to remove the figure into the gamezone
 * @details Given a figure, a gamezone, and its upper-left corner positioning, 
 * remove the figure from the gamezone
 * @param matrix Record containing all the relevant information for the game.
 * @param letter Actual figure to be placed.
 * @param x_position Gamezone column (from left to right) to place the figure.
 * @param y_position Gamezone row (upper row) to place the figure.
 * @param rotation Actual rotation of the figure to place it.
 *
 */
// void figure_remover(game_t* matrix, char* letter, int x_position,
//     int y_position, int rotation);
void figure_remover(game_t* matrix, char letter, int x_position,
    int y_position, int rotation);

/**
 * @brief Recursive method to get the score for the depth requested.
 * @details Given a basegame and a current level, place the figures in the tetris and calculate the game score.
 * @param matrix Record containing all the relevant information for the game.
 * @param current_level Current level being played.
 * @param best_game Pointer to records saving the best game for each level
 * @param best_score Best score recorded in the past.
 * @return Score of the actual path taken.
 *
 */
int find_best_score(game_t* matrix, int current_level, game_t** best_game,
    int* best_score, bool* save_bg);

/**
 * @brief Method to check the deepest place in the gamezone that the figure can fit.
 * @details Given a figure and a gamezone, get the lowest position where the figure can fit.
 * @param matrix Record containing all the relevant information for the game.
 * @param letter Actual figure to be placed.
 * @param x_position Gamezone column (from left to right) to place the figure.
 * @param rotation Actual rotation of the figure to place it.
 *
 */
// int figure_allocator(game_t* matrix, char* letter, int x_position,
//     int rotation);
int figure_allocator(game_t* matrix, char letter, int x_position,
    int rotation);

/**
 * @brief Method to compute the score of the game.
 * @details Given a gamezone, compute the score as gamezone height minus the first position, from top to bottom, where a figure is.
 * @param matrix Record containing all the relevant information for the game.
 * @return Score of the gamezone.
 *
 */
int score_calculator(game_t* matrix);

/**
 * @brief Method to place the figure into the gamezone.
 * @details Given a gamezone and a figure, place the figure into the gamezone.
 * @param matrix Record containing all the relevant information for the game.
 * @param figure Figure to be placed in the gamezone.
 * @param d_row Down side row position to place the figure.
 * @param l_col Left side column position to place the figure.
 * @return Gamezone with the figure in place.
 *
 */
void figure_stamp(game_t* matrix, figure_t* figure, int d_row, int l_col);

#endif
