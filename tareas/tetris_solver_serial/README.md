Tetris serial solver

Description: These scripts are intended to solve the issue of finding the best possible game when playing tetris. It takes an initial state,
a certain amount of figures, and places one by one,  when reached the amount of figures desired to place, a metric is computed, which basically
counts the amount of empty cells, and computes the metric as score = max_height - y_position_first_non-empty_cell.

The whole description of the problem to solve can be found on this repository: https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/01

Running the code:
  1. Make sure you are using a computer with the following libraries up and working:
        a. stdbool.h
        b. stdio.h
        c. stdlib.h
        d. stdint.h
        e. string.h
  2. Open a terminal pointing to where you'd like to have this project. Then clone the repo there (git clone command).
  3. Execute make command
  4. Run ./bin/tetris_solver_serial
  5. Wait for the program to end.
  6. Check files into /output_states for the txt files of the best game.
