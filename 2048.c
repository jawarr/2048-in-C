/*
Known bugs:
    - (COMPLETE) Tiles can merge multiple times in one move. In the original 2048 game, each tile can only merge once per move. 
        - e.g. a row of 2s: 
          [2][2][2][2]
          should merge into two 4s if moved left:
          [4][4][ ][ ]
          currently, the game outputs one 8:
          [8][ ][ ][ ]
        - I think we would need to use the mapTile function and the keep track of which tiles have been merged, but i haven't figured it out yet.
    
    - (COMPLETE) If the selected direction does not have any moves across the board, the game still places a new tile and lets you continue playing the game. It should force you to select a direction that moves the board in some way.

    - There is no check for win or lose condition, if the board is full with no legal moves, or a 2048 tile is created, nothing happens.

    - (COMPLETE) There needs to be a way to exit the game without the Ctrl + C.

    - (COMPLETE) A new tile doesn't spawn if the board is full before your last move.

Nice-to-haves but not necessary:
    - Keep track of score; in the original game, the score was the total value of the merged tiles, so the score only goes up when you merge tiles, and not with the randomly generated tiles.

    - Give a description of the game, rules, how to play, etc. before the game starts.

    - Its a little unintuitive how the new tiles are randomly placed because there is no way to animate sliding, maybe new tiles could be marked with an astrisk or similar.

    - (COMPLETE) Ability to reset the board with "r".
    
    - Ability to play with both arrow keys and WASD.

    - Clear the screen on operating systems other than Linux. I'm pretty sure "printf("\033[H\033[J")" only works on Linux.

    - Save a game/board to a file and have the ability to resume after closing the program.

    - Save highscore to the same file and display in the terminal.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLANK 0
#define CLEAR printf("\033[H\033[J") // only works on linux, might have to comment out on other OS

enum Inputs {
    // ASCII codes
    UP = 119, // w
    DOWN = 115, // s
    LEFT = 97, // a
    RIGHT = 100, // d
    RESET = 114, // r
    EXIT = 120 // x
};

enum Moves {
    STOP,
    SLIDE,
    MERGE,
    WIN
};

void placeNewTile();
void printBoard();
char getInput();
enum Moves moveTile(int row, int column, char direction, int merged);
int moveBoard(char direction);
void printLogo();
void resetBoard();
void checkBoard();
void win();
void lose();



// int board[4][4] = {BLANK};
// int board[4][4] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 32, 8, 16, 32, BLANK}; //lose condition board
int board[4][4] = {1024, 1024, BLANK};

int score = 0;
int won = 0;
int lost = 0;

int main (void) {
    char ch;
    int moved = 0;
    srand(time(NULL));
    resetBoard();
   
    while (!won && !lost) {
        CLEAR;
        printLogo();
        printBoard();
        ch = getInput();
        switch (ch) //switch prevents undefined behavior
        {
        case UP:
            moved = moveBoard(UP);
            break;
        case DOWN:
            moved = moveBoard(DOWN);
            break;
        case LEFT:
            moved = moveBoard(LEFT);
            break;
        case RIGHT:
            moved = moveBoard(RIGHT);
            break;
        case RESET:
            moved = 0;
            resetBoard();
            break;
        case EXIT:
            return 0;
        default:
            break;
        }

        if (moved) {
            placeNewTile();
        }

        checkBoard();
    }

    if (won) win();
    else lose();

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
    printf("\033[0;90m            -----+----+----+-----\n");
    for (int i = 0; i < 4; i++)
    {
        printf("            ");
        for (int j = 0; j < 4; j++)
        {
            printf("\033[0;90m|");
            //ANSI color codes to color the output numbers
            switch (board[i][j])
            {
            case 2:
                printf("\033[0;37m"); //WHITE
                printf("  %d ", board[i][j]);
                break;
            case 4:
                printf("\033[0;31m"); //RED
                printf("  %d ", board[i][j]);
                break;
            case 8:
                printf("\033[0;33m"); //YELLOW
                printf("  %d ", board[i][j]);
                break;
            case 16:
                printf("\033[0;32m"); //GREEN
                printf(" %d ", board[i][j]);
                break;
            case 32:
                printf("\033[0;36m"); //CYAN
                printf(" %d ", board[i][j]);
                break;
            case 64:
                printf("\033[0;35m"); //PURPLE
                printf(" %d ", board[i][j]);
                break;
            case 128:
                printf("\033[1;31m"); //BOLD RED
                printf(" %d", board[i][j]);
                break;
            case 256:
                printf("\033[1;33m"); //BOLD YELLOW
                printf(" %d", board[i][j]);
                break;
            case 512:
                printf("\033[1;32m"); //BOLD GREEN
                printf(" %d", board[i][j]);
                break;
            case 1024:
                printf("\033[1;36m"); //BOLD CYAN
                printf("%d", board[i][j]);
                break;
            case 2048:
                printf("\033[1;35m"); //BOLD PURPLE
                printf("%d", board[i][j]);
                break;
            default:
                printf("%4.d", board[i][j]);
                break;
            }
            printf("\033[0m");
        }
        printf("\033[0;90m|\n            -----+----+----+-----\n\033[0m");
    }
}

char getInput() {
    printf("\n             Slide Direction: ");
    char direction;
    scanf(" %c", &direction); //space so it can read the value
    return direction;
}

void resetBoard() {
    // reset everything 
    // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < 4; j++) {
    //         board[i][j] = BLANK;
    //     }
    // }
    placeNewTile();
    placeNewTile();
}

enum Moves moveTile(int row, int column, char direction, int merged) { 
    //attempt to move a single tile and return its move type
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
    else if (nextTileVal == tileVal && !merged) {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal << 1; //left shift because it's cooler than * 2
        if (tileVal == 2048) return WIN;
        else return MERGE;
    }

    //if the tiles are both occupied and do not match, stop
    else if (nextTileVal != BLANK && nextTileVal != tileVal) return STOP;
}

int indexTile(int row, int column) {
    //map a tile's coordinates to a single index number

    return (row << 2) | column;
    /* This works because the highest row or column value is 3, which is two bits in binary.
       By left-shifting two bits, there is no overlap in the column and row values,
       which makes the bitwise OR is just the sum of the two values,
       which will be unique for every coordinate. */
}

int moveBoard(char direction) {
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

    
    int merged[16] = {0};
    int tileIndex;
    enum Moves moveType;
    int moved = 0;

    for (int k = 0; k < 3; k++) { // tiles can only move up to three times in any direction
        for (int i = rowStart; i != rowEnd; i += rowCounter) { 
            for (int j = columnStart; j != columnEnd; j += columnCounter) { 
                tileIndex = indexTile(i, j);
                moveType = moveTile(i, j, direction, merged[tileIndex]);
                if (moveType == WIN) {
                    moved = 1;
                    won = 1;
                }
                else if (moveType == MERGE) {
                    //block tile from being moved
                    merged[tileIndex] = 1;
                    //block next tile as well
                    int next_row = i + ((direction == UP || direction == DOWN) ? rowCounter * -1 : 0);
                    int next_column = j + ((direction == RIGHT || direction == LEFT) ? columnCounter * -1 : 0);
                    int next_tile_index = indexTile(next_row, next_column);
                    merged[next_tile_index] = 1;
                    moved = 1;
                }
                else if (moveType != STOP) moved = 1;
            }
        }
    }
    return moved;
}



void checkBoard() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((board[i][j] == BLANK) ||
                (i != 3 && board[i][j] == board[i + 1][j]) || 
                (j != 3 && board[i][j] == board[i][j + 1])) 
                return;
        }
    }
    lost = 1;
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

void lose() {
    CLEAR;
    printf("\033[0;31m");
    puts("\n ___       ________  ________  _________  ___");       
    puts("|\\  \\     |\\   __  \\|\\   ____\\|\\___   ___\\\\  \\");      
    puts("\\ \\  \\    \\ \\  \\|\\  \\ \\  \\___|\\|___ \\  \\_\\ \\  \\");     
    puts(" \\ \\  \\    \\ \\  \\\\\\  \\ \\_____  \\   \\ \\  \\ \\ \\  \\");    
    puts("  \\ \\  \\____\\ \\  \\\\\\  \\|____|\\  \\   \\ \\  \\ \\ \\__\\");   
    puts("   \\ \\_______\\ \\_______\\____\\_\\  \\   \\ \\__\\ \\|__|");   
    puts("    \\|_______|\\|_______|\\_________\\   \\|__|      ___"); 
    puts("                        \\|_________|            |\\__\\");
    puts("                                                \\|__|\n");
    printf("\033[0m");
    printBoard();
}

void win() {
    CLEAR;
    printf("\033[0;32m");
    puts(" ___       __   ___  ________   ___");       
    puts("|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\  \\");      
    puts("\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\");     
    puts(" \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\");    
    puts("  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\__\\");   
    puts("   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\|__|");   
    puts("    \\|____________|\\|__|\\|__| \\|__|   ___"); 
    puts("                                     |\\__\\");
    puts("                                     \\|__|");
    printf("\033[0m");
    printBoard();
}

