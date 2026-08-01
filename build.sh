#!/usr/bin/env bash
# Configures, builds, and runs the project. This is the single entry point
# for all compilation and execution.
set -euo pipefail

git config core.hooksPath .githooks

mkdir -p build
cd build

cmake ..
make

cp ./compile_commands.json ../compile_commands.json

../build/main
