#!/bin/bash

# Compile the C program
echo "Compiling $C_FILE..."
gcc -c -o ogl.o ogl.c

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program:"
    echo "-----------------------------"
    ./"$OUTPUT_FILE"
    echo "-----------------------------"
else
    echo "Compilation failed."
fi

echo "Linking object files..."
gcc -o ogl ogl.o -lX11 -lGL -lGLU -lm

