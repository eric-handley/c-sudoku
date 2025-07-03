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

bool fill_single_candidates(Sudoku* s, bool do_visible) {
    bool new = False;
    for_ij_09() {
        Cell* c = s->rows[i][j];
        if (c->value) continue;
        if (__builtin_popcount(c->cand) == 1) {
            c->value = c->cand;
            c->cand = ALL_F;
            if (do_visible) {
                system("clear");
                print_sudoku(s);
                sleep(0.1);
            }
            new = True;
        }
    }}
    return new;
}

bool fill_exclusive_candidates_by_house(Sudoku* s, Cell* houses[9][9], bool do_visible) {
    bool new = False;
    for_ij_09() {
        Cell* c = houses[i][j];
        bin possible = c->cand;
        if (!possible) continue;
        for_range_09(k) {
            if (k == j) continue;
            possible ^= (possible & houses[i][k]->cand);
        }
        if (possible) {
            c->value = possible;
            c->cand = ALL_F;
            if (do_visible) {
                system("clear");
                print_sudoku(s);
                sleep(0.1);
            }
            new = True;
        }
    }}
    return new;
}

bool fill_exclusive_candidates(Sudoku* s, bool do_visible) {
    if (
        fill_exclusive_candidates_by_house(s, s->boxes, do_visible) ||
        fill_exclusive_candidates_by_house(s, s->rows, do_visible)  ||
        fill_exclusive_candidates_by_house(s, s->cols, do_visible) 
    ) return True;
    return False;
}

int count_filled(Sudoku* s) {
    int total = 0;
    for_ij_09() {
        total += (s->rows[i][j]->value > 0);
    }}
    return total;
}

bool solve(Sudoku* s, bool do_visible) {
    while (count_filled(s) < 81) {
        remove_candidates(s);
        if (
            fill_single_candidates(s, do_visible) ||
            fill_exclusive_candidates(s, do_visible)
        ) continue;
        return False;
    }
    return True;
}