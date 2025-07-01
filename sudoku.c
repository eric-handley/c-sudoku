#include "sudoku.h"

#define PUZZLE_LINE_SIZE_BYTES 82

int main(int argc, char* argv[]) {
    Sudoku* puzzle = (Sudoku*)malloc(sizeof(Sudoku));
    
    read_puzzle("data/puzzles0_kaggle", 0, puzzle);
    system("clear");
    print_sudoku(puzzle);
    solve_sudoku(puzzle, True);

    return 0;
}

void fill_ninth_possibilities(Sudoku* s) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            Ninth* n = &s->ninths[x][y];
            bool was_found[9] = {};
            for (int i = 0; i < 9; i++) {
                int val = n->cells_lin[i]->value;
                int val_idx = val - 1;
                was_found[val_idx] = was_found[val_idx] || val != 0;
            }
            for (int i = 0; i < 9; i++) {
                Cell* c = n->cells_lin[i];
                if (c->given) continue;
                for (int j = 0; j < 9; j++) {
                    c->might_be[j] = c->might_be[j] && !was_found[j];
                }

            }
        }
    }
}

void fill_row_possibilities(Sudoku* s) {
    for (int y = 0; y < 9; y++) {
        Row* r = &s->rows[y];
        bool was_found[9] = {};
        for (int x = 0; x < 9; x++) {
            int val = r->cells[x]->value;
            int val_idx = val - 1;
            was_found[val_idx] = was_found[val_idx] || val != 0;
        }

        for (int x = 0; x < 9; x++) {
            Cell* c = r->cells[x];
            if (c->given) continue;
            for (int i = 0; i < 9; i++) {
                c->might_be[i] = c->might_be[i] && !was_found[i];
            }
        }
    }
}

void fill_col_possibilities(Sudoku* s) {
    for (int x = 0; x < 9; x++) {
        Col* col = &s->cols[x];
        bool was_found[9] = {};
        for (int y = 0; y < 9; y++) {
            int val = col->cells[y]->value;
            int val_idx = val - 1;
            was_found[val_idx] = was_found[val_idx] || val != 0;
        }

        for (int y = 0; y < 9; y++) {
            Cell* c = col->cells[y];
            if (c->given) continue;
            for (int i = 0; i < 9; i++) {
                c->might_be[i] = c->might_be[i] && !was_found[i];
            }
        }
    }
    return;
}

bool solve_sudoku(Sudoku* s, bool do_visible) {
    fill_ninth_possibilities(s);
    fill_row_possibilities(s);
    fill_col_possibilities(s);
    return False;
}

void link_ninths(Sudoku* s) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            Ninth* n = &s->ninths[y][x];
            int row_idx[3] = {3*y, 3*y+1, 3*y+2};
            int col_idx[3] = {3*x, 3*x+1, 3*x+2};

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int sx = col_idx[j];
                    int sy = row_idx[i];
                    n->cells[i][j] = s->cols[sx].cells[sy];
                    n->cells_lin[(3*i) + j] = s->cols[sx].cells[sy];
                }
            }
        }
    }
}

void read_puzzle(char* filename, int line_number, Sudoku* output) {
    FILE* puzzle_file = fopen(filename, "r");
    fseek(puzzle_file, line_number * PUZZLE_LINE_SIZE_BYTES, 0);

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

    link_ninths(output);
}