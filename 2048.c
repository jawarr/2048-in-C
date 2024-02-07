#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define CLEAR printf("\033[H\033[J")

enum Moves {
    STOP,
    SLIDE,
    MERGE
};

void placeNewTile();
void printBoard();
char getDirection();
enum Moves moveTile(int row, int column, char direction);
void moveBoard(char direction);
void printLogo();

int board[4][4] = {BLANK};


int main (void) {

    srand(time(NULL));
    placeNewTile();
    placeNewTile();
    while (1) {
        printLogo();
        printBoard();
        moveBoard(getDirection());
        // CLEAR;
    }
    
    
    
    
    
    
    return 0;
}


void placeNewTile() {
    // places a new tile on one of the empty tiles
    int tileVal = BLANK;
    int emptyTiles[16][2];
    
    //generate new tile value of "2" (90% chance) or "4" (10%  chance)
    if (rand() % 10 == 9) tileVal = 4;
    else tileVal = 2;

    //gather all empty tiles
    int n = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == BLANK) {
                emptyTiles[n][0] = i;
                emptyTiles[n][1] = j;
                n++;
            }
        }
    }

    //pick a random empty tile and give it the generated value
    int tilePosition = rand() % n;
    board[emptyTiles[tilePosition][0]][emptyTiles[tilePosition][1]] = tileVal;
}

void printBoard() {
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

char getDirection() {
    printf("\n             Slide Direction: ");
    char direction = getchar();
    return direction;
}

enum Moves moveTile(int row, int column, char direction) {
    //attempt to move a single tile and return its move type
    int tileVal = board[row][column];
    int nextRow = row;
    int nextColumn = column;
    printf("%c\n", direction);

    //if attempting to slide a blank tile, stop
    if (tileVal == BLANK) return STOP;
    
    switch (direction) {
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

    //if the tile is attempting to slide of the board, stop
    if (nextRow < 0 || nextRow > 3 || nextColumn < 0 || nextColumn > 3) return STOP;
    
    //if the next tile is blank, slide current tile to next spot
    else if (nextTileVal == BLANK) {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal;
        return SLIDE;
    }
    
    //if the next tile is not blank and is the same value as the current tile, merge the tiles
    else if (nextTileVal == tileVal) {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal << 1; //left shift because it's cooler than * 2
    }

    //if the tiles are both occupied and do not match, stop
    else if (nextTileVal != BLANK && nextTileVal != tileVal) return STOP;
    
    //if this happens I missed something
    else printf("This is not supposed to happen.");
}

int mapTile(int row, int column) {
    //map a tile's coordinates to a single index number
    return (row << 2) | column;

 /* This works because the highest row or column value is 3, which is two bits in binary.
    By left-shifting two bits, there is no overlap in the column and row values,
    which makes the bitwise OR is just the sum of the two values, 
    which will be unique for every coordinate. */
}

void moveBoard(char direction) {
    //move every tile on the board in a direction
    int rowStart, columnStart, rowEnd, columnEnd, rowCounter, columnCounter;
    
    switch (direction) {
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

    for (int k = 0; k < 3; k++) { // tiles can only move up to three times in any direction
        for (int i = rowStart; i != rowEnd; i += rowCounter) {
            for (int j = columnStart; j != columnEnd; j += columnCounter) {
                tileIndex = mapTile(i, j);
                moveType = moveTile(i, j, direction);
                // if (moveType == MERGE) {
                //     mergedTiles[tileIndex] = 1;
                // }

            }
            

        }


    }



}

void printLogo() {
    puts("\n  _______  ________  ___   ___  ________     ");
    puts(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\    ");
    puts("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\   ");
    puts("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\  ");
    puts("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\ ");
    puts("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\");
    puts("    \\|_______|\\|_______|      \\|__|\\|_______|\n\n\n");
}