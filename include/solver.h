#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"

void remove_candidates_by_house(Cell* houses[9][9]) {
    for_ij_09() {
        if (!houses[i][j]->value) continue; // Cell is a zero
        for_range_09(k) {
            houses[i][k]->cand &= ~(houses[i][j]->value);
        }
    }}
}

void remove_candidates(Sudoku* s) {
    remove_candidates_by_house(s->rows);
    remove_candidates_by_house(s->cols);
    remove_candidates_by_house(s->boxes);
}

int fill_single_candidates(Sudoku* s, bool do_visible) {
    int total_filled = 0;
    for_ij_09() {
        Cell* c = s->boxes[i][j];
        if (c->value) {
            total_filled++;
            continue;
        }
        if (__builtin_popcount(c->cand) == 1 || c->value) {
            c->value = c->cand;
            c->cand = ALL_F;
            if (do_visible) {
                system("clear");
                print_sudoku(s);
                sleep(0.1);
            }
            total_filled++;
        }
    }}
    return total_filled;
}

bool solve(Sudoku* s, bool do_visible) {
    int total_filled = 1, total_filled_last = 0;
    while (total_filled < 81) {
        total_filled_last = total_filled;
        total_filled = 0;
        remove_candidates(s);
        total_filled = fill_single_candidates(s, do_visible);

        if (total_filled_last == total_filled) return False;
    }
    return True;
}