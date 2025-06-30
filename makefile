.PHONY: sudoku sudoku-strict

default: sudoku-strict run clean

sudoku:
	@gcc -Wall -std=c18 -o sudoku.o sudoku.c 

sudoku-strict:
	@gcc -Wall -Werror -std=c18 -o sudoku.o sudoku.c 

run:
	@./sudoku.o

clean:
	@rm -rf sudoku.o