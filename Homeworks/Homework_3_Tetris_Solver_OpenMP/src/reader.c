// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Base extracted from Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include "reader.h"

void** create_matrix_value(size_t row_count,
    size_t col_count, size_t element_size);
void free_matrix_value(const size_t row_count, void** matrix);

////////////////////////////////////////////////////////////////////////////////
game_t* read_matrix(FILE* fptr) {
    game_t* game = calloc(1, sizeof(game_t));  // Creates initial game
    int program_length = 3;  // Initializes program length
    int counter = 0;
    while (counter < program_length) {
        if (counter == 0) {
            fscanf(fptr, "%lu", &game->id);  // Reads and saves game ID
            counter++;
        }
        if (counter == 1) {
            fscanf(fptr, "%i", &game->depth);  // Saves game depth
            counter++;
        }
        if (counter == 2) {
            fscanf(fptr, "%i", &game->gamezone_num_rows);  // Saves game depth
            program_length = 5 + game->gamezone_num_rows;  // Re-compute length
            counter++;
        }
        if (counter == 3) {
            fscanf(fptr, "%i", &game->gamezone_num_cols);  // Saves game depth
            counter++;
        }
        if (counter == 4) {
            game->gamezone =
                (char**)create_matrix_value(game->gamezone_num_rows,
                game->gamezone_num_cols + 1, sizeof(char));
            for (int i = 0; i < game->gamezone_num_rows; i++) {
                fscanf(fptr, "%s", game->gamezone[i]);  // Saves the gamezone
                counter++;
            }
        }
        if (counter == 4 + game->gamezone_num_rows) {
            fscanf(fptr, "%i", &game->num_figures);  // Saves num figures
            program_length = program_length + game->num_figures;
            counter++;
        }
        if (counter == 5 + game->gamezone_num_rows) {
            game->figures = (char*) calloc(game->num_figures, sizeof(char));
            int breakline_counter = 0;
            for (int i = 0; i < game->num_figures*2; i++) {
                if (i%2 == 0) {
                    char thrasher;
                    fscanf(fptr, "%c", &thrasher);
                    breakline_counter++;
                } else if (i%2 != 0) {
                    fscanf(fptr, "%c", &game->figures[i-breakline_counter]);
                }
                counter++;
            }
        }
    }
    fclose(fptr);  // Closing file
    return game;
    }

////////////////////////////////////////////////////////////////////////////////
void destroy_matrix(game_t* game_record, int rows_qty) {
    // Release memory used by the gamezone
    free_matrix_value(rows_qty, (void**)game_record->gamezone);
    // Release memory used by the figures
    free(game_record->figures);
    // Release record
    free(game_record);
}

////////////////////////////////////////////////////////////////////////////////
void** create_matrix_value(size_t row_count,
    size_t col_count, size_t element_size) {
    void** matrix = (void**) calloc(row_count, sizeof(void*));
    if ( matrix == NULL ) {
        return NULL;
    }
    for (size_t row = 0; row < row_count; ++row) {
        if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
            free_matrix_value(row_count, matrix);
            return NULL;
        }
    }
    return matrix;
}

////////////////////////////////////////////////////////////////////////////////
void free_matrix_value(const size_t row_count, void** matrix) {
    if (matrix) {
        for (size_t row = 0; row < row_count; ++row) {
            free(matrix[row]);
        }
    }
    free(matrix);
}
