#include "main.h"

int main(int argc, char* argv[]) {
    Sudoku* puzzle = (Sudoku*)malloc(sizeof(Sudoku));
    
    char* filename;
    if (argc == 2) filename = argv[1];
    else filename = "data/puzzles8_gen_puzzles";

    FILE* f = fopen(filename, "r");
    
    float total_solved = 0;
    float total_incorrect = 0;
    float total_puzzles = count_lines(f);
    bool do_visible = True;

    fclose(f);

    // for (int i = 0; i < total_puzzles; i++) {
        int i = 8;
        read_puzzle(filename, i, puzzle);
        print_sudoku(puzzle);
        bool could_solve = solve_sudoku(puzzle, do_visible);
    
        if (could_solve) {
            // print("Solved puzzle %d." NL, i);
            if (double_check(puzzle)) {
                total_solved++;
            } else {
                total_incorrect++;
                print("WARN: Incorrectly completed puzzle %d." NL, i);
                exit(1);
            }
        }
        // else print("Could not solve puzzle %d." NL, i);
        if (do_visible) sleep(2);
    // }

    print(
        "[%s]:" NL
        " - Solved %d out of %d puzzles" NL
        " - %d puzzles could not be solved" NL
        " - %d puzzles were completed incorrectly" NL
        " - Error rate: %.3f%%" NL
        " - Solve rate: %.3f%%" NL, 
        filename,
        (int)total_solved, 
        (int)total_puzzles, 
        (int)(total_puzzles-total_solved), 
        (int)total_incorrect,
        (total_incorrect / total_puzzles) * 100,
        (total_solved / total_puzzles) * 100
    );

    return 0;
}
