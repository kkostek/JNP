#!/bin/bash
g++ -c -Wall -Wextra -O2 -std=c++17 sejf.cc -o sejf.o
g++ -c -Wall -Wextra -O2 -std=c++17 test.cc -o test.o
g++ test.o sejf.o  -o test

