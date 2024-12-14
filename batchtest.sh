#!/bin/bash

inputext=".jtl.h"

# Function to run test on a given input file
run_test() {
  local input_file="$1"
  local output_file="${input_file}.out"
  if [ "$verbose" = true ]; then
    echo "Testing on $input_file"
    echo "Output to $output_file"
  fi
  ./main "$input_file" > "$output_file"
}

# Function to iterate over all files in the given directory and run tests
test_on_directory() {
  local dir="$1"
  for input_file in "$dir"/*; do
    if [ -f "$input_file" ] && [[ "$input_file" == *"$inputext" ]]; then
      run_test "$input_file"
    fi
  done
}

# Function to iterate over all files in the given directories and run tests
test_on_directories() {
  for dir in "$@"; do
    test_on_directory "$dir"
  done
}

# Function to clean all .out files in the given directory
clean_directory() {
  local dir="$1"
  for output_file in "$dir"/*.out; do
    if [ -f "$output_file" ]; then
      if [ "$verbose" = true ]; then
        echo "Removing $output_file"
      fi
      rm "$output_file"
    fi
  done
}

# Function to clean all .out files in the given directories
clean_directories() {
  for dir in "$@"; do
    clean_directory "$dir"
  done
}

# Check if at least one directory argument is provided
if [ -z "$2" ]; then
  echo "Usage: $0 [-v] <command> <directory>..."
  echo "Commands:"
  echo "  test <dir>...   : run the test in all files in the directories."
  echo "  clean <dir>...  : rm all the *.out files in the directories."
  exit 1
fi

# Check for verbose flag
if [ "$1" = "-v" ]; then
  verbose=true
  shift
else
  verbose=false
fi

# Main script execution
command="$1"
shift
directories=("$@")

case "$command" in
  test)
    test_on_directories "${directories[@]}"
    ;;
  clean)
    clean_directories "${directories[@]}"
    ;;
  *)
    echo "Unknown command: $command"
    echo "Usage: $0 [-v] <command> <directory>..."
    echo "Commands:"
    echo "  test <dir>...   : run the test in all files in the directories."
    echo "  clean <dir>...  : rm all the .out files in the directories."
    exit 1
    ;;
esac
