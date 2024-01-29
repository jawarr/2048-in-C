#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BLANK 0


void placeNewTile();
void printBoard();
char scanForDirection();

//intialize blank board 
int board[4][4] = {BLANK};


int main (void) {
    srand(time(NULL));
    placeNewTile();
    placeNewTile();
    printBoard();
    switch (scanForDirection()) {
    case 'w':
        printf("\nSUCCESS");
        break;
    default:
        break;
    }
    
    
    return 0;
}

void placeNewTile() {
    // place a new tile on one of the empty tiles.
    int tileVal = BLANK;
    // int tilePosi[2] = {0};
    int emptyTiles[16][2];
    
    //generate new tile value of "2" (90% chance) or "4" (10%  chance).
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
    printf("---------------------\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("|%4.d", board[i][j]);
        }
        printf("|\n---------------------\n");
    }
}

char scanForDirection() {
    printf("\nSlide Direction: ");
    char direction = getchar();
    return direction;
}