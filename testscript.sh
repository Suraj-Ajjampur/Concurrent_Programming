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
./mysort -i 321.txt -o out.ans -t5 --lock=tas
if cmp -s 321.txt out.ans; then
    echo -e "${GREEN}Assertion passed: Files are identical for 321${NOCOLOR}"
else
    echo -e "${RED}Assertion failed: Files are different for 321${NOCOLOR}"
fi 

#Testing from the my_own_test_directory
./mysort -i my_own_test_directory/10in0-20.txt -o my_own_test_directory/10in0-20.ans -t2 --lock=ttas
if cmp -s ${TESTDIR}/10in0-20.txt ${TESTDIR}/10in0-20.ans; then
    echo -e "${GREEN}Assertion passed: Files are identical for 10in0-20${NOCOLOR}"
else
    echo -e "${RED}Assertion failed: Files are different for 10in0-20${NOCOLOR}"
fi

rm *.ans ${TESTDIR}/*.ans