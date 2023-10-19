#!/bin/bash

mkdir _build
cd _build

# Build and install
cmake -DPY=False ..
make
sudo make install
