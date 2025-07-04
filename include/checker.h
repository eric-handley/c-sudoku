#pragma once

#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"

bool check_solvable_by_houses(Cell* houses[9][9]) {
    for_ij_09() {
        int popcount = __builtin_popcount(houses[i][j]->cand);
        if (!houses[i][j]->value && !popcount) return False; // Cell has no candidates

        bin digit_mask = B(j + 1);
        int possible_positions = 0;
        for_range_09(k) {
            Cell* c = houses[i][k];
            if ((c->value | c->cand) & digit_mask) possible_positions++;
        }
        if (!possible_positions) return False; // Digit j cannot be placed in any cell
    }}
    return True;
}

bool is_solvable(Sudoku* s_candidates_filled) {
    if (
        check_solvable_by_houses(s_candidates_filled->rows)  &&
        check_solvable_by_houses(s_candidates_filled->cols)  &&
        check_solvable_by_houses(s_candidates_filled->boxes)
    ) return True;
    return False;
}

bool double_check_houses(Cell* houses[9][9]) {
    for_range_09(i) {
        bin vals = ALL_F;
        for_range_09(j) {
            vals ^= houses[i][j]->value; 
        }
        if (vals != ALL_T) return False;
    }
    return True;
}

bool double_check(Sudoku* s) {
    if (
        double_check_houses(s->rows)  &&
        double_check_houses(s->cols)  &&
        double_check_houses(s->boxes)
    ) return True;
    return False;
}

bool double_check_step_houses(Cell* houses[9][9], obj_type o) {
    for_range_09(i) {
        bin vals = ALL_F;
        for_range_09(j) {
            if (vals & houses[i][j]->value) {
                print(
                    "%dth %s filled incorrectly on step %d: more than one %d" NL,
                    i, obj_type_str[o], f.total_visible_steps, D(houses[i][j]->value)
                );
                return False; 
            }
            vals |= houses[i][j]->value;
        }
    }
    return True;
}

bool double_check_step(Sudoku* s) {
    if (
        double_check_step_houses(s->rows,  ROW) &&
        double_check_step_houses(s->cols,  COL) &&
        double_check_step_houses(s->boxes, BOX)
    ) return True;
    return False;
}