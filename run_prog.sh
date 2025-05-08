#!/bin/bash

cd build || { echo "Build directory not found"; exit 1; }

cmake --build . || { echo "Build failed"; exit 1; }

cp spg ../ || { echo "Failed to copy 'spg' executable"; exit 1; }

cd ../ || { echo "Failed to navigate back to user's folder"; exit 1; }


./spg || { echo "Failed to execute 'spg'"; exit 1; }