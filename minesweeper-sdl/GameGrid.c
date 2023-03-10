#include <stdio.h>
#include <stdlib.h>
#include "GameGrid.h"
#include "Array.h"
#include "CONSTS.h"
#include "Input.h"

// VARIBLES //

const int diagonalCoords[4][2] = {
        {3, 3},
        {-3, 3},
        {3, -3},
        {-3, -3},
};


// FUNCTIONS //

void initGrid(GameGrid* grid);
void placeBombs(GameGrid* grid, const Input* firstInput, int difficulty);
void completeGrid(GameGrid* grid);

void placeFlag(GameGrid* grid, int x, int y);
int digAt(GameGrid* grid, int x, int y);
int countBombs(GameGrid* grid);
int countFound(GameGrid* grid);
int countNeighbours(GameGrid* grid, int x, int y);

// Game logic

/// <summary>
/// Initialises grid
/// </summary>
/// <param name="grid"></param>
void initGrid(GameGrid* grid)
{
    grid->moveCount = 0;
    grid->state = 0;

    grid->bombCount = grid->arraySize / grid->difficulty;
    grid->flagCount = 0;

    free(grid->bombGrid->array);
    initArraySize(grid->bombGrid, grid->arraySize);

    free(grid->displayGrid->array);
    initArraySize(grid->displayGrid, grid->arraySize);

    // static Array bombGrid = newArraySize(grid->arraySize);
    // grid->bombGrid;
    for (int i = 0; i < grid->arraySize; i++)
    {
        insertInto(grid->bombGrid, 0);
    }

    for (int i = 0; i < grid->arraySize; i++)
    {
        insertInto(grid->displayGrid, 0);
    }
}

/// <summary>
/// checks whether or not the diagonals of x and y are bombs
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>0 or 1</returns>
int checkDiagonal(GameGrid* grid, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int bx = x + diagonalCoords[i][0];
        int by = y + diagonalCoords[i][1];
        if (bx >= 0 && bx < grid->gridSize && by >= 0 && by < grid->gridSize && grid->bombGrid->array[bx + by * grid->gridSize] == 1)
        {
            return 0;
        }
    }

    return 1;
}

/// <summary>
/// place bombs according to the first input
/// </summary>
/// <param name="grid"></param>
/// <param name="firstInput"></param>
void placeBombs(GameGrid* grid, const Input* firstInput, int difficulty)
{
    int bombCount = grid->arraySize / difficulty;
    grid->bombCount = 0;

    Array freeSpace = newArraySize(grid->arraySize);
    for (int i = 0; i < grid->arraySize; i++)
    {
        int x = i % grid->gridSize;
        int y = i / grid->gridSize;
        if (abs(x-firstInput->x) > 1 || abs(y - firstInput->y) > 1)
            insertInto(&freeSpace, i);
    }

    while (bombCount > 0 && freeSpace.length > 0)
    {
        int index = removeFrom(&freeSpace, rand() % freeSpace.length);
        int x = index % grid->gridSize;
        int y = index / grid->gridSize;
        if (grid->bombGrid->array[x + y * grid->gridSize] == 0 && 
            countNeighbours(grid, x, y) <= 7 && 
            checkDiagonal(grid, x, y))
        {
            grid->bombGrid->array[x + y * grid->gridSize] = 1;

            grid->bombCount++;
            bombCount--;
        }
    }
    // free(freeSpace.array);
}

int countNeighbours(GameGrid* grid, int x, int y) {
    int count = 0;
    for (int bx = x - 1; bx <= x + 1; bx++)
    {
        for (int by = y - 1; by <= y + 1; by++)
        {
            if (bx >= 0 && bx < grid->gridSize && by >= 0 && by < grid->gridSize) {
                count += grid->bombGrid->array[bx + by * grid->gridSize];
            }
        }
    }
    return count;
}

/// <summary>
/// reveals everything in the grid
/// </summary>
/// <param name="grid"></param>
void completeGrid(GameGrid* grid) 
{
    for (int x = 0; x < grid->gridSize; x++)
    {
        for (int y = 0; y < grid->gridSize; y++)
        {
            int i = x + y * grid->gridSize;
            if (grid->displayGrid->array[i] == 0 || grid->displayGrid->array[i] == 12) {
                int index = 1;
                if (grid->bombGrid->array[i] == 1) {
                    index = 11;
                }
                else {
                    index += countNeighbours(grid, x, y);
                }
                grid->displayGrid->array[i] = index;
            }
        }
    }
}

/// <summary>
/// Place flags at X Y
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void placeFlag(GameGrid* grid, int x, int y) {
    if (grid->displayGrid->array[x + y * grid->gridSize] == 0 && grid->flagCount < grid->bombCount) {
        grid->flagCount++;
        grid->displayGrid->array[x + y * grid->gridSize] = 12;
    }
    else if (grid->displayGrid->array[x + y * grid->gridSize] == 12) {
        grid->flagCount--;
        grid->displayGrid->array[x + y * grid->gridSize] = 0;
    }
}

/// <summary>
/// digs at a coordinate
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>if there's a bomb, it returns 1, else 0</returns>
int digAt(GameGrid* grid, int x, int y)
{
    if (grid->bombGrid->array[x + y * grid->gridSize] == 1) {
        grid->displayGrid->array[x + y * grid->gridSize] = 13;
        return 1;
    }
    else {
        int index = 1;
        int count = countNeighbours(grid, x, y);
        index += count;
        grid->displayGrid->array[x + y * grid->gridSize] = index;

        if (count == 0) {
            for (int bx = x - 1; bx <= x + 1; bx++)
            {
                if (bx >= 0 && bx < grid->gridSize)
                {
                    for (int by = y - 1; by <= y + 1; by++)
                    {
                        if (by >= 0 && by < grid->gridSize && (grid->displayGrid->array[bx + by * grid->gridSize] == 0 || grid->displayGrid->array[bx + by * grid->gridSize] == 12)) {
                            if (grid->displayGrid->array[bx + by * grid->gridSize] == 12)
                                grid->flagCount--;
                            /*if (((bx == x - 1 && by == y - 1) || (bx == x + 1 && by == y - 1) || (bx == x + 1 && by == y + 1) || (bx == x - 1 && by == y + 1)))
                            {
                                if (countNeighbours(grid, bx, by) > 0)
                                {
                                    digAt(grid, bx, by);
                                }
                            }
                            else*/
                            //{
                                digAt(grid, bx, by);
                            //}
                        }
                    }
                }
            }
        }
        return 0;
    }
}

int handleClick(GameGrid* grid, Input* input) {
    if (input->flag)
        placeFlag(grid, input->x, input->y);
    else 
    {
        if (grid->displayGrid->array[input->x + input->y * grid->gridSize] == 0) 
        {
            if (!grid->moveCount)
                placeBombs(grid, input, grid->difficulty);
            grid->moveCount++;
            if (digAt(grid, input->x, input->y))
            {
                grid->state = 1;
                completeGrid(grid);
                return 2;
            } 
            else if(countFound(grid) == grid->bombCount)
                grid->state = 2;

            return 1;
        }
    }
    return 0;
}

int countBombs(GameGrid* grid)
{
    int count = 0;
    for (int i = 0; i < grid->arraySize; i++) {
        count += grid->bombGrid->array[i];
    }
    return count;
}

int countFound(GameGrid* grid)
{
    int count = 0;
    for (int i = 0; i < grid->arraySize; i++) {
        count += grid->displayGrid->array[i] == 0 || grid->displayGrid->array[i] == 12;
    }
    return count;
}

// Display Functions

int findIndex(char character) {
    int index = -1;

    for (int i = 0; i < sizeof charset; i++) {
        if (i == character) {
            index = i;
            break;
        }
    }

    return index;
}
