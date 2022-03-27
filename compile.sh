#!/bin/bash

##############################
# For linux only. Usage:
# ./compile.sh FILE.c [run]
##############################

# Check if sdl2 is installed
if [[ $(command -v sdl2-config) == "" ]]; then
	echo "Cant' find sdl2. Exiting..."
	exit 1;
fi

SDL_FLAGS=$(sdl2-config --cflags --libs)
OUTPUT_FILE="langtons-ant.out"  # Change with project name

if [[ $1 == "" ]]; then
	echo "No arguments. Please enter a file. Exiting..."
	exit 1;
fi

# Compile the file
gcc -Wall -o $OUTPUT_FILE $1 $SDL_FLAGS

# If the second arg is run then run after compiling
if [[ $2 == "run" ]]; then
	echo "Compiled! Running..."
	./$OUTPUT_FILE
fi
