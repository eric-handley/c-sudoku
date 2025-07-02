#include "sudoku.h"
#include "printing.h"
#include "bool.h"

#define CORRECT_SUM 45

bool check_solvable_by_cells(Cell** cells, bool print_errors) {
    bool found_might_be[9] = {};

    for (int j = 0; j < 9; j++) {
        Cell* c = cells[j];
        if (c->given) {
            found_might_be[c->value - 1] = True;
            continue;
        }
        for (int k = 0; k < 9; k++) {
            found_might_be[k] = found_might_be[k] || c->might_be[k];
        }
    }

    for (int j = 0; j < 9; j++) {
        if (!found_might_be[j]) {
            if (print_errors) print(RED "WARN" RST ": Checker - cannot fill any %d's" NL, j + 1);
            return False;
        }
    }

    return True;
}

bool is_solvable(Sudoku* s_possibilities_filled, bool print_errors) {
    for (int i = 0; i < 9; i++) {
        if(!check_solvable_by_cells(s_possibilities_filled->ninths[i % 3][i / 3].cells_lin, print_errors)) {
            if (print_errors) print(RED "WARN" RST ": Checker - ninth x: %d y: %d unsolvable" NL, i/3, i%3);
            return False;
        }
        if(!check_solvable_by_cells(s_possibilities_filled->rows[i].cells, print_errors)) {
            if (print_errors) print(RED "WARN" RST ": Checker - row %d unsolvable" NL, i);
            return False;
        }
        if(!check_solvable_by_cells(s_possibilities_filled->cols[i].cells, print_errors)) {
            if (print_errors) print(RED "WARN" RST ": Checker - column %d unsolvable" NL, i);
            return False;
        }
    }

    return True;
}

bool double_check(Sudoku* s) {
    for (int i = 0; i < 9; i++) {
        int sum = 0;
        Cell** row_cells = s->rows[i].cells;
        for (int j = 0; j < 9; j++) {
            sum += row_cells[j]->value;
        }
        if (sum != CORRECT_SUM) return False;
    }

    for (int i = 0; i < 9; i++) {
        int sum = 0;
        Cell** col_cells = s->cols[i].cells;
        for (int j = 0; j < 9; j++) {
            sum += col_cells[j]->value;
        }
        if (sum != CORRECT_SUM) return False;
    }

    return True;
}
