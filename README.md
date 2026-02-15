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

## Building

Build the library:
```bash
bazel build :lakeml-lib
```

Build and run the demo:
```bash
bazel run :lakeml-demo
```

## Project Structure

```
lakeml/
├── src/              # Source files (.h and .cpp)
│   ├── classifier.h  # Base classifier interface
│   ├── dataset.h     # Dataset container
│   └── ...
├── demo/             # Demo applications
│   └── demo.cpp      # K-means demo
├── BUILD             # Bazel build configuration
└── README.md         # This file
```

## License

This project is licensed under the GNU General Public License v3.0 or later.  
See [COPYING](COPYING) for details.

Copyright 2008-2012 Hugo Penedones
