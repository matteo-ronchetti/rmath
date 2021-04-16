#! /usr/bin/env bash

g++ test.cpp -O2 -o test
g++ bench.cpp -lbenchmark -lpthread -O2 -o bench