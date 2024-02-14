/*
Known bugs:
    - Tiles can merge multiple times in one move. In the original 2048 game, each tile can only merge once per move. 
        - e.g. a row of 2s: 
          [2][2][2][2]
          should merge into two 4s if moved left:
          [4][4][ ][ ]
          currently, the game outputs one 8:
          [8][ ][ ][ ]
        - I think we would need to use the mapTile function and the keep track of which tiles have been merged, but i haven't figured it out yet.
    
    - If the selected direction does not have any moves across the board, the game still places a new tile and lets you continue playing the game. It should force you to select a direction that moves the board in some way.

    - There is no check for win or lose condition, if the board is full with no legal moves, or a 2048 tile is created, nothing happens.

    - There needs to be a way to exit the game without the Ctrl + C.

Nice-to-haves but not necessary:
    - Keep track of score; in the original game, the score was the total value of the merged tiles, so the score only goes up when you merge tiles, and not with the randomly generated tiles.

    - Give a description of the game, rules, how to play, etc. before the game starts.

    - Its a little unintuitive how the new tiles are randomly placed because there is no way to animate sliding, maybe new tiles could be marked with an astrisk or similar.

    - Ability to reset the board with "r".
    
    - Ability to play with both arrow keys and WASD.

    - Clear the screen on operating systems other than Linux. I'm pretty sure "printf("\033[H\033[J")" only works on Linux.

    - Save a game/board to a file and have the ability to resume after closing the program.

    - Save highscore to the same file and display in the terminal.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define CLEAR printf("\033[H\033[J") // only works on linux, might have to comment out on other OS

enum Moves {
    STOP,
    SLIDE,
    MERGE,
};

void placeNewTile();
void printBoard();
char getDirection();
enum Moves moveTile(int row, int column, char direction); // changed return type
void moveBoard(char direction);
void printLogo();

int board[4][4] = {BLANK};


int main (void) {
    char direction;
    srand(time(NULL));
    
    placeNewTile();
    while (1) {
        printLogo();
        placeNewTile();
        printBoard();
        direction = getDirection();
        moveBoard(direction);
        CLEAR;
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
    char direction;
    printf("\n             Slide Direction: ");
    scanf(" %c", &direction); //space so it can read the value
    return direction;
}

enum Moves moveTile(int row, int column, char direction) { 
    //attempt to move a single tile and return its move type
    int tileVal = board[row][column];
    int nextRow = row;
    int nextColumn = column;

    // if attempting to slide a blank tile, stop
    if (tileVal == BLANK)
        return STOP;

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
        return STOP;
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
    
    //if the next tile is the same value as the current tile and has not already been merged, merge the tiles
    else if (nextTileVal == tileVal) {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal << 1; //left shift because it's cooler than * 2
        return MERGE;
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
    int rowStart=0, columnStart=0, rowEnd=4, columnEnd=4, rowCounter=1, columnCounter=1;
    
    switch (direction) {
    case UP:
        rowStart = 1;
        break;
    case DOWN:
        rowStart = 2;
        rowCounter = -1;
        rowEnd = -1;
        break;
    case LEFT:
        
        break;
    case RIGHT:
        columnStart = 2;
        columnCounter = -1;
        columnEnd = -1;
        break;
    default:
        break;
    }
    
    int mergedTiles[] = {1};
    int tileIndex;
    enum Moves moveType;

    for (int k = 0; k < 3; k++) { // tiles can only move up to three times in any direction
        for (int i = rowStart; i != rowEnd; i += rowCounter) { 
            for (int j = columnStart; j != columnEnd; j += columnCounter) { 
                tileIndex = mapTile(i, j);
                moveType = moveTile(i, j, direction);
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