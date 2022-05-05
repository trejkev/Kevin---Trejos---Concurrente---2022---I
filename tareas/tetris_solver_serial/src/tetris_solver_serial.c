#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetris_figure_factory.h"                                              // Library made by Jose Andres Mena <jose.menaarias@ucr.ac.cr>

int main(int argc, char** args){
    FILE* ptr;

    ptr = fopen("test.txt", "r");                                               // Open the tetris input file in read mode

    if (NULL == ptr) {
        printf("DEBUG: File can't be opened \n");
    } else {
        printf("DEBUG: File opened \n")
    }
    
    return EXIT_SUCCESS;
}