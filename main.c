#include <stdio.h>
#include <stdlib.h>
#include "ConwayFunc.h"

int main() {
    //File that contains the initial state of the board
    FILE *initState = fopen(getFileName(), "r");
    gridSize size;
    size.rows = 0;
    size.columns = 0;
    checkFile(initState);
    getSize(initState, &size);

    //Created an empty dynamically allocated 2D array
    char **grid = (char **) malloc(size.rows * sizeof(char *));
    //Filling this array symbols from the file
    initBoard(initState, grid, size);
    //Printing out this array into the console
    printBoard(grid, size);
    //Print menu with commands available for user
    printMenu();
    //Initialize evolution
    evolutionProcess(grid, size);

    for (int i = 0; i < size.rows; i++) {
        free(grid[i]);
    }
    free(grid);
    fclose(initState);
    return 0;
}