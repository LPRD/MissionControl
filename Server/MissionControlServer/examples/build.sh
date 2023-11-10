#!/usr/bin/env bash

# build some example executables

g++ -Wall -Werror -O3 -std=c++17 postgresExampleAlt2.cpp -lpqxx -lpq -o exampleAlt2;

g++ -Wall -Werror -O3 -std=c++17 postgresRemoveTable.cpp -lpqxx -lpq -o removeTable;


