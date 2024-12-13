#!/bin/bash

# Usage: ./reduce.sh input.cpp output.cpp

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 input_file output_file"
  exit 1
fi

input_file="$1"
output_file="$2"

# Remove comments, trim lines, and remove empty lines
sed -E '
    # Remove single-line comments
    s://.*$::g;
    # Remove multi-line comments
    :start
    s:/\*[^*]*\*+([^/*][^*]*\*+)*/::g;
    /\/\*/{
        N
        b start
    }
    # Trim leading and trailing whitespace
    s/^[[:space:]]+//g;
    s/[[:space:]]+$//g;
    # Remove empty lines
    /^[[:space:]]*$/d;
' "$input_file" >"$output_file"

echo "Cleaned file saved to $output_file"
