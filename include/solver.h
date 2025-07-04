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

bin exclusive_box_col_row_cand(Cell** box, int idx, obj_type row_or_col) {
    bin cand = (row_or_col == COL) ? or_col_cand(box, idx) : or_row_cand(box, idx); // All candidates for col/row i
    for_range_03(i) {
        if (idx == i) continue;
        cand &= ~((row_or_col == COL) ? or_col_cand(box, i) : or_row_cand(box, i)); // Minus all candidates for other col/row
    }
    return cand;
}

bool remove_cand_from_house_if_not_in_box(Sudoku* s, bin cand, Cell** house, int box_idx) {
    bool work = False;
    for_range_09(i) {
        Cell* c = house[i];
        if (x_y_to_box(c->x, c->y) == box_idx) continue;
        if (c->cand & cand) {
            c->cand &= ~cand;
            work = True;
        }
    }
    return work;
}

bool row_column_elimination(Sudoku* s) {
    bool work = False;
    bin cand;
    for_range_09(i) {
        Cell** box = s->boxes[i];
        for_range_03(j) {
            cand = exclusive_box_col_row_cand(box, j, COL); // cand has any exclusive candidates for jth column of box i
            if (cand) { // Remove cand from any cells in this column but /not/ in this box
                int col_idx = box[j]->x;
                work |= remove_cand_from_house_if_not_in_box(s, cand, s->cols[col_idx], i);
            }
            cand = exclusive_box_col_row_cand(box, j, ROW); // cand has any exclusive candidates for jth row of box i
            if (cand) { // Remove cand from any cells in this row but /not/ in this box
                int row_idx = box[j * 3]->y;
                work |= remove_cand_from_house_if_not_in_box(s, cand, s->rows[row_idx], i);
            }
        }
    }
    return work;
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
            fill_single_candidates(s)    ||
            fill_exclusive_candidates(s) ||
            row_column_elimination(s)
        ) continue;
        return False;
    }
    return True;
}