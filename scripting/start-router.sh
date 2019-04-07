#!/bin/bash

# start-router script

# Delay before the start of each process
readonly TIME_BETWEEN=0.5

# All possible router arguments
# COMMANDS=("A" "B" "C" "D" "E" "F" "G")
COMMANDS=("0" "1" "2" "3" "4" "5" "6")

# Randomise router arguments
COMMANDS=($(shuf -e "${COMMANDS[@]}"))

# Run each router binary with a delay after each
for COMMAND in "${COMMANDS[@]}"; 
  do
    ./test $COMMAND &
    # sleep $TIME_BETWEEN
  done