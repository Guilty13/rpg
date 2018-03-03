#!/bin/bash
set -x
g++ -O0 --std c++14 requirement.cpp stats.cpp quest.cpp requirement.ut.cpp -o test
./test
g++ -O0 --std c++14 requirement.cpp stats.cpp quest.cpp quest.ut.cpp -o test
./test
