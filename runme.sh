#!/bin/bash
set -e
set -x
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp quest.cpp globals.cpp requirement.ut.cpp -o test
./test
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp quest.cpp globals.cpp quest.ut.cpp -o test
./test
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp quest.cpp hero.cpp globals.cpp hero.ut.cpp -o test
./test

