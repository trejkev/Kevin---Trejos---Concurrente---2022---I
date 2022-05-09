Tetris serial solver

Description: These scripts are intended to solve the issue of finding the best possible game when playing tetris. It takes an initial state,
a certain amount of figures, and places one by one,  when reached the amount of figures desired to place, a metric is computed, which basically
counts the amount of empty cells, and computes the metric as score = max_height - y_position_first_non-empty_cell.
