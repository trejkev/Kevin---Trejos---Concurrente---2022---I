// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#ifndef T_SERIAL_PG_H
#define T_SERIAL_PG_H

#include <stdbool.h>
#include "reader.h"
#include "tetris_figure_factory.h"


typedef struct shared_data {
    game_t** bg_matrix;
} shared_data_t;

typedef struct private_data {
    shared_data_t* shared_data;
    game_t* basegame;
    int best_score;
    size_t thread_num;
    size_t num_threads;
    bool* save_best_game;
} private_data_t;


/**
 * @brief Method to remove the figure into the gamezone.
 * @details Given a figure, a gamezone, and its upper-left corner positioning, 
 * remove the figure from the gamezone
 * @param matrix Record containing all the relevant information for the game.
 * @param letter Actual figure to be placed.
 * @param x_position Gamezone column (from left to right) to place the figure.
 * @param y_position Gamezone row (upper row) to place the figure.
 * @param rotation Actual rotation of the figure to place it.
 *
 */
void figure_remover(game_t* matrix, char letter, int x_position,
    int y_position, int rotation);

/**
 * @brief Recursive method to get the score for the depth requested.
 * @details Given a basegame and a current level, place the figures in the tetris and calculate the game score.
 * @param matrix Record containing all the relevant information for the game.
 * @param current_level Current level being played.
 * @param data Pointer to private data from current thread.
 * @return Score of the actual path taken.
 *
 */
int find_best_score(game_t* matrix, int current_level, private_data_t* data);

/**
 * @brief Method to check the deepest place in the gamezone that the figure can fit.
 * @details Given a figure and a gamezone, get the lowest position where the figure can fit.
 * @param matrix Record containing all the relevant information for the game.
 * @param letter Actual figure to be placed.
 * @param x_position Gamezone column (from left to right) to place the figure.
 * @param rotation Actual rotation of the figure to place it.
 *
 */
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

/**
 * @brief Method to clone an specific game, values copy instead of address.
 * @details Given a gamezone, clone its values to another game_t* record.
 * @param matrix Record containing all the relevant information for the original game to be cloned.
 * @return Cloned game_t* record.
 *
 */
game_t* game_cloner(game_t* matrix);

/**
 * @brief Method to save the best game for the current level.
 * @param best_game Pointer to records saving the best game for each level.
 * @param clone Record containing all the relevant information for the current level game clone.
 * @param current_level Current level being played.
 * @param save_bg Boolean value to block or allow best_game writing.
 * @param thread_num used to give the offset to matrix initial position, together with depth.
 * @return Score of the actual path taken.
 *
 */
void best_game_saver(game_t** best_game, game_t* clone,
    int current_level, bool* save_bg, size_t thread_num);

/**
 * @brief Method to write the file with the best game for an specific level.
 * @details Given a txt path and a game_t* record, save the data from the record to the txt file.
 * @param file_pointer Pointer to the file.
 * @param best_game Game to be saved into file.
 * @return game_t* Record with the game to be saved.
 *
 */
void write_bestgame(FILE* file_pointer, game_t* best_game);

/**
 * @brief Method to get block start for threads blocks mapping division.
 * @param data Private data record for the thread.
 * @param i Thread number.
 * @param D Quantity of columns in the game.
 * @param w Quantity of threads running.
 * @return initial column to run over.
 *
 */
int block_start(size_t i, int D, size_t w);

/**
 * @brief Method to get block finish for threads blocks mapping division.
 * @param data Private data record for the thread.
 * @param i Thread number.
 * @param D Quantity of columns in the game.
 * @param w Quantity of threads running.
 * @return Last column to run over.
 *
 */
int block_finish(size_t i, int D, size_t w);

/**
 * @brief Method to run the threads for data parallelization.
 * @param data Private data record for the thread.
 *
 */
void* run_threads(void *params);

#endif
