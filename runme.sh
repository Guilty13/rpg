#!/bin/bash
set -e
set -x
g++ -O0 --std c++14 requirement.cpp stats.cpp requirement.ut.cpp -o test
./test
g++ -O0 --std c++14 requirement.cpp stats.cpp quest.cpp quest.ut.cpp -o test
./test
g++ -O0 --std c++14 requirement.cpp stats.cpp quest.cpp hero.cpp hero.ut.cpp -o test
./test

