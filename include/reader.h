#include <stdio.h>
#include "sudoku.h"
#include "bool.h"

#define BUF_SIZE 65536
#define PUZZLE_LINE_SIZE_BYTES 82

int count_lines(FILE* file)
{
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUF_SIZE, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    return counter;
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