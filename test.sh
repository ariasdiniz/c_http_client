#!/bin/bash

# Created by Aria Diniz on January 5, 2024
# This script was created with the objective of simplifying the compilation of C programs from command line.

# Directory with the files to compile. It will search for files with extension .c and .h on every dir inside this directory.
# Caution, when pointed to the software's main folder it can compile the tests along with the program.
directory="src"
test_dir="test"
libs_dir="lib"

# Find all files with the specified extension in the directory and its subdirectories
# and read them into an array
readarray -t lib < <(find "$libs_dir" -type f \( -name "*.c" -o -name "*.h" \))
readarray -t files < <(find "$directory" -type f \( -name "*.c" -o -name "*.h" \))
readarray -t testfiles < <(find "$test_dir" -type f \( -name "*.c" -o -name "*.h" \))

# Initialize an empty string to hold all file names
all_files=""

# Loop through the array and append each file name to the string
for file in "${files[@]}"; do
    all_files+="$file "
done
for file in ${testfiles[@]}; do
    all_files+="$file "
done
for file in "${lib[@]}"; do
    all_files+="$file "
done

# Compile the result
gcc -o tests $all_files -O0 -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-result -Wno-unused-label -Wno-unused-val

# Run the program
chmod u+x tests
valgrind --leak-check=yes ./tests

# Remove the executable
rm tests
