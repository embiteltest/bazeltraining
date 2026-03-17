# Stage 5: Configurable Builds, Platforms, and Compile Flags

## Objectives

By the end of this stage, you will:
- Use **`select()`** for conditional build logic
- Define **`config_setting`** rules for build configurations
- Create **custom build flags** with Starlark
- Understand **debug vs release** (`-c dbg` vs `-c opt`) builds
- Work with **platform-specific** code
- Use **`defines`** to pass preprocessor macros from BUILD files

---

## What Changed from Stage 4?

- Added a root `BUILD` file with `config_setting` definitions
- Libraries use **`select()`** for conditional compilation
- Custom **`--//:verbose`** build flag controls logging behavior
- Platform detection and build mode awareness
- Added `bazel_skylib` dependency for custom flags

---

## Project Structure

```
stage5/
├── MODULE.bazel              ← Includes bazel_skylib for custom flags
├── BUILD                     ← Root BUILD: config_setting definitions
├── main/
│   ├── BUILD
│   └── main.cc
├── lib/
│   ├── BUILD                 ← Uses select() for conditional defines
│   ├── logger.cc             ← Behavior changes based on build config
│   ├── logger.h
│   ├── platform.cc           ← OS detection
│   └── platform.h
└── test/
    ├── BUILD
    ├── logger_test.cc
    └── platform_test.cc
```

---

## Walkthrough

### Step 1: Understanding `config_setting`

Look at the root `BUILD` file:

```python
config_setting(
    name = "verbose_mode",
    flag_values = {
        "//:verbose": "True",
    },
)

config_setting(
    name = "opt_build",
    values = {
        "compilation_mode": "opt",
    },
)
```

A `config_setting` defines a **named condition** that can be referenced in `select()` statements. It matches when the given build flags have specific values.

| Setting | Matches When |
|---------|-------------|
| `verbose_mode` | `--//:verbose=True` is passed |
| `opt_build` | `-c opt` is passed |
| `dbg_build` | `-c dbg` is passed |
| `is_linux` | Building on/for Linux |
| `is_windows` | Building on/for Windows |

### Step 2: Custom Build Flags

```python
load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")

bool_flag(
    name = "verbose",
    build_setting_default = False,
)
```

This defines a custom boolean flag `--//:verbose` that defaults to `False`. You can pass it on the command line:

```bash
# Default (verbose = False)
bazel run //main:app

# With verbose enabled
bazel run //main:app --//:verbose=True
```

### Step 3: Using `select()` for Conditional Logic

Look at `lib/BUILD`:

```python
cc_library(
    name = "logger",
    srcs = ["logger.cc"],
    hdrs = ["logger.h"],
    defines = select({
        "//:verbose_mode": ["ENABLE_VERBOSE_LOGGING"],
        "//conditions:default": [],
    }),
)
```

**How `select()` works:**

```
select({
    condition_label_1: value_if_true,
    condition_label_2: value_if_true,
    "//conditions:default": fallback_value,
})
```

- Bazel evaluates conditions at **build time**
- The matching condition's value is used
- `//conditions:default` is the fallback when no condition matches
- Unlike `#ifdef` in C++, `select()` is resolved by the **build system**, not the preprocessor

### Step 4: Build and Compare

```bash
cd stage5

# --- Default build (no verbose logging) ---
bazel run //main:app
# Output: Only WARNING and ERROR messages shown

# --- Verbose build ---
bazel run //main:app --//:verbose=True
# Output: All log messages shown with timestamps

# --- Debug build (default) ---
bazel run //main:app -c dbg
# Shows "Build: Debug"

# --- Optimized release build ---
bazel run //main:app -c opt
# Shows "Build: Release (optimized)"

# --- Combined: verbose + optimized ---
bazel run //main:app --//:verbose=True -c opt
```

### Step 5: Platform Detection

The `platform.cc` file uses C++ preprocessor macros to detect the OS:

```cpp
std::string get_os_name() {
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown OS";
#endif
}
```

While this uses C++ preprocessor (not Bazel's `select()`), Bazel also provides platform-aware `config_setting` via constraint values for build-system-level platform logic.

### Step 6: Run Tests

```bash
# Run all tests
bazel test //...

# Tests with verbose logging enabled
bazel test //... --//:verbose=True --test_output=all

# Tests in release mode
bazel test //... -c opt --test_output=all
```

---

## Deep Dive: `select()` vs C++ `#ifdef`

| Feature | `select()` (Bazel) | `#ifdef` (C++) |
|---------|-------------------|---------------|
| Where it runs | Build system | Preprocessor |
| What it controls | Build rules, deps, flags | Source code |
| Can switch dependencies? | **Yes** | No |
| Can switch source files? | **Yes** | No |
| Visible in BUILD files? | **Yes** | No (hidden in code) |
| IDE support | Starlark-aware | C++ analyzer |

**When to use `select()`:**
- Switching between different source files per platform
- Adding/removing dependencies based on build config
- Setting different compiler flags (defines, copts)

**When to use `#ifdef`:**
- Small in-code differences (specific code blocks)
- Standard platform detection macros
- Feature toggling within a single source file

---

## Advanced: `.bazelrc` for Default Flags

Create a `.bazelrc` file in the workspace root to set default flags:

```
# .bazelrc — Default build settings

# Always use verbose logging in debug builds
build:dev --//:verbose=True
build:dev -c dbg

# Release profile
build:release -c opt
build:release --strip=always

# Usage:
#   bazel build --config=dev //main:app
#   bazel build --config=release //main:app
```

---



---

## Summary of What You've Learned Across All 5 Stages

| Stage | Key Skills |
|-------|-----------|
| 1 | Workspace setup, `cc_binary`, labels, basic build/run |
| 2 | `cc_library`, `deps`, `hdrs` vs `srcs`, incremental builds |
| 3 | Multiple packages, `visibility`, cross-package deps |
| 4 | `cc_test`, Google Test, external dependencies, `bazel test` |
| 5 | `select()`, `config_setting`, custom flags, platform builds |

**You're now equipped to use Bazel for real C++ projects!**

---

**Previous**: [← Stage 4](../stage4/) | **Back to**: [Main README](../README.md)
