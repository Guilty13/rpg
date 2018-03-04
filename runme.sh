#!/bin/bash
set -e
set -x
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp requirement.ut.cpp -o test
./test
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp quest.cpp quest.ut.cpp -o test
./test
g++ -Wall -O3 --std c++17 requirement.cpp stats.cpp quest.cpp hero.cpp hero.ut.cpp -o test
./test

