# Tetris concurrent solver using OpenMP and MPI

Description: These scripts are intended to solve the issue of finding the best possible game when playing tetris. It takes an initial state,
a certain amount of figures, and places one by one,  when reached the amount of figures desired to place, a metric is computed, which basically
counts the amount of empty cells, and computes the metric as score = max_height - y_position_first_non-empty_cell. This code is based on the work performed for tetris_solver_omp, it uses OpenMP to concurrently execute depth 1, while executing with MPI depth 0, see the following link to check the work done for omp: https://github.com/trejkev/Kevin-Trejos-Concurrente-2022-I/tree/main/tareas/tetris_solver_omp

The whole description of the problem to solve can be found on these repositories, the first for serial solution, and the second for concurrent solution using pthread, and the third for concurrent solution using OpenMP:
1. Serial solution: https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/01
2. Concurrent solution using pthreads: https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/02
3. Concurrent solution using OpenMP: https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/03
4. Hybrid solution using MPI and OpenMP: https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/04

Results analysis of the comparison can be found in this link: https://github.com/trejkev/Kevin-Trejos-Concurrente-2022-I/tree/main/tareas/tetris_solver_omp/report

Now that we are using a hybrid methodology, depth 0 is executed by processes using MPI, while depth 1 is using OpenMP, both with block mapping.

## Running the code:

  1. Make sure you are using a computer with the following libraries and modules up and working:

        a. stdbool.h
        b. stdio.h
        c. stdlib.h
        d. stdint.h
        e. string.h
        f. Valgrind
        g. C compiler (build-essential)
        h. C linter (cpplint)
        i. C make
        j. libomp-dev
  2. Open a terminal pointing to where you'd like to have this project. Then clone the repo there (git clone command).
  3. Execute make command
  4. Run mpirun -np {processes quantity} ./bin/tetris_solver_mpi_and_omp {threads quantity}
  5. Wait for the program to end.
  6. Check files into /test for the txt files of the best game.
