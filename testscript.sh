#!/bin/bash
#Shebang

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NOCOLOR='\033[0m'
TESTDIR="./my_own_test_directory"

### Build your executable
make clean && make

# Basic Testing
./mysort -i 321.txt -o 321.my -t5 --lock=tas
if cmp -s 321.my 321.my; then
    echo -e "${GREEN}Assertion passed: Files are identical for 321${NOCOLOR}"
else
    echo -e "${RED}Assertion failed: Files are different for 321${NOCOLOR}"
fi 

#Testing from the my_own_test_directory
./mysort -i my_own_test_directory/10in0-20.txt -o my_own_test_directory/10in0-20.my -t2 --lock=ttas
if cmp -s ${TESTDIR}/10in0-20.ans ${TESTDIR}/10in0-20.my; then
    echo -e "${GREEN}Assertion passed: Files are identical for 10in0-20${NOCOLOR}"
else
    echo -e "${RED}Assertion failed: Files are different for 10in0-20${NOCOLOR}"
fi

# Function to run the executable and compare output
run_test() {
    input_file="$1"
    output_file="${input_file%.txt}.my"
    answer_file="${input_file%.txt}.ans"
    
    # Run the executable
    ./mysort -i "$input_file" -o "$output_file" -t5 --lock=tas

    # Compare the generated output with the answer file
    if cmp -s "$output_file" "$answer_file"; then
        echo -e "${GREEN}Assertion passed: Files are identical for $input_file${NOCOLOR}"
    else
        echo -e "${RED}Assertion failed: Files are different for $input_file${NOCOLOR}"
    fi
}

# Testing from the my_own_test_directory
for input_file in ${TESTDIR}/*.txt; do
    run_test "$input_file"
done

rm *.ans ${TESTDIR}/*.ans