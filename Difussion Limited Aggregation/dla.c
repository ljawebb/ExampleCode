/*
* Difussion Limited Aggregation (DLA) using neillncurses.
* An attempt to mimic DLA using a grid of squares.
* Laurence Webb 2017 <lw17773@my.bristol.ac.uk>
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neillncurses.h"

#define ARRAYSIZE    50
#define MAXSQUARES   250
#define DIRECTION    4
#define DELAYTIME    25.0
#define EDGE         0
#define CELLSHIFT    1
#define OPPOSINGEDGE ARRAYSIZE-1

enum direction {north, south, west, east};

void fillarray(char squaregrid[][ARRAYSIZE]);
void createsquare(char squaregrid[][ARRAYSIZE], int *x, int *y);
void movesquare(int *x, int *y);
void swap(int *x, int *y);
int ifadjacent(char squaregrid[][ARRAYSIZE], int *x, int *y);

int main(void) {
    /*Co-ordinates used to track squares*/
    int x = EDGE, y = EDGE;
    char squaregrid [ARRAYSIZE] [ARRAYSIZE];
    int i;
    NCURS_Simplewin grid;

    srand(time(NULL));

    Neill_NCURS_Init(&grid);
    Neill_NCURS_CharStyle(&grid, " ", COLOR_BLACK, COLOR_BLACK, A_NORMAL);
    Neill_NCURS_CharStyle(&grid, "#", COLOR_RED, COLOR_RED, A_NORMAL);

    fillarray(squaregrid);
    /*Sets the central starting square*/
    squaregrid[ARRAYSIZE/2][ARRAYSIZE/2] = '#';

    for (i = 0; i < MAXSQUARES; i++) {
        createsquare(squaregrid, &x, &y);
        while (ifadjacent(squaregrid, &x, &y) == 0) {
            movesquare(&x, &y);
            swap(&x, &y);
        }
        squaregrid[x][y] = '#';
        Neill_NCURS_PrintArray(&squaregrid[0][0], ARRAYSIZE, ARRAYSIZE, &grid);
        Neill_NCURS_Delay(DELAYTIME);
        }

    do {
    Neill_NCURS_Events(&grid);
    } while(!grid.finished);

    atexit(Neill_NCURS_Done);
    exit(EXIT_SUCCESS);

    return 0;
}

void fillarray(char squaregrid[][ARRAYSIZE])
{
    int i;
    int j;
    for (i = 0; i < ARRAYSIZE; i++) {
        for (j = 0; j < ARRAYSIZE; j++) {
            squaregrid[j][i] = ' ';
        }
    }
}

void createsquare(char squaregrid[][ARRAYSIZE], int *x, int *y)
{
    /* The loop here ensures we do not create a starting
     position on top of a placed square */
    do {
        int side = rand()%DIRECTION;
        int cell = rand()%ARRAYSIZE;
        if (side == north) {
            *x = cell;
            *y = EDGE;
        }
        if (side == south) {
            *x = cell;
            *y = OPPOSINGEDGE;
        }
        if (side == west) {
            *x = EDGE;
            *y = cell;
        }
        if (side == east) {
            *x = OPPOSINGEDGE;
            *y = cell;
        }
    } while (squaregrid[*x][*y] == '#');
}

void movesquare(int *x, int *y)
{
    int direction = rand()%DIRECTION;
    switch (direction) {
        case north:
            (*y)--;
            break;
        case south:
            (*y)++;
            break;
        case west:
            (*x)--;
            break;
        case east:
            (*x)++;
            break;
        default :
            Neill_NCURS_Warning("Random number not within expected range!");
            break;
    }
}

void swap(int *x, int *y)
{
    if (*x < EDGE) {
        *x = OPPOSINGEDGE;
    }
    if (*x >= ARRAYSIZE) {
        *x = EDGE;
    }
    if (*y < EDGE) {
        *y = OPPOSINGEDGE;
    }
    if (*y >= ARRAYSIZE) {
        *y = EDGE;
    }
}

int ifadjacent(char squaregrid[][ARRAYSIZE], int *x, int *y)
{
    int northcheck = *y - CELLSHIFT;
    int southcheck = *y + CELLSHIFT;
    int westcheck  = *x - CELLSHIFT;
    int eastcheck  = *x + CELLSHIFT;

    /* Ensures the adjacency checks are toroidal */
    swap(x, &northcheck);
    swap(x, &southcheck);
    swap(&westcheck, y);
    swap(&eastcheck, y);

    if (squaregrid[*x][northcheck] == '#' ||
        squaregrid[*x][southcheck] == '#' ||
        squaregrid[westcheck][*y]  == '#' ||
        squaregrid[eastcheck][*y]  == '#') {
        return 1;
    }
    return 0;
}
