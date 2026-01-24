#! /bin/bash

echo "Starting tests..."

# Detect the ALU_Group*.circ file(s) in the current or ALU directory
circ_file=$(find . -type f -name "FDE_Group*.circ" | head -n 1)

# Check if any .circ file was found; if not, exit with an error message
if [[ -z $circ_file ]]; then
  echo "Error: No FDE_Group*.circ file found in the current directory"
  exit 1
fi

# Run tests if the --donttest flag is not set
if [[ $1 != --donttest ]]; then
	for test_file in $(find . -type f -name "*.txt"); do
		python3.11 Test_2425_zit1.py -c "$circ_file" -f -i "$test_file"
	done
fi

echo "Done :)"
