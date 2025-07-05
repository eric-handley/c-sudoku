#pragma once

#include "bool.h"
#include "printing.h"

typedef struct flags {
    str   puzzle_filename;
    bool  display_solve;
    bool  display_candidates;
    float display_step_time;
    bool  stop_on_incorrect;
    bool  stop_on_cant_solve;
    int   one_puzzle_only;
    int   total_steps;
    bool  double_check_steps;
    bool  check_solvable;
    bool  check_incorrect;
    int   recursion_limit;
} flags;

flags f = {
    .puzzle_filename     = "data/puzzles7_serg_benchmark",
    .display_solve       = False,
    .display_candidates  = True,
    .display_step_time   = 0.5,
    .stop_on_incorrect   = False,
    .stop_on_cant_solve  = False,
    .one_puzzle_only     = -1,
    .total_steps         = 0,
    .double_check_steps  = False,
    .check_solvable      = False,
    .check_incorrect     = True,
    .recursion_limit     = 2
};
