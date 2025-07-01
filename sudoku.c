#include "sudoku.h"
#include "countlines.h"

#define PUZZLE_LINE_SIZE_BYTES 82
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

int main(int argc, char* argv[]) {
    Sudoku* puzzle = (Sudoku*)malloc(sizeof(Sudoku));
    
    char* filename;
    if (argc == 2) filename = argv[1];
    else filename = "data/puzzles7_serg_benchmark";

    FILE* f = fopen(filename, "r");
    
    float total_solved = 0;
    float total_incorrect = 0;
    float total_puzzles = count_lines(f);
    bool do_visible = False;

    fclose(f);

    for (int i = 0; i < total_puzzles; i++) {
        // int i = 0;
        read_puzzle(filename, i, puzzle);
        // print_sudoku(puzzle);
        bool could_solve = solve_sudoku(puzzle, do_visible);
    
        if (could_solve) {
            // print("Solved puzzle %d." NL, i);
            if (double_check(puzzle)) {
                total_solved++;
            } else {
                total_incorrect++;
                // print("WARN: Incorrectly completed puzzle %d." NL, i);
            }
        }
        // else print("Could not solve puzzle %d." NL, i);
        if (do_visible) sleep(2);
    }

    print(
        "[%s]:" NL
        " - Solved %d out of %d puzzles" NL
        " - %d puzzles could not be solved" NL
        " - %d puzzles were completed incorrectly" NL
        " - Solve rate: %.3f%%" NL, 
        filename,
        (int)total_solved, 
        (int)total_puzzles, 
        (int)(total_puzzles-total_solved), 
        (int)total_incorrect,
        (total_solved / total_puzzles) * 100
    );

    return 0;
}

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
            run_check(s, do_visible, SINGLE_POSSIBLE,    &best_n_filled) ||
            run_check(s, do_visible, EXCLUSIVE_POSSIBLE, &best_n_filled) ||
            run_check(s, do_visible, COLUMN_ELIMINATION, &best_n_filled) ||
            run_check(s, do_visible, ROW_ELIMINATION,    &best_n_filled)
        ) continue;
        
        return False;
    }
    return True;
}

void link_ninths(Sudoku* s) {
    for (int i = 0; i < 81; i++) {
        int x = (i % 9) % 3, y = (i % 9) / 3;
        Ninth* n = &s->ninths[y][x];
        int row_idx[3] = {3*y, 3*y+1, 3*y+2};
        int col_idx[3] = {3*x, 3*x+1, 3*x+2};

        x = (i / 9) % 3, y = (i / 9) / 3;
        int sx = col_idx[y];
        int sy = row_idx[x];
        n->cells[x][y] = s->cols[sx].cells[sy];
        n->cells_lin[(3*x) + y] = s->cols[sx].cells[sy];
    }
}

void read_puzzle(char* filename, int line_number, Sudoku* output) {
    FILE* puzzle_file = fopen(filename, "r");
    fseek(puzzle_file, line_number * PUZZLE_LINE_SIZE_BYTES, 0);
    output->is_solved = False;

    int x = 0, y = 0;
    for (char c = getc(puzzle_file); c != '\n'; c = getc(puzzle_file)) {
        if (c == '.') c = '0';
        int val = ((int)c - 48);

        Cell* new_cell = (Cell*)malloc(sizeof(Cell));
        *new_cell = (Cell){
            .value = val,
            .x = x,
            .y = y,
            .given = (val != 0),
            .empty = (val == 0),
            .might_be = {1,1,1,1,1,1,1,1,1}
        };

        output->cols[x].cells[y] = new_cell;
        output->rows[y].cells[x] = new_cell;

        x++;
        if (x == 9) {
            x = 0;
            y++;
        }
    }

    fclose(puzzle_file);

    link_ninths(output);
}