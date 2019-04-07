#!/bin/bash

# start-router script

# Delay inbetween each process
readonly TIME_BETWEEN=0.5
readonly ROUTER_DIR="routers-run"
readonly ROUTER_EXE="router"

# All possible router arguments
COMMANDS=("A" "B" "C" "D" "E" "F" "G")

# Randomise router arguments
COMMANDS=($(shuf -e "${COMMANDS[@]}"))

# Clear all router files
rm -r $ROUTER_DIR/*
# Navigate into router files
cd $ROUTER_DIR

# Create files and folder for each router
for COMMAND in "${COMMANDS[@]}"; 
  do
    # Create seperate directory for current router
    mkdir $ROUTER_EXE-$COMMAND
    # Copy router binary and other files into new folders
    cp ../$ROUTER_EXE $ROUTER_EXE-$COMMAND
    cp ../paths.txt $ROUTER_EXE-$COMMAND
    cp ../links.csv $ROUTER_EXE-$COMMAND
  done

# Run each router binary with a delay after each
for COMMAND in "${COMMANDS[@]}"; 
  do
    echo $ROUTER_EXE $COMMAND
    ./$ROUTER_EXE-$COMMAND/$ROUTER_EXE $COMMAND &
    sleep $TIME_BETWEEN
  done