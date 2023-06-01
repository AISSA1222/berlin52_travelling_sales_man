#!/bin/bash
g++ -c main.cpp
g++ -c esclade_simple.cpp
g++ main.o esclade_simple.o -o pvc
./pvc

