#!/bin/bash

# start-router script

# All possible router setups
# COMMANDS=("./router A"
#     "./router B"
#     "./router C"
#     "./router D"
#     "./router E"
#     "./router F"
#     "./router G")
# COMMANDS=("router A"
#     "router B"
#     "router C"
#     "router D"
#     "router E"
#     "router F"
#     "router G")

COMMANDS=("test A"
    "test B"
    "test C"
    "test D"
    "test E"
    "test F"
    "test G")

NUMBERS=(0 1 2 3 4 5 6)

# 

# $(shuf -n7 -e "${NUMBERS[@]}")

# ${NUMBERS[RANDOM % ${#NUMBERS[@]}]}

NUMBERS=($(shuf -e "${NUMBERS[@]}"))
# echo ${COMMANDS[@]}
# printf "%s" "${myarray[@]}"

# for i in "${COMMANDS[@]}"
# do
#    echo "$i"    
#    # or do whatever with individual element of the array
# done

# for cmd in "$COMMANDS"   #  <-- Note: Added "" quotes.
# do
#   echo "$cmd"  # (i.e. do action / processing of $databaseName here...)
# done

# array=( $(echo "a;b;c;d;e;f;" | sed -r 's/(.[^;]*;)/ \1 /g' | tr " " "\n" | shuf | tr -d " " ) )

# for Item in ${COMMANDS[*]}
#   do
#     echo $Item
#     echo "Hello"
#   done

# for cmd in "${COMMANDS[@]}"; 
#   do 
#     echo "$cmd"
#     echo "#"
#   done

for num in "${NUMBERS[@]}"; 
  do 
    ./${COMMANDS[num]}
    sleep .5
  done

# listOfNames="RA
# RB
# R C
# RD"

# COMMANDS=("router A"
#     "router B"
#     "router C"
#     "router D"
#     "router E"
#     "router F"
#     "router G")

# To allow for other whitespace in the string:
# 1. add double quotes around the list variable, or
# 2. see the IFS note (under 'Side Notes')

# for databaseName in "$listOfNames"   #  <-- Note: Added "" quotes.
# do
#   echo "$databaseName"  # (i.e. do action / processing of $databaseName here...)
# done

# Outputs
# RA
# RB
# R C
# RD