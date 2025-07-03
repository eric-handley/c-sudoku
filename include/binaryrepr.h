#pragma once

#define B(n) ((n) ? (0b1 << ((n) - 1)) : 0)
#define D(n) ((n) ? (__builtin_ctz(n) + 1) : 0)
#define for_bdigit_in_range(i, j, itername) int decim = 0; for (int itername = B(i); itername != (B(j) << 1); itername = ((itername) ? (itername << 1) : 0b1))
#define for_range(i, j, itername) for (int itername = i; itername < j; itername++)
#define for_range_09(itername) for_range(0, 9, itername)
#define for_ij_09() for_range(0, 9, i) { for_range(0, 9, j)
#define for_ijk_09() for_range(0, 9, i) { for_range(0, 9, j) { for_range(0, 9, k)

#include "sudoku.h"
#include "printing.h"
#include "reader.h"

#define ALL_T 0b111111111
#define ALL_F 0b000000000