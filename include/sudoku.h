#pragma once

#include <pthread.h>
#include <stdlib.h>
#include "bool.h"
#include "printing.h"

typedef enum StructureType {
    COL,
    ROW,
    NINTH,
    CELL,
    SUDOKU
} StructureType;

typedef struct Cell {
    int value;
    int x;
    int y;
    bool given;
    bool empty;
    bool might_be[9];
} Cell;

typedef struct Ninth {
    Cell* cells[3][3];
    Cell* cells_lin[9];
    pthread_t thread;
} Ninth;

typedef struct Row {
    Cell* cells[9];
} Row;

typedef struct Col {
    Cell* cells[9];
} Col;

typedef struct Sudoku
{
    Row rows[9];
    Col cols[9];
    Ninth ninths[3][3];
    bool is_solved;
} Sudoku;

void print_sudoku(Sudoku* s) {
    print(T_BORDER NL);
    
    for (int y = 0; y < 9; y++) {
        print(THICK_V_BAR);
        for (int x = 0; x < 9; x++) {
            // int val = s->cols[x].cells[y]->value;
            int val = s->rows[y].cells[x]->value;
            print(CELL_PAD);
            if (val) printf("%d", val);
            else print (CELL_PAD);
            print(CELL_PAD);
            if (x < 8 && (x + 1) % 3 != 0) print(THIN_V_BAR);
            else print(THICK_V_BAR);
        }
        if (y < 8) {
            print(NL);
            if ((y + 1) % 3 != 0) print(THIN_ROW_SEP);
            else print(THICK_ROW_SEP);
            print(NL);
        } 
        else print(NL);
    } 

    print(B_BORDER NL);
}
