// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code

#include "serial_solver_methods.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>



game_t** best_games_creator(game_t* matrix);
void* run(void *params);


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** arg) {


    // -- Set the amount of threads to be used
    size_t thread_qty = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 2) {
        if (sscanf(arg[1], "%zu", &thread_qty) != 1 || errno) {
            fprintf(stderr, "Invalid number of threads.\n");
            return EXIT_FAILURE;
        }
    }
    printf("DEBUG: Thread count is %zu\n", thread_qty);


    // -- Read the basegame file
    FILE* fptr = stdin;

    fptr = fopen("test/Input_Tetris.txt", "r");  // Open the file in read mode

    // -- Check if  file opened correctly
    if (!fptr) {
        fprintf(stderr, "Invalid file \n");
        return EXIT_FAILURE;
    } else {
        printf("DEBUG: File opened \n");
    }

    game_t *matrix = read_matrix(fptr);  // Saves the document into record

    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }


    /* Shared data matrix for best games for each thread, but with
     * an specific block of data for each of them.
     */
    
    // shared_data_t* shared_data =
    //     (shared_data_t*)calloc(1, sizeof(shared_data_t));
    // shared_data->bg_matrix = (game_t*)calloc(thread_qty*(matrix->depth + 1), sizeof(game_t));
    // // game_t* best_game_matrix[thread_qty*(matrix->depth + 1)];
    // // Getting memory for the best games for all threads
    // for (size_t thread = 0; thread < thread_qty; thread++) {
    //     for (int depth = 0; depth <= matrix->depth; depth++) {
    //         // best_game_matrix[thread + depth] =
    //         shared_data->bg_matrix[thread + depth] =
    //             (game_t*)calloc(1, sizeof(game_t));
    //         // best_game_matrix[thread + depth]->gamezone = 
    //         shared_data->bg_matrix[thread + depth]->gamezone = 
    //             (char**)create_matrix_value(matrix->gamezone_num_rows,
    //             matrix->gamezone_num_cols + 1, sizeof(char));
    //         // best_game_matrix[thread + depth]->figures = 
    //         shared_data->bg_matrix[thread + depth]->figures =
    //             (char*)calloc(matrix->num_figures, sizeof(char));
    //     }
    // }
    // shared_data->bg_matrix = best_game_matrix;
    // game_t *(*bg)[] = &best_game_matrix;  // Pointer to best game init position

    printf("Best game positions is %li\n", thread_qty*(matrix->depth+1));
    game_t* best_game[thread_qty*(matrix->depth+1)+2];
    // -- Getting memory for best games
    for (size_t i = 0; i < ((matrix->depth+1)*thread_qty+2); i++) {
        // for (size_t thread = 0; thread < thread_qty; thread++) {
        best_game[i] = (game_t*)calloc(1, sizeof(game_t));
        best_game[i]->gamezone =
            (char**)create_matrix_value(matrix->gamezone_num_rows,
            matrix->gamezone_num_cols + 1, sizeof(char));
        best_game[i]->figures =
            (char*)calloc(matrix->num_figures, sizeof(char));
        // }
    }
    shared_data_t* shared_data =
        (shared_data_t*)calloc(1, sizeof(shared_data_t));
    shared_data->bg_matrix =
        (game_t**)calloc(thread_qty*(matrix->depth + 1) + 2, sizeof(game_t));
    shared_data->bg_matrix = best_game;
    // game_t *(*bg)[] = &best_game;  // Pointer to best_game init position


    // int bs = matrix->gamezone_num_rows;  // Best score initialization
    // int* best_score = &bs;


    // -- Creating variables for each thread
    private_data_t* private_data[thread_qty];
    for (size_t thread = 0; thread < thread_qty; thread++) {
        // Allocate space for private data
        private_data[thread] = 
            (private_data_t*)calloc(1, sizeof(private_data_t));
        // Allocate space for best games
        // private_data[thread]->best_games = 
        //     (game_t**)create_matrix_value(matrix->depth+1,
        //     1, sizeof(game_t*));
        // private_data[thread]->best_games = (game_t**)calloc(matrix->depth+1,
        //     sizeof(game_t**));
        // for (int i = 0; i <= matrix->depth; i++) {
        //     private_data[thread]->best_games[i]->gamezone =
        //         (char**)create_matrix_value(matrix->gamezone_num_rows,
        //         matrix->gamezone_num_cols + 1, sizeof(char));
        //     private_data[thread]->best_games[i]->figures =
        //     (char*)calloc(matrix->num_figures, sizeof(char));
        // }
        // private_data[thread]->best_games = (*bg);
        // private_data[thread]->basegame->gamezone =
        //     (char**)create_matrix_value(matrix->gamezone_num_rows,
        //     matrix->gamezone_num_cols, sizeof(char));
        // private_data[thread]->basegame->figures =
        //     (char*)calloc(matrix->num_figures, sizeof(char));
        // private_data[thread]->basegame = (game_t*)calloc(1, sizeof(game_t));
        private_data[thread]->basegame = game_cloner(matrix);
        private_data[thread]->best_score = matrix->gamezone_num_rows;
        private_data[thread]->num_threads = thread_qty;
        private_data[thread]->thread_num = thread;
        printf("My threadnum is %zu\n", private_data[thread]->thread_num);
        private_data[thread]->shared_data = shared_data;
        // -- Variable to keep locking the best game savings per level
        private_data[thread]->save_best_game = 
            (bool*)calloc(matrix->depth + 1, sizeof(bool));
        // private_data[thread]->bg = (game_t**)calloc(matrix->depth + 1, sizeof(game_t*));
        // private_data[thread]->bg = best_games_creator(matrix);
        // printf("I am here\n");
    }


    // -- Concurrence begins here
    pthread_t* threads = calloc(thread_qty, sizeof(pthread_t));
    for (size_t i = 0; i < thread_qty; i++) {
        if (pthread_create(&threads[i], NULL, run,
            (void*)private_data[i]) != EXIT_SUCCESS) {
            fprintf(stderr, "DEBUG: Could not create thread %zu.\n", i);
            return EXIT_FAILURE;
        }
    }

    // -- Wait for all threads to complete
    for (size_t thread = 0; thread < thread_qty; thread++) {
        pthread_join(threads[thread], NULL);
    }

    printf("Joint completed!\n");


    // -- Print the basegame
    printf("DEBUG: Basegame\n");
    for (int row = 0; row < 10; row++) {
        printf("%i  %s\n", row, matrix->gamezone[row]);
    }
    for (int row = 10; row < matrix->gamezone_num_rows; row++) {
        printf("%i %s\n", row, matrix->gamezone[row]);
    }
    printf("\n\n");

    // -- Print the best games
    for (size_t thread = 0; thread < thread_qty; thread++) {
        printf("DEBUG: Best score thread %zu is %d\n", thread, private_data[thread]->best_score);
        for (int depth = 0; depth <= matrix->depth; depth++) {
            printf("DEBUG: Best game thread %zu level %i\n", thread, depth);
            for (int row = 0; row < 10; row++) {
                printf("%i  %s\n", row, shared_data->bg_matrix[thread*(matrix->depth+1) + depth]->gamezone[row]);
            }
            for (int row = 10; row < matrix->gamezone_num_rows; row++) {
                printf("%i %s\n", row, shared_data->bg_matrix[thread*(matrix->depth+1) + depth]->gamezone[row]);
            }
            printf("\n\n");
        }
    }


    // -- Saves the best games into files
    // for (int level = 0; level <= matrix->depth; level++) {
    //     char game_result_path[24];
    //     snprintf(game_result_path, sizeof(game_result_path), "%s%d%s",
    //         "test/tetris_play_", level, ".txt");
    //     FILE *fptr;
    //     fptr = fopen(game_result_path, "w");
    //     // -- Check if  file opened correctly
    //     if (!fptr) {
    //         fprintf(stderr, "DEBUG: Invalid file \n");
    //         return EXIT_FAILURE;
    //     } else {
    //         printf("DEBUG: File opened \n");
    //     }
    //     printf("DEBUG: Saving game %i\n", level);
    //     write_bestgame(fptr, (*bg)[level]);
    //     fclose(fptr);
    // }

    // Destroy private data
    for (size_t thread = 0; thread < thread_qty; thread++) {
        destroy_matrix(private_data[thread]->basegame);
        free(private_data[thread]->save_best_game);
        free(private_data[thread]);
    }

    // -- Destroy shared data
    for (int i = 0; i <= matrix->depth; i++) {
        for (size_t thread = 0; thread < thread_qty; thread++) {
            destroy_matrix(best_game[i+thread]);
            destroy_matrix(shared_data->bg_matrix[i+thread]);
        }
    }
    free(shared_data->bg_matrix);
    free(shared_data);


    // -- Destroy best games
    // for (int i = 0; i <= matrix->depth; i++) {
    //     printf("DEBUG: Destroying best game %i\n", i);
    //     destroy_matrix((*bg)[i]);
    // }

    // -- Destroy best game saving flags
    // free(save_best_game);
    // printf("DEBUG: Destroyed save_best_game flags\n");

    // -- Destroy initial game
    destroy_matrix(matrix);
    printf("DEBUG: Destroyed initial game state\n");


    return EXIT_SUCCESS;
}


void* run(void *params) {
    private_data_t* data = (private_data_t*)params;
    // -- Best game lookup level zero extracted
    int current_level = 0;
    game_t* clone = game_cloner(data->basegame);

    int cols_per_thread = (int)(data->basegame->gamezone_num_cols/data->num_threads);
    int init = data->thread_num*cols_per_thread; 
    int end = init + cols_per_thread;
    if (data->thread_num == data->num_threads - 1) {
        end = data->basegame->gamezone_num_cols; // Ensure all cols are covered
    }
    printf("Cols per thread is %i, init %i, end %i, thread %zu\n", cols_per_thread, init, end, data->thread_num);

    // Blocks mapping for columns
    printf("Threadnum is %zu\n", data->thread_num);
    for (int col = init; col < end; col++) {
    // for (int col = data->thread_num;
    //     col < data->basegame->gamezone_num_cols;
    //     col = col + data->num_threads) {
        printf("I am here for thread %zu, col %i\n", data->thread_num, col);
        char figure = data->basegame->figures[current_level];
        int num_rotations = get_tetris_figure_num_rotations(figure);
        for (int rot = 0; rot < num_rotations; rot++) {
            int row = figure_allocator(clone, figure, col, rot);
            find_best_score(clone, current_level + 1, data);
            if (data->save_best_game[current_level] == true) {
                best_game_saver(data->shared_data->bg_matrix, clone, current_level,
                    data->save_best_game, data->thread_num);
            }
            if (row != -1) {
                figure_remover(data->basegame, figure, col, row, rot);
            }
        }
    }
    destroy_matrix(clone);
    // data->basegame = NULL;
    return NULL;
}







game_t** best_games_creator(game_t* matrix) {
    // game_t* best_game[matrix->depth+1];
    game_t** best_game_matrix =
        (game_t**)calloc(matrix->depth + 1, sizeof(game_t*));

    // Getting memory for the best games for all threads
    for (int depth = 0; depth <= matrix->depth; depth++) {
        best_game_matrix[depth]->gamezone = 
            (char**)create_matrix_value(matrix->gamezone_num_rows,
            matrix->gamezone_num_cols + 1, sizeof(char));
        best_game_matrix[depth]->figures = 
            (char*)calloc(matrix->num_figures, sizeof(char));
    }
    return best_game_matrix;
}