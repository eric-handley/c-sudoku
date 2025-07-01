#include "sudoku.h"
#include "bool.h"

void fill(Cell* cells[9]) {
    bool was_found[9] = {};
    for (int i = 0; i < 9; i++) {
        int val = cells[i]->value;
        int val_idx = val - 1;
        was_found[val_idx] = was_found[val_idx] || val != 0;
    }
    for (int i = 0; i < 9; i++) {
        Cell* c = cells[i];
        if (c->given) continue;
        for (int j = 0; j < 9; j++) {
            c->might_be[j] = c->might_be[j] && !was_found[j];
        }
    }
}

void fill_possibilities(Sudoku* s) {
    for (int i = 0; i < 9; i++) {
        Ninth* n = &s->ninths[i % 3][i / 3];
        fill(n->cells_lin);

        Row* r = &s->rows[i];
        fill(r->cells);

        Col* c = &s->cols[i];
        fill(c->cells);
    }
}

bool check_single_possibilities(Cell* c) {
    bool can_set = False;
    int can_set_to = 1;
    for (int i = 0; i < 9; i++) {
        if (c->might_be[i]) {
            if (!can_set) {
                can_set = True;
                can_set_to += i;
            } else {
                return False;
            }
        }
    }

    c->value = can_set_to;
    c->empty = False;
    return True;
}

bool check_exclusive_possibilities(Cell** cells, int idx) {
    Cell* c = cells[idx];
    
    for (int i = 0; i < 9; i++) {
        bool try_next = False;
        if(c->might_be[i]) {

            for (int j = 0; j < 9; j++) {
                if (j == idx) continue;
                if (!cells[j]->empty) continue;
                if (cells[j]->might_be[i]) try_next = True;
            }

            if (try_next) continue;

            c->value = i + 1;
            c->empty = False;
            return True;
        }
    }

    return False;
}

bool check_elimination(Cell* c, Cell** cells, bool do_columns) {
    int ninth;
    bool did_work = False;
    if (do_columns) ninth = c->y / 3;
    else ninth = c->x / 3;
    
    for (int i = 0; i < 9; i++) {
        if(c->might_be[i]) {
            int total_found = 0;

            for (int j = 0; j < 9; j++) {
                Cell* check_cell = cells[j];
                if (j / 3 == ninth) continue;
                if (!check_cell->empty) continue;
                if (check_cell->might_be[i]) total_found++;

                c->might_be[i] = !(total_found == 2);
                did_work = total_found == 2;
            }

        }
    }
    
    return did_work;
}

typedef enum CheckType {
    SINGLE_POSSIBLE,
    EXCLUSIVE_POSSIBLE,
    COLUMN_ELIMINATION,
    ROW_ELIMINATION
} CheckType;

bool run_check(Sudoku* s, bool do_visible, CheckType check, int* best_n_filled) {
    int total_filled = 0, total_work = 0;
    bool work_done = False;
    for (int i = 0; i < 9; i++) {
        Cell** cells = s->ninths[i / 3][i % 3].cells_lin;

        for (int j = 0; j < 9; j++) {
            Cell* c = cells[j];
            if(!c->empty || c->given) total_filled++;
            else {
                bool was_filled = False;

                switch (check) {
                    case SINGLE_POSSIBLE:
                        was_filled = check_single_possibilities(c);
                        break;
                    
                    case EXCLUSIVE_POSSIBLE:
                        was_filled = check_exclusive_possibilities(cells, j);
                        break;

                    case COLUMN_ELIMINATION:
                        work_done = check_elimination(c, s->cols[c->x].cells, True);
                        break;
                    
                    case ROW_ELIMINATION:
                        work_done = check_elimination(c, s->rows[c->y].cells, False);

                    default:
                        break;
                }

                total_work += work_done;
                total_filled += was_filled;
                if (do_visible && was_filled) {
                    system("clear");
                    print_sudoku(s);
                    sleep(1);
                }
            }
        }
    }

    if(total_filled != *best_n_filled || total_work) {
        *best_n_filled = total_filled;
        return True;
    } 
    return False;
}

bool solve_sudoku(Sudoku* s, bool do_visible) {
    if (do_visible) {
        system("clear");
        print_sudoku(s);
        sleep(0.05);
    }

    int best_n_filled = 0;
    while (best_n_filled < 81) {
        fill_possibilities(s);
        
        if(
            run_check(s, do_visible, SINGLE_POSSIBLE,    &best_n_filled)
            // run_check(s, do_visible, EXCLUSIVE_POSSIBLE, &best_n_filled)
            // run_check(s, do_visible, COLUMN_ELIMINATION, &best_n_filled) ||
            // run_check(s, do_visible, ROW_ELIMINATION,    &best_n_filled)
        ) continue;
        
        return False;
    }
    return True;
}
