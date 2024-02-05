#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define CLEAR printf("\033[H\033[J")

enum MoveType {
    STOP,
    SLIDE,
    MERGE
};

void placeNewTile();
void printBoard();
char getDirection();
enum MoveType moveTile(int row, int column, char direction);
void printLogo();

int board[4][4] = {2, 2, BLANK};


int main (void) {

    srand(time(NULL));
    
    while (1) {
        printLogo();
        printBoard();
        moveTile(0, 0, getDirection());
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

enum MoveType moveTile(int row, int column, char direction) {
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
        printf("\nError: Cannot read keynboard input!\n");
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
        board[nextRow][nextColumn] = tileVal*2;
    }

    //if the tiles are both occupied and do not match, stop
    else if (nextTileVal != BLANK && nextTileVal != tileVal) return STOP;
    
    //if this happens I missed something
    else printf("This is not supposed to happen.");
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