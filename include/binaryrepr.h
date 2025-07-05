#pragma once

#define popcount(num) __builtin_popcount(num) // Count number of set bits in num (population count)
#define ctz_9bit(num) __builtin_ctz(num)      // Count trailing zeros of 9 bit binary number 
#define clz_9bit(num) 31 - __builtin_clz(num) // Count leading zeros of 9 bit binary number 

#define base2(n) ((n) ? (0b1 << ((n) - 1)) : 0)   // Base 2 representation of base 10 digit n 
#define base10(n) ((n) ? (ctz_9bit((n)) + 1) : 0) // Base 10 digit for base 2 representation n
#define for_bdigit_in_range(i, j, itername) for (int itername = base2(i); itername < base2(j); itername = ((itername) ? (itername << 1) : 0b1))
#define for_range(i, j, itername) for (int itername = i; itername < j; itername++)
#define for_range_03(itername) for_range(0, 3, itername)
#define for_range_09(itername) for_range(0, 9, itername)
#define for_ij_09() for_range(0, 9, i) { for_range(0, 9, j)
#define for_ijk_09() for_range(0, 9, i) { for_range(0, 9, j) { for_range(0, 9, k)
        
#define minus_cand(cand, rem)  (cand) &= ~(rem)          // Reset bits in cand that are set in rem
#define get_lowest_cand(cand)  base2(ctz_9bit(cand) + 1) // Get binary representation of smallest candidate digit
#define get_highest_cand(cand) base2(clz_9bit(cand) + 1) // Get binary representation of largest candidate digit

#include "sudoku.h"
#include "printing.h"
#include "reader.h"

#define ALL_T 0b111111111 // All 9 bits set true
#define ALL_F 0b000000000 // All 9 bits set false