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

void generic_fill(Cell* cells[9]) {
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
        Row* r = &s->rows[i];
        generic_fill(r->cells);

        Col* c = &s->cols[i];
        generic_fill(c->cells);

        Ninth* n = &s->ninths[i % 3][i / 3];
        generic_fill(n->cells_lin);
    }
}

bool solve_sudoku(Sudoku* s, bool do_visible) {
    fill_possibilities(s);
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