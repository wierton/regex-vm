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
#include "regex-vm.hpp"

using namespace regvm;

void run_tests() {
  // Basic character matching
  assert(match(parse("a"), "a") == true);
  assert(match(parse("a"), "b") == false);
  assert(match(parse("abc"), "abc") == true);
  assert(match(parse("abc"), "abd") == false);

  // Alternation tests
  assert(match(parse("a|b"), "a") == true);
  assert(match(parse("a|b"), "b") == true);
  assert(match(parse("a|b"), "c") == false);
  assert(match(parse("cat|dog"), "cat") == true);
  assert(match(parse("cat|dog"), "dog") == true);
  assert(match(parse("cat|dog"), "rat") == false);

  // Kleene star tests
  assert(match(parse("a*"), "") == true);
  assert(match(parse("a*"), "a") == true);
  assert(match(parse("a*"), "aaa") == true);
  assert(match(parse("a*"), "b") == false);
  assert(match(parse("(ab)*"), "") == true);
  assert(match(parse("(ab)*"), "ab") == true);
  assert(match(parse("(ab)*"), "abab") == true);
  assert(match(parse("(ab)*"), "aba") == false);

  // Plus operator tests
  assert(match(parse("a+"), "") == false);
  assert(match(parse("a+"), "a") == true);
  assert(match(parse("a+"), "aaa") == true);
  assert(match(parse("(ab)+"), "") == false);
  assert(match(parse("(ab)+"), "ab") == true);
  assert(match(parse("(ab)+"), "abab") == true);

  // Complex patterns
  assert(match(parse("(a|b)*abb"), "abb") == true);
  assert(match(parse("(a|b)*abb"), "aabb") == true);
  assert(match(parse("(a|b)*abb"), "babb") == true);
  assert(match(parse("(a|b)*abb"), "ababb") == true);
  assert(match(parse("(a|b)*abb"), "abc") == false);

  // Nested groups
  assert(match(parse("((a|b)+)(c|d)"), "ac") == true);
  assert(match(parse("((a|b)+)(c|d)"), "bd") == true);
  assert(match(parse("((a|b)+)(c|d)"), "aaac") == true);
  assert(match(parse("((a|b)+)(c|d)"), "bbbd") == true);
  assert(match(parse("((a|b)+)(c|d)"), "e") == false);

  // Mixed operators
  assert(match(parse("a+(b|c)*d"), "ad") == true);
  assert(match(parse("a+(b|c)*d"), "abd") == true);
  assert(match(parse("a+(b|c)*d"), "abcd") == true);
  assert(match(parse("a+(b|c)*d"), "aaaabbbcd") == true);
  assert(match(parse("a+(b|c)*d"), "bcd") == false);

  // Edge cases
  assert(match(parse("(a|)*"), "") == true);
  assert(match(parse("(a|)*"), "a") == true);
  assert(match(parse("((a)|)*"), "aaa") == true);
  assert(match(parse("()*"), "") == true);

  // Long patterns
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "abcdefgh") == true);
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "ababefef") == true);
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "cdcdghgh") == true);
  assert(match(parse("(ab|cd)*(ef|gh)+"), "abcd") == false);

  cout << "All tests passed!" << endl;
}

int main() {
  run_tests();
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
