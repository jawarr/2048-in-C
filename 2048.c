/*
    - Its a little unintuitive how the new tiles are randomly placed because there is no way to animate sliding, maybe new tiles could be marked with an astrisk or similar.

    - Ability to play with both arrow keys and WASD.

    - Save a game/board to a file and have the ability to resume after closing the program.

    - Save highscore to the same file and display in the terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BLANK 0
#define CLEAR printf("\033[H\033[J")

enum Inputs
{
    // ASCII codes
    UP = 119,    // w
    DOWN = 115,  // s
    LEFT = 97,   // a
    RIGHT = 100, // d
    RESET = 256, //first available number that isn't an ASCII code
    EXIT = 257, 
    HELP = 258
};

enum Moves
{
    STOP,
    SLIDE,
    MERGE
};

void placeNewTile();
void printBoard();
enum Inputs getInput();
enum Moves moveTile(int row, int column, char direction, int merged);
int moveBoard(char direction);
void printLogo();
void printScore();
void resetBoard();
void checkWin();
void checkLoss();
void win();
void lose();
void help();



int board[4][4] = {BLANK};

int score = 0;
int won = 0;
int lost = 0;

int main (void) 
{
    enum Inputs input;
    int moved = 0;
    int firstTime = 1; 
    srand(time(NULL));
    resetBoard();

    while (1)
    {
        CLEAR;
        printLogo();

        //only prints when you first run the program.
        if (firstTime) 
        {
            printf("          Enter \"help\" for a tutorial.");
            firstTime = 0;
        }

        printf("\n\n");

        printScore();
        printBoard();
        input = getInput();
        
        switch (input) //switch prevents undefined behavior
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
        case HELP:
            help();
            moved = 0;
            break;
        case RESET:
            moved = 0;
            resetBoard();
            break;
        case EXIT:
            exit(0);
        default:
            moved = 0;
            break;
        }

        if (moved) placeNewTile();

        checkWin();
        checkLoss();

        if (won || lost) break;
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

void printScore() 
{
    //prints the score
    if (score == 0){
        printf("                 Score:      0\n\n");
    }
    else printf("                 Score:%7.d\n\n", score);
}

void printBoard() 
{
    //prints the game board with formatting and colors
    printf("\033[0;90m             -----+----+----+-----\n");
    for (int i = 0; i < 4; i++)
    {
        printf("             ");
        for (int j = 0; j < 4; j++)
        {
            printf("\033[0;90m|");
            // ANSI color codes to color the output numbers
            switch (board[i][j])
            {
            case 2:
                printf("\033[0;37m"); // WHITE
                printf("  %d ", board[i][j]);
                break;
            case 4:
                printf("\033[0;31m"); // RED
                printf("  %d ", board[i][j]);
                break;
            case 8:
                printf("\033[0;33m"); // YELLOW
                printf("  %d ", board[i][j]);
                break;
            case 16:
                printf("\033[0;32m"); // GREEN
                printf(" %d ", board[i][j]);
                break;
            case 32:
                printf("\033[0;36m"); // CYAN
                printf(" %d ", board[i][j]);
                break;
            case 64:
                printf("\033[0;35m"); // PURPLE
                printf(" %d ", board[i][j]);
                break;
            case 128:
                printf("\033[1;31m"); // BOLD RED
                printf(" %d", board[i][j]);
                break;
            case 256:
                printf("\033[1;33m"); // BOLD YELLOW
                printf(" %d", board[i][j]);
                break;
            case 512:
                printf("\033[1;32m"); // BOLD GREEN
                printf(" %d", board[i][j]);
                break;
            case 1024:
                printf("\033[1;36m"); // BOLD CYAN
                printf("%d", board[i][j]);
                break;
            case 2048:
                printf("\033[1;35m"); // BOLD PURPLE
                printf("%d", board[i][j]);
                break;
            default:
                printf("%4.d", board[i][j]);
                break;
            }
            printf("\033[0m");
        }
        printf("\033[0;90m|\n             -----+----+----+-----\n\033[0m");
    }
}

enum Inputs getInput() 
{
    printf("\n                       ");
    char str[5];
    enum Inputs input;
    scanf(" %s", str); //space so it can read the value
    if (!strcmp(str, "reset")) input = RESET;
    else if (!strcmp(str, "exit")) input = EXIT;
    else if (!strcmp(str, "help")) input = HELP;
    else input = (int)str[0];
    return input;
}

void resetBoard() 
{
    // reset everything
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = BLANK;
        }
    }
    score = 0;
    placeNewTile();
    placeNewTile();
}

enum Moves moveTile(int row, int column, char direction, int merged) 
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
        break;
        return STOP;
    }

    int nextTileVal = board[nextRow][nextColumn];

    // if the tile is attempting to slide of the board, stop
    if (nextRow < 0 || nextRow > 3 || nextColumn < 0 || nextColumn > 3)
        return STOP;

    // if the next tile is blank, slide current tile to next spot
    else if (nextTileVal == BLANK)
    {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal;
        return SLIDE;
    }

    // if the next tile is the same value as the current tile and has not already been merged, merge the tiles
    else if (nextTileVal == tileVal && !merged)
    {
        board[row][column] = BLANK;
        board[nextRow][nextColumn] = tileVal << 1; // left shift because it's cooler than * 2
        score += tileVal << 1;
        return MERGE;
    }

    // if the tiles are both occupied and do not match, stop
    else if (nextTileVal != BLANK && nextTileVal != tileVal)
        return STOP;
}

int indexTile(int row, int column)
{
    // map a tile's coordinates to a single index number

    return (row << 2) | column;
    /* This works because the highest row or column value is 3, which is two bits in binary.
       By left-shifting two bits, there is no overlap in the column and row values,
       which makes the bitwise OR is just the sum of the two values,
       which will be unique for every coordinate. */
}

int moveBoard(char direction)
{
    // move every tile on the board in a direction
    int rowStart = 0, columnStart = 0, rowEnd = 4, columnEnd = 4, rowCounter = 1, columnCounter = 1;

    switch (direction)
    {
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

    //array to track which tiles have merged
    int merged[16] = {0};
    int tileIndex;
    enum Moves moveType;
    
    //if moved == 0 after this function executes, the board did not move.
    int moved = 0;

    for (int k = 0; k < 3; k++) // 3 because tiles can only move up to three times in any direction
    { 
        for (int i = rowStart; i != rowEnd; i += rowCounter)
        {
            for (int j = columnStart; j != columnEnd; j += columnCounter)
            {
                tileIndex = indexTile(i, j);
                moveType = moveTile(i, j, direction, merged[tileIndex]);
                if (moveType == MERGE)
                {
                    // block tile from being moved
                    merged[tileIndex] = 1;
                    // block next tile as well
                    int next_row = i + ((direction == UP || direction == DOWN) ? rowCounter * -1 : 0);
                    int next_column = j + ((direction == RIGHT || direction == LEFT) ? columnCounter * -1 : 0);
                    int next_tile_index = indexTile(next_row, next_column);
                    merged[next_tile_index] = 1;
                    moved = 1;
                }
                else if (moveType != STOP)
                    moved = 1;
            }
        }
    }
    return moved;
}

//checks the board for a 2048 tile
void checkWin() 
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 2048) {
                won = 1;
                return;
            }
        }
    }
}

//checks if the board is full with no moves left
void checkLoss()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((board[i][j] == BLANK) ||
                (i != 3 && board[i][j] == board[i + 1][j]) ||
                (j != 3 && board[i][j] == board[i][j + 1]))
                return;
        }
    }
    lost = 1;
}

//tutorial
void help() 
{
    //create a back-up of the board and score so the player can resume after the tutorial is over.
    //try memcpy later
    int scoreCopy = score;
    int boardCopy[4][4];
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) boardCopy[i][j] = board[i][j];
    }

    //print the controls
    CLEAR;
    printLogo();
    printf("                 CONTROLS:\n\n");
    printf("            w = slide board up ↑\n");
    printf("            a = slide board left ←\n");
    printf("            s = slide board down ↓\n");
    printf("            d = slide board right →\n\n");
    printf("            reset = reset the game\n");
    printf("            exit = exit the program\n");
    printf("            help = view the tutorial\n\n");
    printf("            Enter any key to continue.\n");
    getInput();

    //make the board blank except for one "2".
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) board[i][j] = (i == 1 && j == 1) ? 2 : BLANK;
    
    }

    //loops until player has entered every direction
    enum Inputs input;
    int u = 0, d = 0, l = 0, r = 0;
    while(1) 
    {
        CLEAR;
        printLogo();
        //print green checkmark for each direction entered
        printf("      Slide the tiles using WASD + ENTER.\n\n         up[%s] down[%s] left[%s] right[%s]\n\n", (u == 1?"\033[0;92m✓\033[0m":" "), (d == 1?"\033[0;92m✓\033[0m":" "), (l == 1?"\033[0;92m✓\033[0m":" "), (r == 1?"\033[0;92m✓\033[0m":" "));
        printBoard();
        
        input = getInput();
        switch (input) 
        {
            case UP:
                moveBoard(UP);
                u = 1;
                break;
            case DOWN:
                moveBoard(DOWN);
                d = 1;
                break;
            case LEFT:
                moveBoard(LEFT);
                l = 1;
                break;
            case RIGHT:
                moveBoard(RIGHT);
                r = 1;
                break;
            case EXIT:
                return;
            default:
                break;
        }

        if ((u && d) && (l && r)) break;
    }

    //reset board with new values
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i == 0 && j == 1) || (i == 1 && j == 0)) board[i][j] = 2;
            else if (i == 0 && j == 0) board[i][j] = 4;
            else board[i][j] = BLANK;
        }
    }

    //loops until player creates a "8" tile
    while(1)
    {
        CLEAR;
        printLogo();
        printf("        Equal tiles merge; unequal do not.\n\n              Create an \"8\" tile:\n\n");
        printBoard();
        
        input = getInput();
        switch (input) //switch prevents undefined behavior
        {
        case UP:
            moveBoard(UP);
            break;
        case DOWN:
            moveBoard(DOWN);
            break;
        case LEFT:
            moveBoard(LEFT);
            break;
        case RIGHT:
            moveBoard(RIGHT);
            break;
        default:
            break;
        }


        //check for 16
        int check = 0;
        for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 4; j++) if (board[i][j] == 8) check = 1;
        }

        if (check == 1) break;
    }

    int counter = 0;
    int moved = 0;
    //loops until board is moved 4 times
    while(counter < 4) 
    {
        CLEAR;
        printLogo();
        printf("A new tile is randomly placed after each slide.\n\n          Slide the board 4 times.\n\n");
        printBoard();
        
        input = getInput();
        switch (input) //switch prevents undefined behavior
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
        default:
            moved = 0;
            break;
        }

        if (moved) placeNewTile();
        
        counter++;
    }

    //lose condition board
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) board[i][j] = 2 << (i+j+1);
    }
    
    CLEAR;
    printLogo();
    printf("If the board is full & no merge left, you lose.\n\n          Enter any key to continue.\n\n");
    printBoard();
    getInput();

    //2028 tile board
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) board[i][j] = (i == 1 && j == 1) ? 2048 : 0;
    }

    CLEAR;
    printLogo();
    printf("      If you create a 2048 tile, you win!\n\n         Enter any key to resume game.\n\n");
    printBoard();
    getInput();

    //set board and score to how they were before tutorial
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) board[i][j] = boardCopy[i][j];
    }

    score = scoreCopy;
}

//prints the 2048 logo
void printLogo() 
{
    puts("\n  _______  ________  ___   ___  ________     ");
    puts(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\    ");
    puts("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\   ");
    puts("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\  ");
    puts("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\ ");
    puts("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\");
    puts("    \\|_______|\\|_______|      \\|__|\\|_______|\n");
}

void lose()
{
    CLEAR;
    printf("\033[0;91m"); // RED
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
    printScore();
    printBoard();
}

void win()
{
    CLEAR;
    printf("\033[0;92m"); // GREEN
    puts("\n ___       __   ___  ________   ___");
    puts("|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\  \\");
    puts("\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\");
    puts(" \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\");
    puts("  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\__\\");
    puts("   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\|__|");
    puts("    \\|____________|\\|__|\\|__| \\|__|   ___");
    puts("                                     |\\__\\");
    puts("                                     \\|__|\n");
    printf("\033[0m");
    printScore();
    printBoard();
}
