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

Build and run the Iris dataset demo (from the workspace root):
```bash
bazel run :lakeml-iris-demo
```

The Iris demo loads `data/iris.csv` and demonstrates:
- **K-means clustering** with k=3, showing cluster assignments vs. true class labels
- **Naive Bayes binary classification** distinguishing *Iris setosa* from the other two species

Example output:
```
Loading dataset from: data/iris.csv
Loaded 150 samples with 4 features.

=== K-means Clustering (k=3) ===
Converged in 11 iterations.

Samples per class assigned to each cluster:
Class               Cluster0  Cluster1  Cluster2
Iris-setosa         0         50        0
Iris-versicolor     47        0         3
Iris-virginica      14        0         36

=== Naive Bayes Classification (Setosa vs. Others) ===
Training accuracy: 98.7% (148/150)
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
bazel test //tests:csv_loader_test
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

## Datasets

The `data/` directory contains standard datasets for demonstrating the library's algorithms:

| File | Samples | Features | Classes | Description |
|------|---------|----------|---------|-------------|
| `data/iris.csv` | 150 | 4 | 3 | Fisher's Iris dataset (sepal/petal length and width) |

Datasets are simple CSV files with a header row. The last column is the class label. They can be loaded with the `LoadCsvDataset` helper (see `src/csv_loader.h`), which uses only the C++ standard library:

```cpp
#include "src/csv_loader.h"

Dataset dataset = LoadCsvDataset("data/iris.csv");
```

## Project Structure

```
lakeml/
├── src/                        # Source files (.h and .cpp)
│   ├── classifier.h            # Base classifier interface
│   ├── csv_loader.h            # CSV dataset loader (header-only)
│   ├── dataset.h               # Dataset container
│   └── ...
├── data/                       # Standard datasets (CSV format)
│   └── iris.csv                # Fisher's Iris dataset (150 samples)
├── tests/                      # Unit tests
│   ├── BUILD                   # Test build configuration
│   ├── loss_test.cc            # Loss function tests
│   ├── classifier_test.cc      # Classifier tests
│   ├── boosted_classifier_test.cc  # AdaBoost tests
│   ├── csv_loader_test.cc      # CSV loader tests
│   ├── gaussian_mixture_model_test.cc  # GMM tests
│   └── threshold_learner_test.cc   # Threshold learner tests
├── demo/                       # Demo applications
│   ├── demo.cpp                # K-means demo (hardcoded data)
│   └── iris_demo.cpp           # K-means + Naive Bayes demo on Iris dataset
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
