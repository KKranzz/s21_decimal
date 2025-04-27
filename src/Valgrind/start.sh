#!/bin/bash

dos2unix tests/*.check
make clean
make test

for test in $(ls *test*); do
    echo $(test)
    valgrind --tool=memcheck --leak-check=yes "./$test"
done
