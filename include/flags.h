#pragma once

#include "bool.h"
#include "printing.h"

typedef struct flags {
    str   puzzle_filename;
    bool  do_visible;
    float do_visible_step_time;
    bool  stop_on_incorrect;
    bool  stop_on_cant_solve;
    int   do_one_only;
    int   total_visible_steps;
    bool  double_check_steps;
    bool  check_solvable;
    bool  check_incorrect;
} flags;

flags f = {
    .puzzle_filename      = "data/puzzles2_17_clue",
    .do_visible           = False,
    .do_visible_step_time = 0.5,
    .stop_on_incorrect    = False,
    .stop_on_cant_solve   = False,
    .do_one_only          = -1,
    .total_visible_steps  = 0,
    .double_check_steps   = False,
    .check_solvable       = False,
    .check_incorrect      = False,
};
