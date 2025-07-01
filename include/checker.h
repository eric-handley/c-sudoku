#include "sudoku.h"
#include "box.h"
#include "bool.h"

#define CORRECT_SUM 45

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
