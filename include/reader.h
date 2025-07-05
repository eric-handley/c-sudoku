#pragma once

#include <stdio.h>
#include "binaryrepr.h"
#include "bool.h"
#include "sudoku.h"

#define BUF_SIZE 65536
#define PUZZLE_LINE_SIZE_BYTES 82

int count_lines(str filename)
{
    FILE* puzzle_file = fopen(filename, "r");
    if (puzzle_file == NULL) {
        perror("reader.h: count_lines()");
        exit(1);
    }
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUF_SIZE, puzzle_file);
        if (ferror(puzzle_file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(puzzle_file))
            break;
    }
    fclose(puzzle_file);
    return counter;
}

void link_boxes (Sudoku* s) {
    for_ij_09() {
        s->boxes[x_y_to_box(i,j)][x_y_to_box_cell(i,j)] = &s->cells[j][i];
    }}
}

void read_puzzle(str filename, int line_number, Sudoku* output) {
    FILE* puzzle_file = fopen(filename, "r");
    if (puzzle_file == NULL) {
        perror("reader.h: read_puzzle()");
        exit(1);
    }
    fseek(puzzle_file, line_number * PUZZLE_LINE_SIZE_BYTES, 0);

    int x = 0, y = 0;
    for (char c = getc(puzzle_file); c != '\n'; c = getc(puzzle_file)) {
        if (c == '.') {
            output->cells[y][x].value = 0;
            output->cells[y][x].cand  = ALL_T;
        } else {
            output->cells[y][x].value = base2(((int)c - 48));
            output->cells[y][x].cand  = ALL_F;
        }
        
        output->cells[y][x].x = x;
        output->cells[y][x].y = y;
        output->cells[y][x].marker = 0;

        output->rows[y][x] = &output->cells[y][x];
        output->cols[x][y] = &output->cells[y][x];

        x++;
        if (x == 9) {
            x = 0;
            y++;
        }
    }

    fclose(puzzle_file);

    link_boxes(output);
}