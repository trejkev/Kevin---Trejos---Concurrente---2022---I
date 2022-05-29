Tetris concurrent solver - Non optimized

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
  6. Check files into /test for the txt files of the best game.


Results:

A total of 60 trials were executed, using the first 6 levels of depth, and computing the time spent when the threads amount is varied from 1 (serial case) to 10 (maximum threads that can be used by the script), with only Visual Studio running. The results obtained are marvellous and will be explained below.

1. With increased amount of threads NOT necessarily the elapsed time enhances. In this steady state was because from 5 to 9, using block mapping, there is always at least one thread executing 2 columns, instead of 1 as the rest of the threads, reason why this thread leads the time that the program will late to complete all the tasks, this can be seen in the following figure, which shows the results for depth 5 (to make more evident the behavior), but varying the amount of threads.

![Depth 5 data for all threads](https://user-images.githubusercontent.com/18760154/170848742-89b63d5a-1d57-4448-b6b1-be17762f7e5e.png)

To show that this behavior keeps up for all the different depths, see the following figure. Here the effect is shadowed (and not evident for depth 0 because of its low time spent by each thread) by the evidence that every depth gives an offset to the elapsed time, where basically the higher the depth, the higher the elapsed time.

![Depth data for all threads](https://user-images.githubusercontent.com/18760154/170848764-ccff2d09-98f0-407d-9366-df86c43b3b11.png)
