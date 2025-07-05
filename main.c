#include "include/printing.h"
#include "include/reader.h"
#include "include/binaryrepr.h"
#include "include/sudoku.h"
#include "include/bool.h"
#include "include/solver.h"
#include "include/checker.h"
#include "include/flags.h"

int main(int argc, char* argv[]) {
    Sudoku* s = (Sudoku*)malloc(sizeof(Sudoku));
    
    str filename;
    if (argc == 2) filename = argv[1];
    else filename = f.puzzle_filename;

    int start_range = (f.one_puzzle_only == -1) ? 0 : f.one_puzzle_only;
    int end_range = (f.one_puzzle_only == -1) ? count_lines(filename) : (f.one_puzzle_only + 1);

    int total_solved = 0, total_incorrect = 0, 
        total_unsolvable = 0, total_puzzles = end_range - start_range;

    clock_t t_start = clock();
    
    for_range(start_range, end_range, i) {
        read_puzzle(filename, i, s);
        remove_candidates(s);
        
        if (f.check_solvable && !is_solvable(s)) {
            total_unsolvable++;
            continue;
        }

        if(solve(s, 0)) {
            if (!f.check_incorrect || double_check(s)) total_solved++;
            else {
                total_incorrect++;
                if (f.stop_on_incorrect) {
                    print_sudoku(s);
                    print("Puzzle %d solved incorrectly" NL, i);
                    exit(1);
                }
            }
        } else if (f.stop_on_cant_solve) {
            print_sudoku(s);
            print("Puzzle %d could not be solved" NL, i);
            exit(1);
        }
    }

    clock_t t_end = clock();    
    double cpu_time_used = ((double)(t_end - t_start)) / CLOCKS_PER_SEC;
    double puzzles_per_sec = ((double)total_puzzles) / cpu_time_used;
    double micro_sec_per_puzzle = (cpu_time_used * s_TO_μs) / ((double)total_puzzles);
    
    double steps_per_sec = ((double)f.total_steps) / cpu_time_used;
    double micro_sec_per_step = (cpu_time_used * s_TO_μs) / ((double)f.total_steps);

    total_puzzles -= total_unsolvable;
    int total_cant_solve = total_puzzles - total_solved;
    double err_rate = ((double)total_incorrect) / ((double)total_puzzles) * 100;
    double solve_rate = ((double)total_solved) / ((double)total_puzzles) * 100;

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
        "Total steps:          %d (%.0f step/s, %.2f μs/step)" NL
        H_LINE_THICK NL NL,
        filename,
        total_solved, 
        total_puzzles, 

        colour_conditional[total_cant_solve > 0], total_cant_solve, 
        colour_conditional[total_unsolvable > 0], total_unsolvable, was_checked_str[f.check_solvable],
        colour_conditional[total_incorrect > 0],  total_incorrect,  was_checked_str[f.check_incorrect],
        colour_conditional[err_rate > 0],         err_rate,
        colour_conditional[solve_rate < 100],     solve_rate,
        
        cpu_time_used, puzzles_per_sec, micro_sec_per_puzzle,
        f.total_steps, steps_per_sec,   micro_sec_per_step
    );
    return 0;
}