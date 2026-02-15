# lakeml

Machine learning algorithms implemented in C++.

## Overview

lakeml is a lightweight C++ library providing implementations of classical machine learning algorithms including:

- **Classifiers**: AdaBoost (Boosted Classifier), Naive Bayes, Threshold Learner, Gaussian Learner
- **Clustering**: K-means, Gaussian Mixture Models (GMM)
- **Loss Functions**: Exponential Loss
- **Utilities**: Math utilities, 3D histograms

## Requirements

- [Bazel](https://bazel.build/) - Build system for compiling the project
- C++11 compatible compiler
- [pre-commit](https://pre-commit.com/) (optional) - For automatic code formatting hooks

## Building

Build the library:
```bash
bazel build :lakeml-lib
```

Build and run the demo:
```bash
bazel run :lakeml-demo
```

## Testing

Run all tests:
```bash
bazel test //tests/...
```

Run a specific test:
```bash
bazel test //tests:loss_test
bazel test //tests:classifier_test
bazel test //tests:boosted_classifier_test
bazel test //tests:gaussian_mixture_model_test
bazel test //tests:threshold_learner_test
```

## Development Setup

### Pre-commit Hooks

This project uses pre-commit hooks to automatically format code and run static analysis before commits.

1. Install pre-commit:
```bash
pip install pre-commit
```

2. Install the git hooks:
```bash
pre-commit install
```

3. (Optional) Run hooks manually on all files:
```bash
pre-commit run --all-files
```

The hooks will now run automatically before each commit, ensuring code quality and consistent formatting.

### Code Formatting

The project uses clang-format for C++ code formatting. The configuration is in `.clang-format`.

To manually format a file:
```bash
clang-format -i src/file.cpp
```

## Project Structure

```
lakeml/
├── src/                        # Source files (.h and .cpp)
│   ├── classifier.h            # Base classifier interface
│   ├── dataset.h               # Dataset container
│   └── ...
├── tests/                      # Unit tests
│   ├── BUILD                   # Test build configuration
│   ├── loss_test.cc            # Loss function tests
│   ├── classifier_test.cc      # Classifier tests
│   ├── boosted_classifier_test.cc  # AdaBoost tests
│   ├── gaussian_mixture_model_test.cc  # GMM tests
│   └── threshold_learner_test.cc   # Threshold learner tests
├── demo/                       # Demo applications
│   └── demo.cpp                # K-means demo
├── BUILD                       # Main build configuration
├── WORKSPACE                   # Bazel workspace configuration
├── .clang-format               # C++ formatting configuration
├── .pre-commit-config.yaml     # Pre-commit hooks configuration
└── README.md                   # This file
```

## License

This project is licensed under the GNU General Public License v3.0 or later.  
See [COPYING](COPYING) for details.

Copyright 2008-2012 Hugo Penedones
