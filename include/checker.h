#pragma once

#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"

bool check_solvable_by_houses(Cell* houses[9][9]) {
    for_ij_09() {
        int popcount = popcount(houses[i][j]->cand);
        if (!houses[i][j]->value && !popcount) return False; // Cell has no candidates

        bin digit_mask = base2(j + 1);
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
        bin possible = ALL_F;
        bin vals = ALL_F;
        for_range_09(j) {
            if (vals & houses[i][j]->value) {
                if (f.stop_on_incorrect) {
                    print(
                        "%s %d filled incorrectly on step %d: more than one %d" NL,
                        obj_type_str_capt[o], i+1, f.total_steps, base10(houses[i][j]->value)
                    );
                    sleep(2);
                }
                return False; 
            }
            vals |= houses[i][j]->value;
            possible |= (houses[i][j]->value | houses[i][j]->cand);
        }
        if (possible != ALL_T) {
            if (f.stop_on_incorrect) {
                print(
                    "%s %d filled incorrectly on step %d: no possible placement of %d" NL,
                    obj_type_str_capt[o], i+1, f.total_steps, base10(get_lowest_cand(~possible))
                );
                sleep(2);
            }
            return False;
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