# Stage 2: Multiple Targets — Libraries and Dependencies

## Objectives

By the end of this stage, you will:
- Understand `cc_library` and how to create reusable libraries
- Learn how `deps` connects targets together
- Know the difference between `srcs` and `hdrs`
- Visualize the dependency graph

---

## What Changed from Stage 1?

In Stage 1, **everything** was in a single file. That's fine for tiny programs, but real projects split code into reusable pieces.

In Stage 2, we:
1. Extracted the `get_greet()` function into its own **library**
2. Made the binary **depend** on that library

---

## Project Structure

```
stage2/
├── MODULE.bazel
└── main/
    ├── BUILD               ← Now defines TWO targets
    ├── hello-greet.cc      ← Library implementation
    ├── hello-greet.h       ← Library header
    └── hello-world.cc      ← Binary (uses the library)
```

---

## Walkthrough

### Step 1: Understand the Library Target

Look at `main/BUILD`:

```python
load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
    ],
)
```

Let's understand the new concepts:

### `cc_library` — Creating a Reusable Library

```python
cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],    # Implementation files (private)
    hdrs = ["hello-greet.h"],     # Header files (public API)
)
```

| Attribute | Purpose |
|-----------|---------|
| `name` | Target label for this library |
| `srcs` | Source files — compiled but NOT exposed to dependents |
| `hdrs` | Header files — the public API visible to targets that depend on this library |

> **Key insight**: `srcs` are private, `hdrs` are public. This enforces encapsulation at the build system level!

### `deps` — Declaring Dependencies

```python
cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",        # ← Depends on the library above
    ],
)
```

- `:hello-greet` is a shorthand label for a target **in the same package**
- It's equivalent to `//main:hello-greet`
- Bazel ensures `hello-greet` is built **before** `hello-world`

### Step 2: See the Dependency Graph

```
     ┌─────────────────┐
     │   hello-world    │  cc_binary
     │  (main package)  │
     └────────┬─────────┘
              │ deps
     ┌────────▼─────────┐
     │   hello-greet     │  cc_library
     │  (main package)   │
     └──────────────────┘
```

### Step 3: Build and Run

```bash
cd stage2

# Build just the library
bazel build //main:hello-greet

# Build the binary (this also builds hello-greet automatically)
bazel build //main:hello-world

# Run it
bazel run //main:hello-world
bazel run //main:hello-world -- "Stage 2 learner"

# Query the dependency graph
bazel query 'deps(//main:hello-world)'
```

### Step 4: Observe Caching

```bash
# First build — compiles everything
bazel build //main:hello-world

# Change ONLY hello-greet.cc (e.g., modify the greeting string)
# Then rebuild:
bazel build //main:hello-world
```

Notice: Bazel only recompiles what changed! The binary is re-linked, but unchanged files are not recompiled. This is **incremental building** — one of Bazel's core strengths.

---

## Deep Dive: srcs vs hdrs

This is a critical distinction:

```
┌──────────────────────────────────────────────┐
│  cc_library(name = "hello-greet")            │
│                                              │
│  hdrs = ["hello-greet.h"]   ← PUBLIC         │
│  srcs = ["hello-greet.cc"]  ← PRIVATE        │
│                                              │
│  Dependents CAN #include "hello-greet.h"     │
│  Dependents CANNOT #include "hello-greet.cc" │
└──────────────────────────────────────────────┘
```

**Why this matters:**
- **Encapsulation**: Implementation details stay hidden
- **Build correctness**: Bazel enforces include rules at build time
- **Compile speed**: Changes to `.cc` files don't trigger recompilation of dependents (only re-linking)

---

## Windows Notes (Git Bash)

If you are using **Git Bash** on Windows, the `//` prefix in Bazel labels gets converted to a Windows path. To fix this, prefix commands with `MSYS_NO_PATHCONV=1`:

```bash
# Git Bash — required prefix
MSYS_NO_PATHCONV=1 bazel build //main:hello-world
MSYS_NO_PATHCONV=1 bazel build //main:hello-greet
MSYS_NO_PATHCONV=1 bazel run //main:hello-world
MSYS_NO_PATHCONV=1 bazel query 'deps(//main:hello-world)'
```

Alternatively, use **PowerShell** or **CMD** where `//` works as-is.

> **`rules_cc` version warning**: You may see a warning that `rules_cc@0.1.0` was resolved to `rules_cc@0.2.17`. This is harmless — Bazel auto-upgrades to the latest compatible version. To suppress it, update `MODULE.bazel`:
> ```python
> bazel_dep(name = "rules_cc", version = "0.2.17")
> ```

---

## Exercises

### Exercise 2.1: Add a new library
Create a `math-utils` library with a function `int factorial(int n)`. Make `hello-world` depend on it and print `factorial(5)` along with the greeting.

<details>
<summary>Hint</summary>

Create `main/math-utils.h`:
```cpp
#ifndef MAIN_MATH_UTILS_H_
#define MAIN_MATH_UTILS_H_

int factorial(int n);

#endif
```

Create `main/math-utils.cc`:
```cpp
#include "math-utils.h"

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

Add to `main/BUILD`:
```python
cc_library(
    name = "math-utils",
    srcs = ["math-utils.cc"],
    hdrs = ["math-utils.h"],
)
```

Update the `hello-world` target's deps:
```python
    deps = [
        ":hello-greet",
        ":math-utils",
    ],
```
</details>

### Exercise 2.2: Break a dependency
Try removing `:hello-greet` from `deps` and rebuild. What error do you see? This demonstrates Bazel's strict dependency checking.

### Exercise 2.3: Build individual targets
Build just the library with `bazel build //main:hello-greet`. Verify the `.a` (or `.lib`) file is generated in `bazel-bin/`.

---

## Key Takeaways

1. **`cc_library`** creates reusable code modules with clear public APIs
2. **`hdrs`** = public headers, **`srcs`** = private implementation
3. **`deps`** declares explicit dependencies between targets
4. Bazel enforces **dependency correctness** — you can't use undeclared deps
5. **Incremental builds** — only changed targets are rebuilt
6. Multiple targets can live in the **same BUILD file** (same package)

---

**Previous**: [← Stage 1](../stage1/) | **Next**: [Stage 3 — Multiple Packages →](../stage3/)
