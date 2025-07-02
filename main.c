#include "main.h"

typedef struct flags {
    bool do_visible;
    bool check_solvable;
    bool check_incorrect;
    bool verbose;
    bool print_warnings;
    bool exit_on_unsolvable;
    bool exit_on_incorrect;
    bool exit_on_cant_solve;
} flags;

int main(int argc, char* argv[]) {
    Sudoku* puzzle = (Sudoku*)malloc(sizeof(Sudoku));
    
    char* filename;
    if (argc == 2) filename = argv[1];
    else filename = "data/puzzles0_kaggle";

    FILE* puzzlefile = fopen(filename, "r");
    
    float total_solved = 0;
    float total_incorrect = 0;
    float total_unsolvable = 0;
    float total_puzzles = count_lines(puzzlefile);

    flags f = {
        .do_visible         = False,
        .check_solvable     = False,
        .check_incorrect    = False,
        .verbose            = False,
        .print_warnings     = False,
        .exit_on_unsolvable = False,
        .exit_on_incorrect  = False,
        .exit_on_cant_solve = False
    };

    fclose(puzzlefile);

    clock_t start = clock();

    for (int i = 0; i < total_puzzles; i++) {
        read_puzzle(filename, i, puzzle);
        fill_possibilities(puzzle);

        if (f.check_solvable && !is_solvable(puzzle, (f.exit_on_unsolvable || f.verbose || f.print_warnings))) {
            if (f.verbose || f.print_warnings || f.exit_on_unsolvable) print(WRN "WARN" RST ": Puzzle %d is unsolvable" NL, i);
            if (f.exit_on_unsolvable) exit(1);
            total_unsolvable++;
        } else {
            if (solve_sudoku(puzzle, f.do_visible)) {
                if (f.check_incorrect && !double_check(puzzle)) {
                    if (f.exit_on_incorrect) print_sudoku(puzzle);
                    if (f.verbose || f.print_warnings || f.exit_on_incorrect) print(WRN "WARN" RST ": Incorrectly completed puzzle %d" NL, i);
                    if (f.exit_on_incorrect) exit(1);
                    total_incorrect++;
                } else {
                    if (f.verbose) print(GRN "DONE" RST ": Solved puzzle %d." NL, i);
                    total_solved++;
                }
            }
            else { 
                if (f.exit_on_cant_solve) print_sudoku(puzzle);
                if (f.verbose || f.exit_on_cant_solve) print(YEL "DONE" RST ": Could not solve puzzle %d." NL, i);
                if (f.exit_on_cant_solve) exit(1);
            }
        }
    
        if (f.do_visible) sleep(0.5);
    }

    clock_t end = clock();    
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    double puzzles_per_sec = total_puzzles / cpu_time_used;
    double micro_sec_per_puzzle = (cpu_time_used * s_TO_μs) / total_puzzles;
    
    total_puzzles -= total_unsolvable;
    int total_cant_solve = (int)(total_puzzles-total_solved);

    char* was_checked_str[2] = {RED "Not checked" RST, GRN "Checked" RST};
    char* colour_conditional[2] = {GRN, RED};

    print(
        NL H_LINE_THICK NL
        "[%s]:" NL
        " - Solved " GRN "%d" RST " out of %d solvable puzzles" NL
        " - %s%d" RST " puzzles could not be solved" NL
        " - %s%d" RST " puzzles were mathmatically unsolvable (%s)" NL
        " - %s%d" RST " puzzles were completed incorrectly (%s)" NL
        " - Error rate: " RED "%.3f%%" RST NL
        " - Solve rate: " GRN "%.3f%%" RST NL
        "Total execution time: %.2f seconds (%.1f puz/s, %.1f μs/puz)" NL
        H_LINE_THICK NL NL, 
        filename,
        (int)total_solved, 
        (int)total_puzzles, 
        colour_conditional[total_cant_solve > 0], total_cant_solve, 
        colour_conditional[(int)total_unsolvable > 0], (int)total_unsolvable, was_checked_str[f.check_solvable],
        colour_conditional[(int)total_incorrect > 0], (int)total_incorrect, was_checked_str[f.check_incorrect],
        (total_incorrect / total_puzzles) * 100,
        (total_solved / total_puzzles) * 100,
        cpu_time_used, puzzles_per_sec, micro_sec_per_puzzle
    );

    return 0;
}
