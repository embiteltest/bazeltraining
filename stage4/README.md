# Stage 4: Testing with Bazel and Google Test

## Objectives

By the end of this stage, you will:
- Write unit tests for C++ code using **Google Test (gtest)**
- Use `cc_test` to define test targets in Bazel
- Run tests with `bazel test`
- Understand test output and filtering
- Know how to add external dependencies via `MODULE.bazel`

---

## What Changed from Stage 3?

- Added **Google Test** as an external dependency in `MODULE.bazel`
- Created a dedicated `test/` package for tests
- Introduced `cc_test` build rule
- New libraries (`calculator`, `greeter`) with testable logic

---

## Project Structure

```
stage4/
├── MODULE.bazel              ← Now includes googletest dependency
├── main/
│   ├── BUILD
│   └── main.cc
├── lib/
│   ├── BUILD                 ← Libraries visible to main + test packages
│   ├── calculator.cc
│   ├── calculator.h
│   ├── greeter.cc
│   └── greeter.h
└── test/
    ├── BUILD                 ← Test targets using cc_test
    ├── calculator_test.cc
    └── greeter_test.cc
```

---

## Walkthrough

### Step 1: Adding Google Test Dependency

Look at `MODULE.bazel`:

```python
bazel_dep(name = "rules_cc", version = "0.1.0")
bazel_dep(name = "googletest", version = "1.15.2")
```

The `bazel_dep` line tells Bazel to download Google Test from the [Bazel Central Registry](https://registry.bazel.build/). No manual download or configuration needed!

### Step 2: Understanding `cc_test`

Look at `test/BUILD`:

```python
load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "calculator_test",
    srcs = ["calculator_test.cc"],
    deps = [
        "//lib:calculator",           # The library being tested
        "@googletest//:gtest_main",   # Google Test framework
    ],
)
```

**`cc_test` vs `cc_binary`:**
| Feature | `cc_binary` | `cc_test` |
|---------|-------------|-----------|
| Builds an executable | Yes | Yes |
| Runs with `bazel run` | Yes | Yes |
| Runs with `bazel test` | No | **Yes** |
| Reports pass/fail | No | **Yes** |
| Supports test filtering | No | **Yes** |
| Timeout/flaky handling | No | **Yes** |

**`@googletest//:gtest_main`** — Provides the `main()` function for your tests automatically, so you only write `TEST()` macros.

### Step 3: Writing Tests

Open `test/calculator_test.cc`:

```cpp
#include "lib/calculator.h"
#include <gtest/gtest.h>

TEST(CalculatorTest, Add) {
    EXPECT_EQ(calc::add(2, 3), 5);
    EXPECT_EQ(calc::add(-1, 1), 0);
}

TEST(CalculatorTest, DivideByZeroThrows) {
    EXPECT_THROW(calc::divide(1, 0), std::invalid_argument);
}
```

#### Key Google Test Macros

| Macro | Purpose |
|-------|---------|
| `EXPECT_EQ(a, b)` | Check `a == b` (continues on failure) |
| `ASSERT_EQ(a, b)` | Check `a == b` (stops test on failure) |
| `EXPECT_NE(a, b)` | Check `a != b` |
| `EXPECT_TRUE(x)` | Check `x` is true |
| `EXPECT_FALSE(x)` | Check `x` is false |
| `EXPECT_DOUBLE_EQ(a, b)` | Floating-point equality (with tolerance) |
| `EXPECT_THROW(expr, type)` | Check that `expr` throws `type` |
| `EXPECT_NO_THROW(expr)` | Check that `expr` doesn't throw |

### Step 4: Running Tests

```bash
cd stage4

# Run ALL tests in the workspace
bazel test //...

# Run a specific test
bazel test //test:calculator_test

# Run with verbose output (see individual test results)
bazel test //test:calculator_test --test_output=all

# Run tests matching a filter (Google Test filter)
bazel test //test:calculator_test --test_arg=--gtest_filter="CalculatorTest.Add"

# Run tests with detailed timing
bazel test //... --test_output=all --test_verbose_timeout_warnings
```

### Step 5: Understanding Test Output

```
//test:calculator_test    PASSED in 0.3s
//test:greeter_test       PASSED in 0.2s

Executed 2 out of 2 tests: 2 tests pass.
```

To see detailed output:
```bash
bazel test //test:calculator_test --test_output=all
```

```
[==========] Running 7 tests from 1 test suite.
[----------] 7 tests from CalculatorTest
[ RUN      ] CalculatorTest.Add
[       OK ] CalculatorTest.Add (0 ms)
[ RUN      ] CalculatorTest.Subtract
[       OK ] CalculatorTest.Subtract (0 ms)
...
[==========] 7 tests from 1 test suite ran. (1 ms total)
[  PASSED  ] 7 tests.
```

### Step 6: Visibility for Tests

Notice in `lib/BUILD`:
```python
cc_library(
    name = "calculator",
    srcs = ["calculator.cc"],
    hdrs = ["calculator.h"],
    visibility = ["//main:__pkg__", "//test:__pkg__"],
)
```

The library is visible to both `main` (for the application) and `test` (for testing).

---

## Architecture

```
     ┌──────────┐
     │   app    │  cc_binary (main/)
     └──┬────┬──┘
        │    │
   ┌────▼┐  ┌▼───────┐
   │calc.│  │greeter │  cc_library (lib/)
   └──┬──┘  └──┬─────┘
      │        │
   ┌──▼──────┐ ┌▼──────────┐
   │calc_test│ │greeter_test│  cc_test (test/)
   └─────────┘ └────────────┘
          │           │
     ┌────▼───────────▼────┐
     │  @googletest/gtest  │  External dependency
     └─────────────────────┘
```

---

7. Tests are **first-class citizens** in Bazel — cached, parallelized, and hermetic

---

**Previous**: [← Stage 3](../stage3/) | **Next**: [Stage 5 — Configurable Builds →](../stage5/)
