#include "sudoku.h"
#include "binaryrepr.h"
#include "bool.h"
#include "flags.h"
#include "checker.h"

bool step(Sudoku* s, int recurse_depth, bool do_recurse);
bool solve(Sudoku* s, int recurse_depth);

int count_filled(Sudoku* s) {
    int total = 0;
    for_ij_09() {
        total += (s->rows[i][j]->value > 0);
    }}
    return total;
}

void step_check_and_print(Sudoku* s) {
    if (f.display_solve) {
        system("clear");
        print_sudoku(s);
        sleep(f.display_step_time);
    };
    if(f.double_check_steps && !double_check_step(s)) exit(1);
}

bool remove_candidates_by_house(Cell* houses[9][9]) {
    bool work = False;
    for_ij_09() {
        if (!houses[i][j]->value) continue; // Cell is a zero
        for_range_09(k) {
            bin cand = houses[i][k]->cand, val = houses[i][j]->value;
            if (cand & val) {
                minus_cand(houses[i][k]->cand, houses[i][j]->value);
                work = True;
            }
        }
    }}
    return work;
}

bool remove_candidates(Sudoku* s) {
    bool work = False;
    work |= remove_candidates_by_house(s->rows);
    work |= remove_candidates_by_house(s->cols);
    work |= remove_candidates_by_house(s->boxes);
    return work;
}

bool fill_single_candidates(Sudoku* s) {
    bool new = False;
    for_ij_09() {
        Cell* c = s->boxes[i][j];
        if (c->value) continue;
        if (popcount(c->cand) == 1) {
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
        bin other_cand = (row_or_col == COL) ? or_col_cand(box, i) : or_row_cand(box, i);
        minus_cand(cand, other_cand); // Minus all candidates for other col/row
    }
    return cand;
}

bool remove_cand_from_house_if_not_in_box(Sudoku* s, bin cand, Cell** house, int box_idx) {
    bool work = False;
    for_range_09(i) {
        Cell* c = house[i];
        if (x_y_to_box(c->x, c->y) == box_idx) continue;
        if (c->cand & cand) {
            minus_cand(c->cand, cand);
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

bool find_kth_cell_with_n_candidates(Sudoku* s, int k, int n, Cell* out) {
    int l = 0;
    for_ij_09() {
        Cell* c = s->boxes[i][j];
        if (popcount(c->cand) == n) {
            if (l == k) {
                *out = *c;
                return True;
            }
            else l++;
        }
    }}
    return False;
}

bool solve_recursive(Sudoku* s, int depth) {
    if (depth == f.recursion_limit) {
        return False;
    }
    Cell* gc = (Cell*)malloc(sizeof(Cell));
    SudokuState* original_state = (SudokuState*)malloc(sizeof(SudokuState));
    save_state(s, original_state);

    for_range(2, 10, i) {
        int k = 0;
        while(find_kth_cell_with_n_candidates(s, k++, i, gc)) {
            bin remaining_cand = gc->cand;
            while (remaining_cand) {
                bin guess = get_lowest_cand(remaining_cand);
                s->cells[gc->y][gc->x].value  = guess;
                s->cells[gc->y][gc->x].cand   = ALL_F;
                
                if (solve(s, depth + 1) && double_check(s) && count_filled(s) == 81) {
                    free(original_state);
                    free(gc);
                    return True;
                }

                restore_state(s, original_state);
                minus_cand(remaining_cand, guess);  // Remove this candidate and try the next
            }
        }
    }

    free(original_state);
    free(gc);
    return False;
}

bool step(Sudoku* s, int recurse_depth, bool do_recurse) {
    f.total_steps++;
    if (
        remove_candidates(s)         ||
        fill_single_candidates(s)    ||
        fill_exclusive_candidates(s) ||
        row_column_elimination(s)    ||
        (do_recurse && solve_recursive(s, recurse_depth))
    ) return True;
    return False;
}

bool solve(Sudoku* s, int recurse_depth) {
    while (count_filled(s) < 81) {
        if (step(s, recurse_depth, True)) continue;
        return False;
    }
    return True;
}