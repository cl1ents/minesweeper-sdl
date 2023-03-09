typedef struct Input Input;
typedef struct Array Array;

typedef struct GameGrid {
	Array * bombGrid;
	Array * displayGrid;
	int gridSize;
	int arraySize;
	int moveCount;
	int difficulty;
	int flagCount;
	int bombCount;

	int state;
} GameGrid;

void initGrid(GameGrid* grid);
void completeGrid(GameGrid* grid);
void placeBombs(GameGrid* grid, const Input* firstInput, int difficulty);

void placeFlag(GameGrid* grid, int x, int y);
int digAt(GameGrid* grid, int x, int y);
int countBombs(GameGrid* grid);
int countFound(GameGrid* grid);
int countNeighbours(GameGrid* grid, int x, int y);

int handleClick(GameGrid* grid, Input* input);