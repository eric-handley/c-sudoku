.PHONY: sudoku sudoku-strict extract runall clean

default: sudoku-strict run

sudoku:
	@gcc -Wall -std=c18 -o sudoku.o main.c 

sudoku-strict:
	@gcc -Wall -Werror -std=c18 -o sudoku.o main.c 

run:
	@./sudoku.o

extract:
	@echo "Extracting puzzle sets..."
	@7z x puzzle_sets.7z -y >/dev/null 2>&1 || (echo "Error: Failed to extract puzzle_sets.7z (is 7z available?)" && exit 1)
	@find data -name "*Zone.Identifier" -delete 2>/dev/null || true

runall: sudoku-strict
	@if [ ! -d "$${DIR:-data}" ]; then echo "Error: Directory $${DIR:-data} not found. Run 'make extract' first." && exit 1; fi
	@echo "Processing files in directory: $${DIR:-data}"
	@count=0; for file in $${DIR:-data}/*; do \
		if [ -f "$$file" ]; then \
			echo "Processing: $$file"; \
			./sudoku.o "$$file"; \
			count=$$((count + 1)); \
		fi; \
	done; \
	if [ $$count -eq 0 ]; then echo "Error: No puzzle files found in $${DIR:-data}" && exit 1; fi
	@echo "Finished processing all files."

clean:
	@rm -rf sudoku.o data/