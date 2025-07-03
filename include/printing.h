#pragma once

#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef char* str;

#define WRN "\033[41;1;30m"
#define YEL "\033[33m"
#define RED "\033[31m"
#define GRN "\033[32m"
#define RST "\033[0m"

#define s_TO_μs (1e6)
#define sleep(n) usleep(n * s_TO_μs)
#define print printf

#define H_LINE_THICK  "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
#define NL            "\n"
#define T_BORDER      "┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓"
#define B_BORDER      "┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛"
#define THICK_ROW_SEP "┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫"
#define THIN_ROW_SEP  "┠───┼───┼───╂───┼───┼───╂───┼───┼───┨"
#define THICK_V_BAR   "┃"
#define THIN_V_BAR    "│"
#define CELL_PAD      " "
