#pragma once

#include "binaryrepr.h"
#include "bool.h"
#include "printing.h"
#include "flags.h"

#define box_idx(x, y)         ((x) + 3 * (y))
#define x_y_to_box(x, y)      box_idx((x) / 3, (y) / 3)
#define x_y_to_box_cell(x, y) box_idx((x) % 3, (y) % 3)

#define or_col_cand(box, col_idx)  box[(col_idx)]->cand | box[(col_idx) + 3]->cand | box[(col_idx) + 6]->cand
#define or_row_cand(box, row_idx)  box[3 * (row_idx)]->cand | box[3 * (row_idx) + 1]->cand | box[3 * (row_idx) + 2]->cand

#define candidate_string_arr(cand) \
    { \
        (cand & base2(1)) ? "1" : CELL_PAD, \
        (cand & base2(2)) ? "2" : CELL_PAD, \
        (cand & base2(3)) ? "3" : CELL_PAD, \
        (cand & base2(4)) ? "4" : CELL_PAD, \
        (cand & base2(5)) ? "5" : CELL_PAD, \
        (cand & base2(6)) ? "6" : CELL_PAD, \
        (cand & base2(7)) ? "7" : CELL_PAD, \
        (cand & base2(8)) ? "8" : CELL_PAD, \
        (cand & base2(9)) ? "9" : CELL_PAD  \
    }

#define bin u_int16_t

typedef struct Cell {
    bin value;
    bin cand;
    int x;
    int y;
} Cell;

typedef struct Sudoku {
    Cell  cells[9][9];
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
const str obj_type_str_capt[6] = {"Row", "Column", "Box", "Cell", "Value", "Candidate"};

typedef struct {
    Cell cells[9][9];
} SudokuState;

void save_state(Sudoku* s, SudokuState* state) {
    memcpy(state->cells, s->cells, sizeof(s->cells));
}

void restore_state(Sudoku* s, SudokuState* state) {
    memcpy(s->cells, state->cells, sizeof(s->cells));
}

void print_sudoku_cand(Sudoku* s);

void print_sudoku(Sudoku* s) {
    if (f.display_candidates) {
        print_sudoku_cand(s);
        return;
    }
    print(T_BORDER NL);
    
    for_range_09(y) {
        print(THICK_V_BAR);
        for_range_09(x) {
            int val = base10(s->cells[y][x].value);
            print(CELL_PAD);
            if (val) print("%d", val);
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

void print_sudoku_cand(Sudoku* s) {
    print(T_BORDER_CAND NL);
    
    for_range_09(y) {
        for_range_03(cell_row) {
            print(THICK_V_BAR);
            for_range_09(x) {
                int cell_val     = base10(s->cells[y][x].value);
                str cell_cand[9] = candidate_string_arr(s->cells[y][x].cand);

                if (cell_val) {
                    if (cell_row == 1) print("   " GRN "%d" RST "   ", cell_val);
                    else print("       ");
                } else {
                    print(
                        RED " %s %s %s " RST, 
                        cell_cand[0 + 3 * cell_row],
                        cell_cand[1 + 3 * cell_row],
                        cell_cand[2 + 3 * cell_row]
                    );
                }

                if (x < 8 && (x + 1) % 3 != 0) print(THIN_V_BAR);
                else print(THICK_V_BAR);
            }
            print(NL);
        }
        if (y < 8) {
            if ((y + 1) % 3 != 0) print(THIN_ROW_SEP_CAND);
            else print(THICK_ROW_SEP_CAND);
            print(NL);
        } 
    } 

    print(B_BORDER_CAND NL);
}