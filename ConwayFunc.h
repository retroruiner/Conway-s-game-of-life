//Structure that contains size of the grid (rows and columns)
typedef struct gridSize {
    int rows;
    int columns;
} gridSize;

void checkFile(FILE *file);
void initBoard(FILE *initState, char **grid, gridSize size);
void printMenu(void);
void printBoard(char **grid, gridSize size);
void evolve(char **grid, gridSize size);
void countAllLive(char **grid, gridSize size);
void getSize(FILE *grid, gridSize *size);
void saveGrid(char **grid, gridSize size);
void evolutionProcess(char **grid, gridSize size);
int countLive(char **grid, int i, int j, gridSize size);
char *getFileName(void);