#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# GLFW Paths
GLFW_SOURCE="vendor/glfw"
GLFW_BUILD_DIR="libs/glfw-build"
GLFW_INSTALL_DIR="${GLFW_BUILD_DIR}/install"

# Build GLFW
echo "Configuring GLFW..."
cmake -S "$GLFW_SOURCE" \
      -B "$GLFW_BUILD_DIR" \
      -D GLFW_BUILD_TESTS=OFF \
      -D GLFW_BUILD_EXAMPLES=OFF \
      -D GLFW_BUILD_DOCS=OFF \
      -D GLFW_BUILD_WAYLAND=0


echo "Building GLFW..."
cmake --build "$GLFW_BUILD_DIR" --parallel 10

echo "Installing GLFW..."
cmake --install "$GLFW_BUILD_DIR" --prefix "$GLFW_INSTALL_DIR"