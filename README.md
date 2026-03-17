# Bazel C++ Hands-On Training

A progressive, hands-on training repository for learning **Bazel** build system with **C++**. Each stage builds on the previous one, introducing new concepts step-by-step.

> **Reference**: Based on the official [Bazel C++ Tutorial](https://github.com/bazelbuild/examples/tree/main/cpp-tutorial) with extended examples, detailed explanations, and local setup instructions.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [System Setup](#system-setup)
  - [Windows](#windows-setup)
  - [Linux (Ubuntu/Debian)](#linux-setup)
- [Verify Installation](#verify-installation)
- [Bazel Key Concepts](#bazel-key-concepts)
- [Training Stages](#training-stages)
- [Cheat Sheet](#cheat-sheet)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

| Tool | Minimum Version | Purpose |
|------|----------------|---------|
| Bazel | 7.0+ | Build system |
| C++ Compiler | GCC 11+ / Clang 14+ / MSVC 2022 | Compiling C++ code |
| Git | 2.30+ | Version control |
| Python | 3.8+ | Required by Bazel internally |

---

## System Setup

### Windows Setup

#### Option A: Using Bazelisk (Recommended)

[Bazelisk](https://github.com/bazelbuild/bazelisk) is a launcher for Bazel that automatically downloads the right version.

```powershell
# 1. Install Chocolatey (if not already installed)
# Open PowerShell as Administrator:
Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# 2. Install Bazelisk (manages Bazel versions automatically)
choco install bazelisk -y

# 3. Install MSYS2 (provides Unix tools needed by Bazel)
choco install msys2 -y

# 4. Install Visual Studio Build Tools 2022 (C++ compiler)
choco install visualstudio2022buildtools --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools" -y
```

#### Option B: Using Scoop

```powershell
# Install Scoop
irm get.scoop.sh | iex

# Install Bazelisk and GCC
scoop install bazelisk
scoop install gcc
```

#### Windows Environment Variables

Add the following to your system PATH:
```
C:\tools\msys64\usr\bin
```

Set the following environment variable:
```powershell
[Environment]::SetEnvironmentVariable("BAZEL_SH", "C:\tools\msys64\usr\bin\bash.exe", "User")
```

### Linux Setup

```bash
# ---- Ubuntu / Debian ----

# 1. Install prerequisites
sudo apt update
sudo apt install -y build-essential g++ git python3 zip unzip curl

# 2. Install Bazelisk
curl -L https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 -o /usr/local/bin/bazel
sudo chmod +x /usr/local/bin/bazel

# 3. Verify GCC
gcc --version  # Should show 11+
```

```bash
# ---- Fedora / RHEL ----

sudo dnf install -y gcc-c++ git python3 zip unzip curl
curl -L https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 -o /usr/local/bin/bazel
sudo chmod +x /usr/local/bin/bazel
```

---

## Verify Installation

After setup, verify everything works:

```bash
# Check Bazel version
bazel --version
# Expected: bazel 7.x.x (or higher)

# Check C++ compiler
# Linux:
g++ --version
# macOS:
clang++ --version
# Windows (MSVC Developer Command Prompt):
cl

# Quick test - clone this repo and build Stage 1
git clone <this-repo-url>
cd bazel-cpp-training/stage1
bazel build //main:hello-world
bazel run //main:hello-world
```

If you see `Hello world` followed by the current time, your setup is working!

---

## Bazel Key Concepts

Before diving into the stages, understand these core concepts:

### Workspace
A directory containing source files and a `MODULE.bazel` file (or legacy `WORKSPACE` file). This is the root of your Bazel project.

### Packages
A directory within the workspace that contains a `BUILD` file. Packages organize related source files and build rules.

### Targets
Individual buildable units defined in `BUILD` files. Common target types:
- **`cc_binary`** — An executable program
- **`cc_library`** — A reusable library (static/shared)
- **`cc_test`** — A test executable

### Labels
The naming scheme Bazel uses to identify targets:
```
//package_name:target_name

Examples:
  //main:hello-world       → target "hello-world" in package "main"
  //lib:math-utils         → target "math-utils" in package "lib"
  :hello-greet             → target in the current package (shorthand)
```

### BUILD Files
Define **what** to build and **how** targets relate to each other. Written in Starlark (a Python-like language).

### MODULE.bazel
Declares external dependencies your project needs (e.g., `rules_cc` for C++ build rules).

### Dependency Graph
Bazel models your project as a **directed acyclic graph (DAG)**. Each target declares its dependencies, and Bazel builds them in the correct order, caching results for speed.

```
     ┌─────────────┐
     │ hello-world  │  (cc_binary)
     │   (main)     │
     └──────┬───────┘
            │ deps
     ┌──────┴───────┐
     │              │
┌────▼─────┐  ┌─────▼─────┐
│hello-greet│  │hello-time │  (cc_library)
│  (main)   │  │  (lib)    │
└───────────┘  └───────────┘
```

---

## Training Stages

| Stage | Concepts | Description |
|-------|----------|-------------|
| [Stage 1](stage1/) | `cc_binary`, `BUILD`, `MODULE.bazel` | Single target — build a Hello World binary |
| [Stage 2](stage2/) | `cc_library`, `deps`, `hdrs` | Multiple targets — split code into a library + binary |
| [Stage 3](stage3/) | Multiple packages, `visibility` | Cross-package dependencies with visibility rules |
| [Stage 4](stage4/) | `cc_test`, Google Test, `data` | Writing and running unit tests with Bazel |
| [Stage 5](stage5/) | `select()`, `config_setting`, platforms | Configurable builds, platform-specific code, optimization |

### Recommended Flow

```
Stage 1 ──▶ Stage 2 ──▶ Stage 3 ──▶ Stage 4 ──▶ Stage 5
(basics)    (libs)     (packages)   (testing)   (advanced)
```

Each stage has its own `README.md` with:
- **Objectives**: What you'll learn
- **Walkthrough**: Step-by-step explanation
- **Exercises**: Practice tasks
- **Key Takeaways**: Summary of concepts

---

## Cheat Sheet

### Common Bazel Commands

```bash
# Build a target
bazel build //package:target

# Run a binary target
bazel run //package:target

# Run with arguments
bazel run //package:target -- arg1 arg2

# Run tests
bazel test //package:target
bazel test //...                    # Run ALL tests

# Clean build artifacts
bazel clean
bazel clean --expunge              # Full clean (removes cache)

# Query the dependency graph
bazel query 'deps(//main:hello-world)'

# Visualize dependencies (requires Graphviz)
bazel query 'deps(//main:hello-world)' --output graph | dot -Tpng -o deps.png

# Show build rule for a target
bazel query //main:hello-world --output build

# Find all targets in a package
bazel query //main:all

# Find all targets in entire workspace
bazel query //...
```

### Build Flags

```bash
# Debug build (default)
bazel build -c dbg //main:hello-world

# Optimized release build
bazel build -c opt //main:hello-world

# Show verbose compiler commands
bazel build -s //main:hello-world

# Set number of parallel jobs
bazel build --jobs=4 //main:hello-world
```

---



### Getting Help

```bash
# Built-in help
bazel help
bazel help build
bazel help query

# Check Bazel server status
bazel info

# Show where Bazel stores outputs
bazel info output_base
bazel info execution_root
```

---

## License

This training material is provided for educational purposes. The examples are inspired by the [official Bazel examples](https://github.com/bazelbuild/examples) (Apache 2.0 License).
