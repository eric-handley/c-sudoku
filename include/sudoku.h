#pragma once

#include "binaryrepr.h"
#include "bool.h"
#include "printing.h"

#define box_idx(x, y)         ((x) + 3 * (y))
#define x_y_to_box(x, y)      box_idx((x) / 3, (y) / 3)
#define x_y_to_box_cell(x, y) box_idx((x) % 3, (y) % 3)

#define bin int

typedef struct Cell {
    bin value;
    bin cand;
    int x;
    int y;
} Cell;

typedef struct Sudoku {
    Cell  cells[9][9][4];
    Cell* rows [9][9];
    Cell* cols [9][9];
    Cell* boxes[9][9];
} Sudoku;

typedef enum obj_type {
    ROW,
    COL,
    BOX,
    CELL,
    VALUE,
    CAND
} obj_type;

const str obj_type_str[6] = {"row", "column", "box", "cell", "value", "candidate"};

void print_sudoku(Sudoku* s) {
    print(T_BORDER NL);
    
    for_range_09(y) {
        print(THICK_V_BAR);
        for_range_09(x) {
            int val = D(s->cells[y][x]->value);
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
