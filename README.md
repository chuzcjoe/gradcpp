# grad.cpp

`grad.cpp` is a small scalar-valued automatic differentiation engine and neural
network library written in C++20. It is the C++ version of Andrej Karpathy's
[karpathy/micrograd](https://github.com/karpathy/micrograd).

The project builds a dynamic computation graph, performs reverse-mode automatic
differentiation with `Backward()`, and includes basic neural-network components
such as neurons, layers, and multilayer perceptrons (MLPs).

## Requirements

- A C++ compiler with C++20 support (Clang or GCC)
- CMake 3.16 or newer
- Make, or another build tool supported by CMake
- Bash, when using the provided `build.sh` script

The project is compiled with optimization enabled (`-O3`) and treats compiler
warnings as errors (`-Werror`).

## Build and run

The simplest way to configure, build, and run the example is:

```bash
./build.sh
```

Alternatively, run the CMake commands manually:

```bash
cmake -S . -B build
cmake --build build
./build/main
```

The build uses the C++20 standard and generates `compile_commands.json` for
editor tooling and static analysis.

## Project structure

```text
grad/include/     Public library headers
grad/src/         Automatic differentiation implementation
src/main.cpp      Example MLP training program
CMakeLists.txt    Top-level CMake configuration
build.sh          Configure, build, and run helper
```

## Example

The example in `src/main.cpp` creates an MLP, evaluates a small training set,
computes a squared-error loss, clears existing gradients, runs backpropagation,
and updates the network parameters with gradient descent.

The core training sequence is:

```cpp
mlp.ZeroGrad();
loss.Backward();

for (grad::Value* parameter : mlp.Parameters()) {
  parameter->node->data -= learning_rate * parameter->grad();
}
```
