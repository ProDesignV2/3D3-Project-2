#!/bin/bash

# start-router script

# Delay inbetween each process
readonly TIME_BETWEEN=0.1
readonly ROUTER_DIR="routers-run"
readonly ROUTER_EXE="my-router"

# All possible router arguments
COMMANDS=("A" "B" "C" "D" "E" "F")
EXTRA="G"
INJECT_FLAG="H"

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

# Key-value list to store process IDs
declare -A ROUTER_IDS

# Run each router binary with a delay after each
for COMMAND in "${COMMANDS[@]}"; 
  do
    echo $ROUTER_EXE $COMMAND
    ./$ROUTER_EXE-$COMMAND/$ROUTER_EXE $COMMAND &
    # Save process ID
    ROUTER_IDS[$COMMAND]=$!
    # Time between starting of processes
    sleep $TIME_BETWEEN
  done

# Flag for termination of processes
stop_all=0

# Wait for user input and take action
while [ $stop_all -eq 0 ]
  do
    # Read in user input
    read -p "ENTER ACTION (ADD DELETE INJECT STOP) " action
    # Switch statement to choose action
    case $action in
    "ADD")
      # Read in user input
      echo "<Port> <Neighbour 1 Name> <Neighbour 1 Cost> <Neighbour 2 Name> <Neighbour 2 Cost>"
      read -p "-> " details
      # Create seperate directory for current router
      mkdir $ROUTER_EXE-$EXTRA
      # Copy router binary and other files into new folder
      cp ../$ROUTER_EXE $ROUTER_EXE-$EXTRA
      cp ../paths.txt $ROUTER_EXE-$EXTRA
      cp ../links.csv $ROUTER_EXE-$EXTRA
      # Run new router process with router arguments
      # <Name> <Port> <Neighbour 1 Name> <Neighbour 1 Cost> <Neighbour 2 Name> <Neighbour 2 Cost>
      ./$ROUTER_EXE-$EXTRA/$ROUTER_EXE $EXTRA $details &
      # Save process ID
      ROUTER_IDS[$EXTRA]=$!
      ;;
    "DELETE")
      # Read in user input
      read -p "Which router? " choice
      kill ${ROUTER_IDS[$choice]}
      ;;
    "INJECT")
      echo "INJECTING DATA..."
      # Create temporary directory for data injecting router
      mkdir $ROUTER_EXE-$INJECT_FLAG
      # Copy router binary and other files into new folder
      cp ../$ROUTER_EXE $ROUTER_EXE-$INJECT_FLAG
      cp ../paths.txt $ROUTER_EXE-$INJECT_FLAG
      cp ../links.csv $ROUTER_EXE-$INJECT_FLAG
      # Run router process with data inject flag and wait for completion
      # ./$ROUTER_EXE-$INJECT_FLAG/$ROUTER_EXE $INJECT_FLAG
      # Delete temporary folder
      rm -r $ROUTER_EXE-$INJECT_FLAG
      ;;
    "STOP")
      echo "STOPPING ALL ROUTERS..."
      # Break from while loop using flag
      stop_all=1
      ;;
    *)
      echo "INVALID ACTION"
      ;;
    esac
  done

# echo "CONFIRM [ctrl + c]"
# Wait for all processes to finish or user to quit
# wait ${ROUTER_IDS["A"]} ${ROUTER_IDS["B"]} ${ROUTER_IDS["C"]}
pkill -P $$