#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define CLEAR printf("\033[H\033[J")

enum Moves
{
    STOP,
    SLIDE,
    MERGE,
};

void placeNewTile();
void printBoard();
char getDirection();
enum Moves moveTile(int row, int column, char direction);
void moveBoard(char direction);
void printLogo();
int checkBoard(int board[][4]);
// void lostGame(); can use later

int board[4][4] = {BLANK};

int main(void)
{
    char direction;
    srand(time(NULL));
    placeNewTile();
    placeNewTile();
    while (1)
    {
        printLogo();
        printBoard();
        direction = getDirection();
        moveBoard(direction);
        placeNewTile();
        printf("direction: %c\n", direction);
        if (checkBoard(board) == 0){
            printf("ya lose, good luck next time.");
        } else if (checkBoard(board) == 2){
            printf("good job, ya won.");
        }
    }

    return 0;
}

void placeNewTile()
{
    // places a new tile on one of the empty tiles
    int tileVal = BLANK;
    int emptyTiles[16][2];

    // generate new tile value of "2" (90% chance) or "4" (10%  chance)
    if (rand() % 10 == 9)
        tileVal = 4;
    else
        tileVal = 2;

    // gather all empty tiles
    int n = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == BLANK)
            {
                emptyTiles[n][0] = i;
                emptyTiles[n][1] = j;
                n++;
            }
        }
    }

    // pick a random empty tile and give it the generated value
    int tilePosition = rand() % n;
    board[emptyTiles[tilePosition][0]][emptyTiles[tilePosition][1]] = tileVal;
}

void printBoard()
{
    printf("            -----+----+----+-----\n");
    for (int i = 0; i < 4; i++)
    {
        printf("            ");
        for (int j = 0; j < 4; j++)
        {
            printf("|%4.d", board[i][j]);
        }
        printf("|\n            -----+----+----+-----\n");
    }
}

char getDirection()
{
    char direction;
    printf("\n             Slide Direction: ");
    scanf(" %c", &direction); // space so it can read the value
    return direction;
}

enum Moves moveTile(int row, int column, char direction)
{
    // attempt to move a single tile and return its move type
    int tileVal = board[row][column];
    int nextRow = row;
    int nextColumn = column;

    // if attempting to slide a blank tile, stop
    if (tileVal == BLANK)
        return STOP;

    switch (direction)
    {
    case UP:
        nextRow--;
        break;
    case DOWN:
        nextRow++;
        break;
    case LEFT:
        nextColumn--;
        break;
    case RIGHT:
        nextColumn++;
        break;
    default:
        printf("\nError: Cannot read keyboard input!\n");
        break;
    }
    int nextTileVal = board[nextRow][nextColumn];

    // if the tile is attempting to slide of the board, stop
    if (nextRow < 0 || nextRow > 3 || nextColumn < 0 || nextColumn > 3)
    {
        printf("out of bounds: (%d,%d)\n", row, column);
        return STOP;
    }

    // if the next tile is blank, slide current tile to next spot
    else if (nextTileVal == BLANK)
    {
        printf("changed to next tile: (%d,%d)\n", row, column);
        board[nextRow][nextColumn] = board[row][column]; // IF THERE IS A VAL > 0 IN board[row][column]
        board[row][column] = BLANK;
    }

    // if the next tile is not blank and is the same value as the current tile, merge the tiles
    else if (nextTileVal == tileVal)
    {
        printf("merged tiles: (%d,%d)\n", row, column);
        board[nextRow][nextColumn] = tileVal << 1; // left shift because it's cooler than * 2
        board[row][column] = BLANK;
        return STOP;
    }

    // if the tiles are both occupied and do not match, stop
    else if (nextTileVal != BLANK && nextTileVal != tileVal)
    {
        printf("stopped due to val in next tile: (%d,%d\n", row, column);
        return STOP;
    }
    // if this happens I missed something
    else
        return STOP;
}

int mapTile(int row, int column)
{
    // map a tile's coordinates to a single index number
    return (row << 2) | column;

    /* This works because the highest row or column value is 3, which is two bits in binary.
       By left-shifting two bits, there is no overlap in the column and row values,
       which makes the bitwise OR is just the sum of the two values,
       which will be unique for every coordinate. */
}

void moveBoard(char direction)
{
    // move every tile on the board in a direction
    int rowStart = 0, columnStart = 0, rowEnd = 4, columnEnd = 4, rowCounter = 1, columnCounter = 1; // SET TO SPECIFIC VALUES BC SOME WERENT IF GIVEN A CERTAIN DIRECTION

    switch (direction)
    {
    case UP:
        rowStart = 1;
        rowCounter = 1;
        rowEnd = 4;
        break;
    case DOWN:
        rowStart = 2;
        rowCounter = -1;
        rowEnd = -1;
        break;
    case LEFT:
        columnStart = 1;
        columnCounter = 1;
        columnEnd = 4;
        break;
    case RIGHT:
        columnStart = 2;
        columnCounter = -1;
        columnEnd = -1;
        break;
    default:
        break;
    }

    int mergedTiles[] = {0};
    int tileIndex;
    enum Moves moveType;

    for (int k = 0; k < 3; k++)
    { // tiles can only move up to three times in any direction
        for (int i = rowStart; i != rowEnd; i += rowCounter)
        {
            for (int j = columnStart; j != columnEnd; j += columnCounter)
            {
                tileIndex = mapTile(i, j);
                moveType = moveTile(i, j, direction);
            }
        }
    }
}

void printLogo()
{
    puts("\n  _______  ________  ___   ___  ________     ");
    puts(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\    ");
    puts("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\   ");
    puts("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\  ");
    puts("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\ ");
    puts("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\");
    puts("    \\|_______|\\|_______|      \\|__|\\|_______|\n\n\n");
}

int checkBoard(int board[][4])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++){
            if (board[i][j] == board[i][j + 1]){
                return 1;
            }
            else if (board[i][j] == 2048){
                return 2;
            }
        }
    }
    return 0;
}