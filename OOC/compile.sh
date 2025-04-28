#!/bin/bash
g++ main.cxx -o main $(root-config --cflags --libs)
