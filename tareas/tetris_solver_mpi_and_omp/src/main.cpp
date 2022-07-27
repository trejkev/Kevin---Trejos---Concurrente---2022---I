// Copyright 2022 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Reading script based in Jose Andres Mena <jose.menaarias@ucr.ac.cr> code

#include <mpi.h>
#include <errno.h>
#include <pthread.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include "omp_solver_methods.h"


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** args) {
    // -- set the amount of processes to be used
    size_t processes_qty = 1;

    // -- Set the amount of threads to be used
    size_t thread_qty = 1;
    if (argc >= 2) {
        sscanf(args[1], "%zu", &thread_qty);
    }
    printf("DEBUG: Threads qty is %zu\n", thread_qty);

    // -- Read the basegame file
    FILE* fptr = stdin;
    fptr = fopen("test/Input_Tetris.txt", "r");  // Open the file in read mode
    // -- Check if  file opened correctly
    if (!fptr) {
        fprintf(stderr, "Invalid file \n");
        return EXIT_FAILURE;
    } else {
        printf("DEBUG: File test/Input_Tetris.txt opened \n");
    }
    game_t *matrix = read_matrix(fptr);  // Saves the document into record
    // -- Check if matrix creation was successful
    if (!matrix) {
        fprintf(stderr, "Invalid file content.\n");
        return EXIT_FAILURE;
    }

    // -- Redefine max threads if greater than maximum needed threads
    if (thread_qty > static_cast<size_t>(matrix->gamezone_num_cols)) {
        thread_qty = matrix->gamezone_num_cols;
        printf("DEBUG: Thread reduced to its max value: %zu\n", thread_qty);
    }

    printf("\n\n");  // Prints slenderization

    // -- Shared data keeps best games for each thread
    game_t* best_game[thread_qty*(matrix->depth + 1) + matrix->depth];
    for (size_t i = 0;
        i < (thread_qty*(matrix->depth + 1) + matrix->depth); i++) {
        best_game[i] = reinterpret_cast<game_t*>(calloc(1, sizeof(game_t)));
        best_game[i]->gamezone =
            reinterpret_cast<char**>
            (create_matrix_value(matrix->gamezone_num_rows,
            matrix->gamezone_num_cols + 1, sizeof(char)));
        best_game[i]->figures =
            reinterpret_cast<char*>(calloc(matrix->num_figures, sizeof(char)));
    }
    shared_data_t* shared_data =
        reinterpret_cast<shared_data_t*>(calloc(1, sizeof(shared_data_t)));
    shared_data->bg_matrix = best_game;

    double start_time = omp_get_wtime();

    // -- Concurrence begins here
    if (MPI_Init(&argc, &args) == MPI_SUCCESS) {
        int rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int process_count = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &process_count);
        // -- Private data for each thread out of parallel section
        private_data_t* all_private_data =
            reinterpret_cast<private_data_t*>(calloc(
            1, sizeof(private_data_t)));
        if (rank == 0) {
            std::cout << "Amount of processes is " << process_count << "\n";
        }
        // int* BSPerProcess = (int)calloc(process_count, sizeof(int));
        run_processes(rank, matrix, shared_data, thread_qty, process_count,
            all_private_data);
        MPI_Barrier(MPI_COMM_WORLD);

        double finish_time = omp_get_wtime();

        double elapsed = finish_time - start_time;
        int message = 900;
        if (rank == 0) {
            printf("DEBUG: Elapsed time is: %f s\n", elapsed);
        }
        int BSRank = 0;
        if (process_count > 1) {
            if (rank == 0) {
                for (int iRank = 1; iRank < process_count; ++iRank) {
                    MPI_Send(&message, 1, MPI_INT, iRank, 0, MPI_COMM_WORLD);
                    MPI_Recv(&message, 1, MPI_INT, iRank, 0, MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE);
                    std::cout << "Best score from " << iRank << " is " <<
                        message << " end \n";
                    if (message < all_private_data->best_score) {
                        BSRank = iRank;
                    }
                }
            } else {
                MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                MPI_Send(&all_private_data->best_score , 1, MPI_INT, 0, 0,
                    MPI_COMM_WORLD);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            std::cout << "Best score rank is " << BSRank << "\n";
            // -- Save times from this scenario
            FILE *fptr2;
            fptr2 = fopen("test/times_depth.csv", "a");
            fprintf(fptr2, "%zu;%i;%.9lf;\n", thread_qty, matrix->depth,
                elapsed);
            fclose(fptr2);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == BSRank) {
            printf("DEBUG: Best score is %i\n", all_private_data->best_score);

            // -- Print the basegame
            printf("\n\nDEBUG: Basegame\n");
            for (int row = 0; row < 10; row++) {
                printf("DEBUG: %i  %s\n", row, matrix->gamezone[row]);
            }
            for (int row = 10; row < matrix->gamezone_num_rows; row++) {
                printf("DEBUG: %i %s\n", row, matrix->gamezone[row]);
            }
            printf("\n\n");

            // -- Print the best games
            for (int depth = 0; depth <= matrix->depth; depth++) {
                printf("DEBUG: Best game level %i\n", depth);
                for (int row = 0; row < 10; row++) {
                    printf("DEBUG: %i  %s\n", row, shared_data->
                        bg_matrix[all_private_data->thread_num*(matrix->depth+1)
                            + depth]->gamezone[row]);
                }
                for (int row = 10; row < matrix->gamezone_num_rows; row++) {
                    printf("DEBUG: %i %s\n", row, shared_data->
                        bg_matrix[all_private_data->thread_num*(matrix->depth+1)
                            + depth]->gamezone[row]);
                }
                printf("\n\n");
            }

            // -- Saves the best games into files
            for (int level = 0; level <= matrix->depth; level++) {
                char game_result_path[24];
                snprintf(game_result_path, sizeof(game_result_path), "%s%d%s",
                    "test/tetris_play_", level, ".txt");
                FILE *fptr3;
                fptr3 = fopen(game_result_path, "w");
                // Check if  file opened correctly
                if (!fptr3) {
                    fprintf(stderr, "DEBUG: Invalid file \n");
                    return EXIT_FAILURE;
                } else {
                    printf("DEBUG: File %s opened\n", game_result_path);
                }
                printf("DEBUG: Saving game %i\n", level);
                int offset = (matrix->depth + 1);
                write_bestgame(fptr3, shared_data->bg_matrix[offset + level]);
                fclose(fptr3);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);


        // -- Destroy shared data
        for (size_t i = 0;
            i < (thread_qty*(matrix->depth + 1) + matrix->depth); i++) {
            destroy_matrix(best_game[i], matrix->gamezone_num_rows);
        }
        free(shared_data);
        printf("\n\nDEBUG: Destroyed shared data rank %i\n", rank);

        // -- Destroy private data out of parallel section
        free(all_private_data);
        printf("DEBUG: Destroyed private data out of parallel section rank %i\n"
            , rank);

        // -- Destroy initial game
        destroy_matrix(matrix, matrix->gamezone_num_rows);
        printf("DEBUG: Destroyed initial game state rank %i\n", rank);

        MPI_Finalize();
    }

    return EXIT_SUCCESS;
}
