.PHONY: sudoku sudoku-strict

default: sudoku-strict run

sudoku:
	@gcc -Wall -std=c18 -o sudoku.o main.c 

sudoku-strict:
	@gcc -Wall -Werror -std=c18 -o sudoku.o main.c 

run:
	@./sudoku.o

clean:
	@rm -rf sudoku.o