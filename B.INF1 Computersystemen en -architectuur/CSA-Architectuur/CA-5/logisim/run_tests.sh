#! /bin/bash

echo "Use the flag '--donttest' to only delete the .report and .test files"
echo "Use the flag '--dontdelete' to only run the tests, but keep the .report and .test files"
echo ""
echo "Starting tests..."

# Detect the ALU_Group*.circ file(s) in the current or ALU directory
circ_file=$(find . -type f -name "SD_Group*.circ" | head -n 1)

# Check if any .circ file was found; if not, exit with an error message
if [[ -z $circ_file ]]; then
  echo "Error: No SD_Group*.circ file found in the current or ALU directory."
  exit 1
fi

# Run tests if the --donttest flag is not set
if [[ $1 != --donttest ]]; then
	for test_file in $(find SD_tests/ -type f -name "*test.txt"); do
		python3.11 Test_2425_zit1.py -c "$circ_file" -s -i "$test_file"
	done
fi

# Delete generated .report and .test files if --dontdelete flag is not set
if [[ $1 != --dontdelete ]]; then
  echo "Deleting files..."
  find * -type f \( -name "*.report*" -o -name "*.test*" \) -exec echo "Deleting file {}" \; -delete
fi

echo "Done :)"
