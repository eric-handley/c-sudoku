#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"

#define CORRECT_SUM 45

bool check_solvable_by_houses(Cell* houses[9][9]) {
    for_range_09(i) {
        bin possible = ALL_F;
            for_range_09(j) {
                possible |= houses[i][j]->cand;
                possible |= houses[i][j]->value;
            }
        if (possible != ALL_T) return False;
    }
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
            vals |= houses[i][j]->value; 
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