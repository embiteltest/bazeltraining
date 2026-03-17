# Stage 3: Multiple Packages and Visibility

## Objectives

By the end of this stage, you will:
- Organize code into multiple **packages** (directories with their own BUILD files)
- Understand **visibility** rules and why they matter
- Use cross-package dependencies with full labels
- Know when and how to control access between packages

---

## What Changed from Stage 2?

In Stage 2, all targets lived in a single package (`main/`). In real projects, code is organized into multiple packages:

- **`main/`** — Application entry point
- **`lib/`** — Shared utility libraries

Now the `print_localtime()` function lives in a separate `lib` package, demonstrating cross-package dependencies.

---

## Project Structure

```
stage3/
├── MODULE.bazel
├── main/                    ← Package 1: Application
│   ├── BUILD
│   ├── hello-greet.cc
│   ├── hello-greet.h
│   └── hello-world.cc
└── lib/                     ← Package 2: Shared library
    ├── BUILD
    ├── hello-time.cc
    └── hello-time.h
```

---

## Walkthrough

### Step 1: Two Packages, Two BUILD Files

Each directory with a `BUILD` file is a **package**. We now have:

**`lib/BUILD`** — Defines the time utility library:
```python
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "hello-time",
    srcs = ["hello-time.cc"],
    hdrs = ["hello-time.h"],
    visibility = ["//main:__pkg__"],    # ← NEW: Visibility control
)
```

**`main/BUILD`** — Depends on both local and cross-package libraries:
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
        ":hello-greet",           # Same-package dependency
        "//lib:hello-time",       # Cross-package dependency ← NEW
    ],
)
```

### Step 2: Understanding Visibility

By default, targets are **private** — only visible within their own package. To allow other packages to use your library, you must set `visibility`.

```python
visibility = ["//main:__pkg__"],
```

This means: "Only the `main` package can depend on this target."

#### Visibility Options Reference

| Visibility | Meaning |
|-----------|---------|
| `["//visibility:private"]` | Only targets in the same package (default) |
| `["//main:__pkg__"]` | Only the `main` package |
| `["//main:__subpackages__"]` | The `main` package and all sub-packages below it |
| `["//visibility:public"]` | Any package can depend on this |
| `["//main:__pkg__", "//test:__pkg__"]` | Multiple specific packages |

> **Best Practice**: Use the most restrictive visibility that works. Avoid `//visibility:public` unless truly needed.

### Step 3: Cross-Package Include Paths

Notice how `#include` paths changed:

```cpp
// In Stage 2 (same package):
#include "hello-greet.h"

// In Stage 3 (cross-package):
#include "lib/hello-time.h"
#include "main/hello-greet.h"
```

When referencing headers across packages, use the **full path from the workspace root**.

### Step 4: Build and Run

```bash
cd stage3

# Build everything
bazel build //main:hello-world

# Run it
bazel run //main:hello-world -- "Stage 3 learner"

# Query the full dependency graph
bazel query 'deps(//main:hello-world)'

# See the graph structure
bazel query 'deps(//main:hello-world)' --output graph
```

### Step 5: Visualize the Architecture

```
     ┌──────────────────┐
     │   hello-world     │  cc_binary  (main/)
     └──┬───────────┬────┘
        │           │
   deps │      deps │
        │           │
┌───────▼───┐  ┌────▼────────┐
│hello-greet│  │ hello-time  │  cc_library
│  (main/)  │  │  (lib/)     │
└───────────┘  └─────────────┘
                visibility:
               [//main:__pkg__]
```

---

## Deep Dive: Why Visibility Matters

Imagine a project with 100 packages. Without visibility:
- Any package could depend on any other → **spaghetti dependencies**
- Refactoring becomes risky — you don't know who uses what
- Build times suffer — unnecessary rebuilds cascade

With visibility rules:
- Dependencies are **explicit and controlled**
- Package owners decide who can use their code
- Bazel **enforces** these rules at build time — violations cause build errors

### Experiment: Break Visibility

Try changing `lib/BUILD` to:
```python
    visibility = ["//visibility:private"],
```

Then rebuild:
```bash
bazel build //main:hello-world
# ERROR: target '//lib:hello-time' is not visible from target '//main:hello-world'
```

This is Bazel protecting your architecture.

---

## Windows Notes (Git Bash)

If you are using **Git Bash** on Windows, the `//` prefix in Bazel labels gets converted to a Windows path. To fix this, prefix commands with `MSYS_NO_PATHCONV=1`:

```bash
# Git Bash — required prefix
MSYS_NO_PATHCONV=1 bazel build //main:hello-world
MSYS_NO_PATHCONV=1 bazel run //main:hello-world -- "Stage 3 learner"
MSYS_NO_PATHCONV=1 bazel query 'deps(//main:hello-world)'
MSYS_NO_PATHCONV=1 bazel query 'deps(//main:hello-world)' --output graph
```

Alternatively, use **PowerShell** or **CMD** where `//` works as-is.

> **`rules_cc` version warning**: You may see a warning that `rules_cc@0.1.0` was resolved to `rules_cc@0.2.17`. This is harmless — Bazel auto-upgrades to the latest compatible version. To suppress it, update `MODULE.bazel`:
> ```python
> bazel_dep(name = "rules_cc", version = "0.2.17")
> ```

---

## Exercises

### Exercise 3.1: Add a new package
Create a `utils/` package with a `string-utils` library that has a function `std::string to_upper(const std::string& s)`. Use it in `hello-world` to print the greeting in uppercase.

### Exercise 3.2: Visibility experiment
1. Set `hello-time` visibility to `["//visibility:private"]` and try to build. Observe the error.
2. Set it to `["//visibility:public"]` and try to build.
3. Set it to `["//main:__pkg__"]` (correct minimal visibility).

### Exercise 3.3: Add a sub-package
Create `lib/format/BUILD` with a formatting library. Set the visibility of `hello-time` to `["//lib:__subpackages__"]` and verify that `lib/format` can use it but `main` cannot.

---

## Key Takeaways

1. **Packages** = directories with BUILD files — they organize your codebase
2. **Visibility** controls which packages can depend on a target
3. Use `//package:target` for **cross-package** dependencies
4. Use `:target` for **same-package** dependencies
5. Include paths use the **workspace-relative path** for cross-package headers
6. Default visibility is **private** — explicitly grant access as needed
7. Visibility is a **build-time enforcement** of your architecture

---

**Previous**: [← Stage 2](../stage2/) | **Next**: [Stage 4 — Testing with Bazel →](../stage4/)
