# cpp_lint_template

A minimal C++20 project template with formatting and static analysis wired in from the start.

## What's included

- CMake build, C++20
- `clang-format` using the Google C++ Style
- `clang-tidy` with a baseline set of checks (bugprone, clang-analyzer, performance, portability, readability, modernize) plus Google-style naming conventions
- Pre-commit hook (`.githooks/pre-commit`) that runs both checks on staged files
- `build.sh` as the single entry point for configuring, building, and running

## Requirements

- CMake >= 3.16
- A C++20 compiler (clang or gcc)
- `clang-format` and `clang-tidy` on `PATH`

## Build and run

```sh
./build.sh
```

This configures the project into `build/`, compiles it, and runs the resulting binary. Run it again after any change; there's no separate build step.

## Git hooks

The hook lives in `.githooks/pre-commit` instead of `.git/hooks` so it's tracked in version control. The first `cmake` configure (triggered by `build.sh`) sets `core.hooksPath` to `.githooks` automatically, so cloning the repo and running `./build.sh` once is all that's needed.

The hook checks staged `.cpp/.cc/.cxx/.h/.hpp` files:

- `clang-format --dry-run --Werror` for formatting
- `clang-tidy` for the checks in `.clang-tidy` (requires `build/compile_commands.json`, which `build.sh` generates)

If either tool is missing from `PATH`, the commit is rejected with an install prompt rather than silently skipping the check.

## Project structure

```
.
├── CMakeLists.txt
├── build.sh
├── .clang-format
├── .clang-tidy
├── .githooks/
│   └── pre-commit
└── src/
    └── main.cpp
```

## Extending

- Add source files under `src/` and list them in `CMakeLists.txt`.
- Add or relax `clang-tidy` checks in `.clang-tidy` as the project grows.
