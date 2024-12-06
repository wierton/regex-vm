# Regular Expression Virtual Machine 🚀
> A lightning-fast regular expression engine implemented in modern C++, just for fun :)

## 🌟 Overview

This project implements a virtual machine for regular expression pattern matching, based on Thompson's construction algorithm. It combines elegance with efficiency, providing a clean and performant solution for regex operations.

## ✨ Key Features

- 🔄 **Composable Operations**
  - Concatenation (implicit joining)
  - Alternation (`|`) - Choose between patterns
  - Kleene star (`*`) - Match zero or more
  - Plus (`+`) - Match one or more
  - Grouping `()` - Control precedence

- ⚡ **Performance Optimized**
  - Efficient state management
  - O(mn) time complexity
  - Memory-efficient implementation

## 🛠️ Technical Architecture

### VM Instructions
```cpp
enum vop_t {
    vop_split,  // Branch execution
    vop_jump,   // Control flow
    vop_match,  // Character matching
    vop_accept  // Pattern completion
};
```

### Core Components

1. **🎯 Parser Engine**
   - Converts regex patterns to VM instructions
   - Handles operator precedence
   - Manages concatenation implicitly

2. **⚙️ Virtual Machine**
   - Stack-based execution model
   - Non-deterministic computation
   - Efficient state tracking

## 🚀 Quick Start

```cpp
#include "regex_vm.hpp"

int main() {
    // Match a string against pattern "(ab|ba)*"
    bool result = match(parse("(ab|ba)*"), "ababbababaabab");
    std::cout << (result ? "Match!" : "No match") << std::endl;
    return 0;
}
```

## 🔧 Build Instructions

```bash
# Build with CMake
cd regex-vm
g++ main.cc -o regex-vm.test
./regex-vm.test
```

## 📊 Performance Metrics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Parsing   | O(m)          | O(m)            |
| Matching  | O(mn)         | O(m)            |

Where:
- m = pattern length
- n = input string length

## 📚 Requirements

- C++17 compatible compiler
- Standard Template Library (STL)
