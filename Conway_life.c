#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ConwayFunc.h"

/*
 * # - live cell
 * . - empty space
 */

//Checking if the file is empty or exists
void checkFile(FILE *file) {
    if (file == NULL) {
        printf("File doesn't exist");
        exit(0);
    } else {
        int size;
        //Goes to the end of file
        fseek(file, 0, SEEK_END);
        //Gets current file pointer
        size = ftell(file);
        if(size == 0) {
            printf("File is empty");
            exit(-1);
        }
        //Goes back to the beginning of the file
        rewind(file);
    }
}

//Initializing board from the file
void initBoard(FILE *initState, char **grid, gridSize size) {
    for (int i = 0; i < size.rows; i++) {
        grid[i] = (char *) malloc(size.columns * sizeof(char));
    }

    for (int i = 0; i < size.rows; i++) {
        for (int j = 0; j < size.columns; j++) {
            fscanf(initState, "%c ", &grid[i][j]);
        }
    }
}

void printMenu(void) {
    printf("Press ENTER to evolve the grid\n");
    printf("Type /st to stop the program\n");
    printf("Type /sv to save the grid\n");
    printf("Type ?help to list available commands\n");
}

//Function that prints out the board into the console
void printBoard(char **grid, gridSize size) {
    for (int i = 0; i < size.rows; i++) {
        for (int j = 0; j < size.columns; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

//Function that evolves the grid into the nex generation
void evolve(char **grid, gridSize size) {
    for (int i = 0; i < size.rows; i++) {
        for (int j = 0; j < size.columns; j++) {
            if (countLive(grid, i, j, size) == 3) {
                grid[i][j] = '#';
            } else if (countLive(grid, i, j, size) <= 1 || countLive(grid, i, j, size) >= 4) {
                grid[i][j] = '.';
            }
        }
    }
    //Printing the next generation board every time
    printBoard(grid, size);
    countAllLive(grid, size);
}

//Count all the live cells in the grid
void countAllLive(char **grid, gridSize size) {
    int alive = 0;
    for (int i = 0; i < size.rows; i++) {
        for (int j = 0; j < size.columns; j++) {
            if(grid[i][j] == '#') {
                alive++;
            }
        }
    }
    //If there are no live cells, then the program will stop
    if(alive == 0) {
        printf("All cells are dead\n");
        exit(0);
    }
}

//Function that counts how many alive neighbours the alive cell has
int countLive(char **grid, int i, int j, gridSize size) {
    int liveCells = 0;

    if (i + 1 < size.rows) {
        if (grid[i + 1][j] == '#')
            liveCells++;
    }

    if (i - 1 >= 0) {
        if (grid[i - 1][j] == '#') {
            liveCells++;
        }
    }

    if (j + 1 < size.columns) {
        if (grid[i][j + 1] == '#')
            liveCells++;
    }

    if (j - 1 >= 0) {
        if (grid[i][j - 1] == '#')
            liveCells++;
    }

    if (i + 1 < size.rows && j + 1 < size.columns) {
        if (grid[i + 1][j + 1] == '#')
            liveCells++;
    }

    if (i - 1 >= 0 && j - 1 >= 0) {
        if (grid[i - 1][j - 1] == '#')
            liveCells++;
    }

    if (i + 1 < size.rows && j - 1 >= 0) {
        if (grid[i + 1][j - 1] == '#')
            liveCells++;
    }

    if (i - 1 >= 0 && j + 1 < size.columns) {
        if (grid[i - 1][j + 1] == '#')
            liveCells++;
    }

    return liveCells;
}

//Function that gets the size of the board
void getSize(FILE *grid, gridSize *size) {
    int c;
    while (!feof(grid)) {
        c = fgetc(grid);
        if(feof(grid)) {
            break;
        }
        if (c == '\n') {
            size->rows++;
        } else if(c != ' '){
            size->columns++;
        }
    }

    //Checking if the board size is correct (e.g., all symbols in each row are equal to each other)
    if (size->columns % size->rows == 0) {
        size->columns /= size->rows;
    } else {
        printf("The board size is incorrect");
        exit(-1);
    }
    //Setting position in the file to the beginning, since because we traversed through the file it was at the end
    fseek(grid, 0, SEEK_SET);
}

//Initializes evolution of the board
void evolutionProcess(char **grid, gridSize size) {
    bool evolution = true;
    char input[10];
    gets(input);
    while (evolution) {
        evolve(grid, size);
        gets(input);

        if(strcmp(input, "") != 0 && strcmp(input, "/st") != 0 && strcmp(input, "/sv") != 0 && strcmp(input, "?help") != 0){
            printf("Command '%s' does not exist. Please type '?help' to see the list of available commands\n", input);
            gets(input);
        }

        if(strcmp(input, "?help") == 0) {
            printMenu();
            gets(input);
        }

        if(strcmp(input, "/st") == 0) {
            evolution = false;
        } else if(strcmp(input, "/sv") == 0) {
            evolution = false;
            saveGrid(grid, size);
        }

    }
}
char *getFileName(void) {
    char input[256];
    char initPath[] = {"D:\\CLionProjects\\Conwoy's game of life\\"};
    char fmt[] = {".txt"};
    char forbidden[] = {"!@#$%^&*()+=-*/|\"""'':;?<>,`"};
    printf("Please enter the name of the file:\n");
    gets(input);

    //Checking for illegal characters
    for (int i = 0; i < strlen(forbidden); ++i) {
        for (int j = 0; j < strlen(input); ++j) {
            if(input[j] == forbidden[i]) {
                printf("File name error!");
                exit(-1);
            }
        }
    }

    char *path = (char*) malloc(sizeof(char) * (strlen(input) + strlen(initPath)));
    strcpy(path, initPath);
    strcat(path, input);
    strcat(path, fmt);
    return path;
}

//Function which saves the last grid into file
void saveGrid(char **grid, gridSize size) {
    FILE *newState = fopen(getFileName(), "w");
    for (int i = 0; i < size.rows; i++) {
        for (int j = 0; j < size.columns; j++) {
            fprintf(newState, "%c ", grid[i][j]);
        }
        fprintf(newState, "\n");
    }
    fclose(newState);
}
