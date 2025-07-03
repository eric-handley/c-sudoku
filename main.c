#include "include/printing.h"
#include "include/reader.h"
#include "include/binaryrepr.h"
#include "include/sudoku.h"
#include "include/bool.h"
#include "include/solver.h"
#include "include/checker.h"

int main(int argc, char* argv[]) {
    
    Sudoku* puzzle = (Sudoku*)malloc(sizeof(Sudoku));
    
    str filename;
    if (argc == 2) filename = argv[1];
    else filename = "data/puzzles3_magictour_top1465";

    FILE* puzzlefile = fopen(filename, "r");

    float total_solved = 0;
    float total_incorrect = 0;
    float total_unsolvable = 0;
    int total_puzzles = count_lines(puzzlefile);

    bool do_visible = False;
    
    fclose(puzzlefile);
    clock_t start = clock();

    for_range(0, total_puzzles, i) {
        read_puzzle(filename, i, puzzle);
        remove_candidates(puzzle);
        
        if (!is_solvable(puzzle)) {
            total_unsolvable++;
            continue;
        }

        if(solve(puzzle, do_visible)) {
            if (double_check(puzzle)) total_solved++;
            else total_incorrect++;
        }
    }

    clock_t end = clock();    
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    double puzzles_per_sec = total_puzzles / cpu_time_used;
    double micro_sec_per_puzzle = (cpu_time_used * s_TO_μs) / total_puzzles;

    total_puzzles -= total_unsolvable;
    int total_cant_solve = (int)(total_puzzles-total_solved);
    float err_rate = (total_incorrect / total_puzzles) * 100;
    float solve_rate = (total_solved / total_puzzles) * 100;

    char* was_checked_str[2] = {RED "Not checked" RST, GRN "Checked" RST};
    char* colour_conditional[2] = {GRN, RED};

    print(
        NL H_LINE_THICK NL
        "[%s]:" NL
        " - Solved " GRN "%d" RST " out of %d solvable puzzles" NL
        " - %s%d" RST " puzzles could not be solved" NL
        " - %s%d" RST " puzzles were mathmatically unsolvable (%s)" NL
        " - %s%d" RST " puzzles were completed incorrectly (%s)" NL
        " - Error rate: %s%.3f%%" RST NL
        " - Solve rate: %s%.3f%%" RST NL
        "Total execution time: %.2f seconds (%.1f puz/s, %.1f μs/puz)" NL
        H_LINE_THICK NL NL, 
        filename,
        (int)total_solved, 
        (int)total_puzzles, 
        colour_conditional[total_cant_solve > 0], total_cant_solve, 
        colour_conditional[(int)total_unsolvable > 0], (int)total_unsolvable, was_checked_str[True],
        colour_conditional[(int)total_incorrect > 0], (int)total_incorrect, was_checked_str[True],
        colour_conditional[err_rate > 0], err_rate,
        colour_conditional[solve_rate < 100], solve_rate,
        cpu_time_used, puzzles_per_sec, micro_sec_per_puzzle
    );

    return 0;
}