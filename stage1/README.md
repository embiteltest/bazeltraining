# Stage 1: Your First Bazel Build — Hello World

## Objectives

By the end of this stage, you will:
- Understand the structure of a Bazel workspace
- Know what `MODULE.bazel` and `BUILD` files do
- Build and run your first C++ binary with Bazel
- Understand the `cc_binary` rule

---

## Project Structure

```
stage1/
├── MODULE.bazel          ← Workspace definition + external dependencies
└── main/
    ├── BUILD             ← Build rules for this package
    └── hello-world.cc    ← Source code
```

---

## Walkthrough

### Step 1: Understand MODULE.bazel

Open `MODULE.bazel`:

```python
bazel_dep(name = "rules_cc", version = "0.1.0")
```

**What this does:**
- Declares this directory as a Bazel workspace
- Pulls in `rules_cc` — the official C++ build rules for Bazel
- Bazel uses the [Bazel Central Registry (BCR)](https://registry.bazel.build/) to find and download `rules_cc`

> **Note**: In older Bazel versions (< 7.0), you would use a `WORKSPACE` file instead. `MODULE.bazel` is the modern approach using [Bzlmod](https://bazel.build/external/overview#bzlmod).

### Step 2: Understand the BUILD File

Open `main/BUILD`:

```python
load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
)
```

Let's break this down line by line:

| Line | Meaning |
|------|---------|
| `load(...)` | Imports the `cc_binary` rule from the `rules_cc` module |
| `cc_binary(...)` | Defines a build target that produces an executable |
| `name = "hello-world"` | The target's label — used in build commands |
| `srcs = [...]` | List of C++ source files to compile |

### Step 3: Review the Source Code

Open `main/hello-world.cc` — it's a simple C++ program that:
1. Takes an optional name argument
2. Prints a greeting
3. Prints the current local time

### Step 4: Build It

```bash
cd stage1

# Build the target
bazel build //main:hello-world
```

**What happens when you run this:**
1. Bazel reads `MODULE.bazel` and downloads `rules_cc`
2. Bazel reads `main/BUILD` and finds the `hello-world` target
3. Bazel compiles `hello-world.cc` using your system's C++ compiler
4. The binary is placed in `bazel-bin/main/hello-world`

### Step 5: Run It

```bash
# Run using Bazel
bazel run //main:hello-world

# Run with an argument
bazel run //main:hello-world -- "Bazel trainee"

# Or run the binary directly
./bazel-bin/main/hello-world
./bazel-bin/main/hello-world "Bazel trainee"
```

**Expected output:**
```
Hello Bazel trainee
Mon Mar 17 10:30:00 2026
```

### Step 6: Explore Bazel Commands

```bash
# See what Bazel built
bazel info output_base

# Query the target
bazel query //main:hello-world --output build

# See all targets in the workspace
bazel query //...

# Clean build outputs
bazel clean
```

---

## Understanding the Label System

The build command uses **labels** to identify targets:

```
bazel build //main:hello-world
             ──┬──  ────┬─────
               │        │
          package    target name
          (directory   (from BUILD
           with BUILD   file)
           file)
```

- `//` means "relative to the workspace root" (where `MODULE.bazel` lives)
- `main` is the package (a directory containing a `BUILD` file)
- `hello-world` is the target name (defined by `name = "hello-world"`)

---

## Windows Notes (Git Bash)

If you are using **Git Bash** on Windows, the `//` prefix in Bazel labels (e.g., `//main:hello-world`) gets converted to a Windows path. To fix this, prefix commands with `MSYS_NO_PATHCONV=1`:

```bash
# Git Bash — required prefix
MSYS_NO_PATHCONV=1 bazel build //main:hello-world
MSYS_NO_PATHCONV=1 bazel run //main:hello-world
```

Alternatively, use **PowerShell** or **CMD** where `//` works as-is.

> **`rules_cc` version warning**: You may see a warning that `rules_cc@0.1.0` was resolved to `rules_cc@0.2.17`. This is harmless — Bazel auto-upgrades to the latest compatible version. To suppress it, update `MODULE.bazel`:
> ```python
> bazel_dep(name = "rules_cc", version = "0.2.17")
> ```

---

## Exercises

### Exercise 1.1: Modify the greeting
Change the greeting format to include the current date and your own custom message. Rebuild and run.

### Exercise 1.2: Add a second binary
Add another `cc_binary` target in the `BUILD` file for a new source file `main/goodbye.cc` that prints a farewell message. Build it with `bazel build //main:goodbye`.

<details>
<summary>Hint</summary>

Create `main/goodbye.cc`:
```cpp
#include <iostream>

int main() {
    std::cout << "Goodbye, see you at Stage 2!" << std::endl;
    return 0;
}
```

Add to `main/BUILD`:
```python
cc_binary(
    name = "goodbye",
    srcs = ["goodbye.cc"],
)
```

Build: `bazel build //main:goodbye`
</details>

### Exercise 1.3: Explore query commands
Run `bazel query //main:all` to list all targets. What do you see?

---

## Key Takeaways

1. **`MODULE.bazel`** defines the workspace root and external dependencies
2. **`BUILD`** files define targets (what to build) inside packages
3. **`cc_binary`** creates executable programs from C++ source files
4. **Labels** like `//main:hello-world` uniquely identify targets
5. **Bazel caches** build results — rebuilds are incremental and fast
6. First build is slow (downloads deps), subsequent builds are near-instant

---

**Next**: [Stage 2 — Multiple Targets and Libraries →](../stage2/)
