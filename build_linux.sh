#!/bin/bash
echo "removing and recreating build directory...."
rm -rf build
mkdir build
cd build
cmake ..
make
cd ..
echo "run using ./build/Application if the build was successful"