#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'



void placeNewTile();
void printBoard();
char getDirection();

int board[4][4] = {BLANK};


int main (void) {
    
    puts("  _______  ________  ___   ___  ________     ");
    puts(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\    ");
    puts("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\   ");
    puts("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\  ");
    puts("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\ ");
    puts("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\");
    puts("    \\|_______|\\|_______|      \\|__|\\|_______|\n\n\n");

    
    srand(time(NULL));
    placeNewTile();
    placeNewTile();
    printBoard();
    
    switch (getDirection()) {
    case UP:
        
        break;
    case DOWN:
        break;
    case LEFT:
        break;
    case RIGHT:
        break;
    default:
        break;
    }
    
    
    return 0;
}

// places a new tile on one of the empty tiles.
void placeNewTile() {
    int tileVal = BLANK;
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

void moveUP() {
    int occupiedTiles[16][2];
    //gather all occupied tiles
    int n = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] != BLANK) {
                occupiedTiles[n][0] = i;
                occupiedTiles[n][1] = j;
                n++;
            }
        }
    }
}
