#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"
#include "flags.h"
#include "checker.h"

void step_check_and_print(Sudoku* s) {
    if (f.do_visible) {
        system("clear");
        print_sudoku(s);
        sleep(f.do_visible_step_time);
    };
    if(f.double_check_steps && !double_check_step(s)) exit(1);
    f.total_visible_steps++;
}

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

bool fill_single_candidates(Sudoku* s) {
    bool new = False;
    for_ij_09() {
        Cell* c = s->boxes[i][j];
        if (c->value) continue;
        if (__builtin_popcount(c->cand) == 1) {
            c->value = c->cand;
            c->cand = ALL_F;
            step_check_and_print(s);
            new = True;
        }
    }}
    return new;
}

bool fill_exclusive_candidates_by_house(Sudoku* s, Cell* houses[9][9]) {
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
            step_check_and_print(s);
            new = True;
        }
    }}
    return new;
}

bool fill_exclusive_candidates(Sudoku* s) {
    if (
        fill_exclusive_candidates_by_house(s, s->boxes) ||
        fill_exclusive_candidates_by_house(s, s->rows)  ||
        fill_exclusive_candidates_by_house(s, s->cols) 
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

bool solve(Sudoku* s) {
    while (count_filled(s) < 81) {
        remove_candidates(s);
        if (
            fill_single_candidates(s) ||
            fill_exclusive_candidates(s)
        ) continue;
        return False;
    }
    return True;
}