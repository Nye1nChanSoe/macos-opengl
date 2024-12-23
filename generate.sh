#!/bin/bash

# Variables
BUILD_DIR="build"
BIN_DIR="bin/Debug"
EXECUTABLE="macos-opengl"

echo "==> Generating build files with Premake5..."
premake5 gmake2

if [ $? -ne 0 ]; then
    echo "Error: Premake generation failed."
    exit 1
fi

echo "==> Building project with make..."
cd $BUILD_DIR
make

if [ $? -ne 0 ]; then
    echo "Error: Build failed."
    cd ..
    exit 1
fi

echo "==> Running the application..."
cd ..
./$BIN_DIR/$EXECUTABLE