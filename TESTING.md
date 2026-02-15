# Testing and Development Setup Guide

## Overview
This document provides detailed instructions for setting up the development environment and running tests for the lakeml C++ machine learning library.

## Prerequisites

- **Bazel**: Version 7.0+ (build system)
- **C++11 compatible compiler**: GCC 9.3+ or Clang 10+
- **Python 3**: For pre-commit hooks (optional)
- **pre-commit**: For automatic code formatting (optional)

## Quick Start

### 1. Build the Library

```bash
bazel build :lakeml-lib
```

### 2. Run All Tests

```bash
bazel test //tests/...
```

### 3. Run Specific Test Suites

```bash
# Loss function tests
bazel test //tests:loss_test

# Classifier base class tests
bazel test //tests:classifier_test

# AdaBoost (Boosted Classifier) tests
bazel test //tests:boosted_classifier_test

# Gaussian Mixture Model tests
bazel test //tests:gaussian_mixture_model_test

# Threshold Learner (Decision Tree) tests
bazel test //tests:threshold_learner_test
```

## Test Coverage

The test suite covers the following components:

### Loss Function Tests (`tests/loss_test.cc`)
- ExponentialLoss value computation
- Gradient computation
- Edge cases (empty datasets, zero weights)

### Classifier Base Class Tests (`tests/classifier_test.cc`)
- Batch response methods
- Batch classification methods
- Single sample predictions
- Empty dataset handling

### AdaBoost Tests (`tests/boosted_classifier_test.cc`)
- Training with weak learners
- Prediction accuracy
- Weak learner count tracking
- Response with specific learner ranges
- Classification consistency

### Gaussian Mixture Model Tests (`tests/gaussian_mixture_model_test.cc`)
- Basic training with clustered data
- Classification after training
- Response computation
- Multiple Gaussian components
- Multidimensional feature support

### Threshold Learner Tests (`tests/threshold_learner_test.cc`)
- Training and prediction
- Classification
- Weighted training
- Multidimensional features
- Response consistency
- Perfectly separable data handling

## Pre-commit Hooks Setup

Pre-commit hooks automatically format your code and run static analysis before each commit.

### Installation

1. Install pre-commit:
   ```bash
   pip install pre-commit
   ```

2. Install the git hooks:
   ```bash
   pre-commit install
   ```

3. (Optional) Run hooks on all files:
   ```bash
   pre-commit run --all-files
   ```

### What the Hooks Do

- **clang-format**: Automatically formats C++ code according to `.clang-format` configuration
- **clang-tidy**: Runs static analysis to catch potential bugs and code quality issues
- **trailing-whitespace**: Removes trailing whitespace
- **end-of-file-fixer**: Ensures files end with a newline
- **check-yaml**: Validates YAML file syntax
- **check-added-large-files**: Prevents accidentally committing large files
- **check-merge-conflict**: Detects merge conflict markers
- **mixed-line-ending**: Normalizes line endings to LF

## Code Formatting

The project uses clang-format for consistent C++ code formatting.

### Configuration
The formatting rules are defined in `.clang-format`:
- **Standard**: C++11
- **Style**: Based on Google style with Allman braces
- **Indent**: 4 spaces
- **Column Limit**: 100 characters
- **Pointer Alignment**: Left

### Manual Formatting

Format a single file:
```bash
clang-format -i src/my_file.cpp
```

Format all source files:
```bash
find src tests -name '*.cpp' -o -name '*.cc' -o -name '*.h' | xargs clang-format -i
```

Check formatting without modifying files:
```bash
clang-format --dry-run --Werror src/my_file.cpp
```

## Build Configuration

The `.bazelrc` file contains build configuration:

- **C++ Standard**: C++11 (`--cxxopt=-std=c++11`)
- **Test Output**: Show errors only (`--test_output=errors`)
- **JVM Memory**: 2GB for build process

## Continuous Integration

When integrated with CI/CD (GitHub Actions, Jenkins, etc.), you can use:

```bash
# Build and test in one command
bazel test //...

# Run with verbose output
bazel test //... --test_output=all

# Run with test summary
bazel test //... --test_summary=detailed
```

## Troubleshooting

### Issue: Tests fail to compile
- **Solution**: Ensure you have C++11 support: `g++ --version` or `clang++ --version`
- Check that all dependencies are available

### Issue: Bazel cannot find dependencies
- **Solution**: Run `bazel clean` and rebuild
- Check that WORKSPACE file is properly configured

### Issue: Pre-commit hooks fail
- **Solution**: Ensure clang-format and clang-tidy are installed
- Update pre-commit: `pre-commit autoupdate`
- Run manually to see detailed errors: `pre-commit run --all-files`

### Issue: clang-format not found
- **Solution**: Install clang-format:
  - Ubuntu/Debian: `sudo apt-get install clang-format`
  - macOS: `brew install clang-format`
  - Or use system package manager

## Writing New Tests

### Test Structure
```cpp
#include <gtest/gtest.h>
#include "src/your_component.h"

class YourComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test fixtures
    }

    void TearDown() override {
        // Cleanup
    }
};

TEST_F(YourComponentTest, TestName) {
    // Test implementation
    EXPECT_EQ(actual, expected);
}
```

### Adding a New Test File

1. Create test file in `tests/` directory: `tests/my_component_test.cc`

2. Add test target to `tests/BUILD`:
   ```python
   cc_test(
       name = "my_component_test",
       srcs = ["my_component_test.cc"],
       deps = [
           "//:lakeml-lib",
           "@com_google_googletest//:gtest_main",
       ],
   )
   ```

3. Run your test:
   ```bash
   bazel test //tests:my_component_test
   ```

## Best Practices

1. **Run tests before committing**: Ensure `bazel test //tests/...` passes
2. **Use pre-commit hooks**: Let them catch formatting and basic issues automatically
3. **Write descriptive test names**: Make it clear what each test validates
4. **Test edge cases**: Empty inputs, boundary conditions, error cases
5. **Keep tests focused**: One concept per test function
6. **Use test fixtures**: Share setup code with `SetUp()` and `TearDown()`

## Additional Resources

- [Bazel C++ Tutorial](https://bazel.build/tutorials/cpp)
- [Google Test Documentation](https://google.github.io/googletest/)
- [Pre-commit Documentation](https://pre-commit.com/)
- [Clang-Format Documentation](https://clang.llvm.org/docs/ClangFormat.html)
