# Contributing to lakeml

Thank you for your interest in contributing to lakeml! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites

1. **Bazel** (build system)
   ```bash
   # Follow installation instructions at https://bazel.build/install
   ```

2. **C++11 compatible compiler**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install g++ build-essential
   
   # macOS
   xcode-select --install
   ```

3. **pre-commit** (optional but recommended)
   ```bash
   pip install pre-commit
   pre-commit install
   ```

### Getting Started

1. Fork and clone the repository:
   ```bash
   git clone https://github.com/yourusername/lakeml.git
   cd lakeml
   ```

2. Build the project:
   ```bash
   bazel build :lakeml-lib
   ```

3. Run the tests:
   ```bash
   bazel test //tests/...
   ```

## Development Workflow

### Before Making Changes

1. Create a new branch for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. Ensure all tests pass:
   ```bash
   bazel test //tests/...
   ```

### Making Changes

1. **Write tests first**: For new features, write tests before implementation
2. **Follow the existing code style**: The project uses `.clang-format` for consistency
3. **Keep changes focused**: One feature/fix per pull request
4. **Update documentation**: If you change public APIs, update relevant documentation

### Code Style

The project enforces code style through `.clang-format` configuration:

- **Standard**: C++11
- **Indentation**: 4 spaces
- **Line length**: 100 characters maximum
- **Braces**: Allman style (braces on new lines)
- **Pointer alignment**: Left (`int* ptr` not `int *ptr`)

Format your code:
```bash
clang-format -i src/your_file.cpp
```

Or let pre-commit hooks do it automatically:
```bash
pre-commit run --all-files
```

### Running Tests

Run all tests:
```bash
bazel test //tests/...
```

Run specific test suite:
```bash
bazel test //tests:your_test
```

Run with verbose output:
```bash
bazel test //tests:your_test --test_output=all
```

### Committing Changes

If you have pre-commit hooks installed, they will run automatically. Otherwise:

1. Format your code:
   ```bash
   clang-format -i $(find src tests -name '*.cpp' -o -name '*.cc' -o -name '*.h')
   ```

2. Run tests:
   ```bash
   bazel test //tests/...
   ```

3. Commit with a descriptive message:
   ```bash
   git commit -m "Add feature X to improve Y"
   ```

## Pull Request Process

1. **Update tests**: Ensure your changes are covered by tests
2. **Update documentation**: If applicable, update README.md and other docs
3. **Ensure CI passes**: All tests must pass
4. **Write a clear description**: Explain what changes you made and why
5. **Link related issues**: Reference any related issue numbers

### Pull Request Template

```markdown
## Description
Brief description of changes

## Motivation
Why is this change needed?

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Performance improvement
- [ ] Code refactoring
- [ ] Documentation update
- [ ] Other (please describe)

## Testing
How was this tested?

## Checklist
- [ ] Tests pass locally
- [ ] Code follows project style
- [ ] Documentation updated
- [ ] No new warnings introduced
```

## Adding New Features

### Adding a New Algorithm

1. **Create header file**: `src/algorithm_name.h`
   - Include proper copyright header
   - Define class interface
   - Document public methods

2. **Create implementation**: `src/algorithm_name.cpp`
   - Implement the algorithm
   - Follow existing patterns

3. **Update BUILD file**: Add to `lakeml-lib` target in root `BUILD`

4. **Write tests**: `tests/algorithm_name_test.cc`
   - Test basic functionality
   - Test edge cases
   - Test error conditions

5. **Add test target**: Update `tests/BUILD`

### Example: Adding New Classifier

```cpp
// src/my_classifier.h
#ifndef MY_CLASSIFIER_H
#define MY_CLASSIFIER_H

#include "classifier.h"

class MyClassifier : public Classifier
{
public:
    MyClassifier(/* parameters */);
    
    void train(const Dataset & training_dataset, 
               const std::vector<double> &weights) override;
    double response(const DataInstance & data_instance) const override;
    int classify(const DataInstance & data_instance) const override;

private:
    // Private members
};

#endif
```

## Testing Guidelines

1. **Test coverage**: Aim for comprehensive coverage of your code
2. **Test names**: Use descriptive names that explain what is tested
3. **Test independence**: Tests should not depend on each other
4. **Edge cases**: Test boundary conditions and error cases
5. **Use fixtures**: Share setup code with test fixtures

Example test:
```cpp
TEST_F(MyClassifierTest, HandlesEmptyDataset) {
    Dataset empty_dataset;
    std::vector<double> weights;
    
    EXPECT_NO_THROW(classifier->train(empty_dataset, weights));
}
```

## Documentation

- **Code comments**: Explain complex logic, not obvious code
- **Header documentation**: Document public APIs
- **README updates**: Update if adding new features
- **Examples**: Provide usage examples for new features

## Bug Reports

When reporting bugs, please include:

1. **Description**: Clear description of the issue
2. **Reproduction steps**: How to reproduce the bug
3. **Expected behavior**: What should happen
4. **Actual behavior**: What actually happens
5. **Environment**: OS, compiler version, Bazel version
6. **Code sample**: Minimal code to reproduce (if applicable)

## Questions and Support

- **Issues**: For bugs and feature requests
- **Discussions**: For questions and general discussion
- **Pull Requests**: For code contributions

## Code Review Process

All submissions require review. We use GitHub pull requests for this purpose.

Reviewers will check:
- Code quality and style
- Test coverage
- Documentation
- Performance implications
- API design

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (GNU General Public License v3.0 or later).

## Thank You!

Your contributions help make lakeml better for everyone. We appreciate your effort and time!
