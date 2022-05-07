// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include "reader.h"

void** create_matrix_value(size_t row_count,
            size_t col_count, size_t element_size);
void free_matrix_value(const size_t row_count, void** matrix);

game_t* read_matrix(FILE* fptr) {
    game_t* game = malloc(sizeof(game_t));
    int program_length = 3;
    int counter = 0;
    while (counter <= program_length) {
      if (counter == 0) {
        fscanf(fptr, "%lu", &game->id);  // Reads and saves game ID
        printf("DEBUG: Game id is %lu \n", game->id);
      } else if (counter == 1) {
        fscanf(fptr, "%i", &game->remaining_depth);  // Saves game depth
        printf("DEBUG: Game depth is %i \n", game->remaining_depth);
      } else if (counter == 2) {
        fscanf(fptr, "%i", &game->gamezone_num_rows);  // Saves game depth
        printf("DEBUG: Game rows are %i \n", game->gamezone_num_rows);
        program_length = 5 + game->gamezone_num_rows;  // Re-compute length
      } else if (counter == 3) {
        fscanf(fptr, "%i", &game->gamezone_num_cols);  // Saves game depth
        printf("DEBUG: Game cols are %i \n", game->gamezone_num_cols);
      } else if (counter == 4) {
          printf("DEBUG: Gamezone below \n");
          game->gamezone = (char**)create_matrix_value(game->gamezone_num_rows,
                        game->gamezone_num_cols + 1, sizeof(char));
          for (int i = 0; i <= game->gamezone_num_rows - 1; i++) {
            fscanf(fptr, "%s", game->gamezone[i]);  // Saves the gamezone
            printf("DEBUG: %s \n", game->gamezone[i]);
            counter++;
          }
      } else if (counter == 5 + game->gamezone_num_rows) {
        fscanf(fptr, "%i", &game->num_figures);  // Saves num figures
        printf("DEBUG: Game num figures is %i \n", game->num_figures);
        program_length = program_length + game->num_figures + 1;
      } else if (counter == 6 + game->gamezone_num_rows) {
        game->figures = (char**)create_matrix_value(game->num_figures,
                        1, sizeof(char));
        for (int i = 0; i < game->num_figures; i++) {
          fscanf(fptr, "%s", game->figures[i]);  // Saves num figures
          printf("DEBUG: Figure %i is %s \n", i, game->figures[i]);
          counter++;
        }
      }
      counter++;
    }
    return game;
}

void destroy_matrix(game_t* game) {
    // Release memory used by gamezone and figures
    free_matrix_value(game->gamezone_num_rows, (void**)game->gamezone);
    free_matrix_value(game->num_figures, (void**)game->figures);
    free(game);  // Release game record
    printf("DEBUG: Matrices destroyed \n");
}

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

void free_matrix_value(const size_t row_count, void** matrix) {
  if (matrix) {
    for (size_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}
