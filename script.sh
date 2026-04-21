#!/bin/bash

# 1. Check if the user provided a directory name
if [ -z "$1" ]; then
    echo "Usage: ./run_tool.sh <directory_name>"
    exit 1
fi

# 2. Compile the C program (if not already compiled)
# Using -o my_tool to give the output a specific name
gcc main.c commands.c -o my_tool

# 3. Check if compilation was successful
if [ $? -eq 0 ]; then
    # 4. Run the program with the first argument passed to the script
    ./my_tool "$1"
else
    echo "Compilation failed!"
    exit 1
fi