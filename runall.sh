#!/bin/bash

# Build the sudoku program
echo "Building sudoku program..."
make sudoku-strict

# Check if build was successful
if [ $? -ne 0 ]; then
    echo "Error: Failed to build sudoku program"
    exit 1
fi

DIRECTORY="${1:-.}"  # Use current directory if not specified

# Check if directory exists
if [ ! -d "$DIRECTORY" ]; then
    echo "Error: Directory '$DIRECTORY' not found"
    exit 1
fi

echo "Processing files in directory: $DIRECTORY"

# Process each file in the directory
for file in "$DIRECTORY"/*; do
    # Skip if it's not a regular file
    if [ ! -f "$file" ]; then
        continue
    fi
    
    # Get just the filename (without path)
    filename=$(basename "$file")
    
    # Call sudoku.o with "data/" prefix
    echo "Processing: data/$filename"
    ./sudoku.o "data/$filename"
done

echo "Finished processing all files."