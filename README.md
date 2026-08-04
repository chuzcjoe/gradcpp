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
  parameter->data() -= learning_rate * parameter->grad();
}
```

Training results:
```
step 0, loss: 6.03021
step 1, loss: 1.99298
step 2, loss: 0.201108
step 3, loss: 0.110702
step 4, loss: 0.078596
step 5, loss: 0.0606581
step 6, loss: 0.0492089
step 7, loss: 0.0412838
step 8, loss: 0.0354848
step 9, loss: 0.031065
step 10, loss: 0.0275898
step 11, loss: 0.0247888
step 12, loss: 0.0224856
step 13, loss: 0.0205598
step 14, loss: 0.0189269
step 15, loss: 0.0175256
step 16, loss: 0.0163106
step 17, loss: 0.0152475
step 18, loss: 0.0143099
step 19, loss: 0.0134771
step 20, loss: 0.0127328
step 21, loss: 0.0120636
step 22, loss: 0.011459
step 23, loss: 0.0109101
step 24, loss: 0.0104097
step 25, loss: 0.00995169
step 26, loss: 0.00953103
step 27, loss: 0.00914335
step 28, loss: 0.008785
step 29, loss: 0.00845279
step 30, loss: 0.00814402
step 31, loss: 0.00785631
step 32, loss: 0.00758762
step 33, loss: 0.00733614
step 34, loss: 0.00710029
step 35, loss: 0.00687869
step 36, loss: 0.00667008
step 37, loss: 0.00647339
step 38, loss: 0.00628761
step 39, loss: 0.0061119
step 40, loss: 0.00594545
step 41, loss: 0.00578757
step 42, loss: 0.00563761
step 43, loss: 0.00549501
step 44, loss: 0.00535925
step 45, loss: 0.00522984
step 46, loss: 0.00510636
step 47, loss: 0.00498841
step 48, loss: 0.00487565
step 49, loss: 0.00476772
```