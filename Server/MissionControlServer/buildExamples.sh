#!/usr/bin/env bash

# build some example executables
cd ./examples;

g++ -Wall -Werror -O3 -std=c++17 ./postgresExampleAlt2.cpp -lpqxx -lpq -o ../output/exampleAlt2;

g++ -Wall -Werror -O3 -std=c++17 ./postgresRemoveTable.cpp -lpqxx -lpq -o ../output/removeTable;

cd ./tcpScaleServer; make all; cd ..;

cd ..;
