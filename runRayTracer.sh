#!/bin/bash

LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./doRayTracing 1e20
./doRayTracing 1e21