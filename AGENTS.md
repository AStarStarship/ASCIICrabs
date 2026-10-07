# Agent Rules for Crabs

## Kanban board

This project belongs to the **AStarship** organization; its Kanban board slug is `astarship` (see `~/AStarStarship/AGENTS.md`). Always target `astarship` explicitly (`--board astarship` / `board="astarship"`). Never rely on the current-board pointer. If a task's assigned board differs, report the mismatch before acting — do not silently switch.

This file contains rules and guidelines for AI agents operating in the Crabs repository.

## 1. Build, Lint, and Test Commands

### Build
- **Compile:** `g++ -g -o Crabs _Seams/_main.cpp`
- **Output Binary:** `Crabs` (Windows: `Crabs.exe`)

### Test
The project uses a custom seam tree testing framework located in `_Seams/`. You change the SEAM macro to one of the seams in the `_Seams.h` file, then all the debug information changes and a different unit test is runs.

- **Run All Tests:** `./Crabs` (Runs the compiled executable)
- **Run Specific Seam:** Tests are organized in a tree structure. To debug a specific area, you may need to modify `_Seams/_main.cpp` or set breakpoints in `Test.hpp` as described in `README.md`.
- **Debugging:**
  - Breakpoints: `Test.hpp` contains a `TBRK` function intended for one breakpoints.
  - Setup: Copy `_Seams` directory to workspace if needed (per `README.md`).

### Lint / Standards
- **Style Check:** There is no automated linter configured (e.g., clang-format) visible in the root.

## 2. Code Style & Conventions

All Crabs code shall use Chimera C++ formatting where all immutable members are UpperCamelCase and all mutable members are lower_snake_case with 2-space tabs and the root namespace isn't indented. The first curly bracket is inline, 80 character lines except for the description Doxygen tags, which must be all one line, and when required.

### General Philosophy
- **ASCII Data Types:** The project uses custom [ASCII Data Types](./_Spec/Data) (e.g., `ISN`, `BOL`, `CHR`, `IUW`, `DTW`) instead of standard C++ types like `int`, `bool`, `char`.
  - **Mapping:**
    - `ISN` -> Integer Signed N-bit (likely platform dependent width, often `int` or `long long`)
    - `BOL` -> Boolean
    - `CHR`, `CHA` -> char (ASCII)
    - `IUW` -> Unsigned Integer Word (machine word size)
    - `DTW` -> Data Type Word
- **Minimal Standard Library:** The project aims to replace the C++ std library. Avoid `#include <string>`, `<vector>`, `<iostream>` unless explicitly allowed or wrapping legacy code. Use project-specific containers (`Array`, `List`, `Stack`, `String`) instead.
- **Crabs Module Format:** Follow the module structure: `.h` (Contains the decl), `.hpp` (Contains all code that uses templates), `.hxx` (contains the impl usually found in .ccp files because we use only one translation unit to compile as fast as possible).

### Naming Conventions
This naming convention is called Chimera case due to the mix of snake_case and CamelCase.,
- **Types:** CamelCase (`MyExcitingClass`, `UrlTable`).
- **Variables:** snake_case (`my_local_variable`, `num_errors`).
- **Class Members:** snake_case with trailing underscore (`table_name_`).
- **Struct Members:** snake_case *without* trailing underscore (`name`, `num_entries`).
- **Constants/Globals:** CamelCase.
- **Enums:**
  - Type Name: CamelCase (`UrlTableErrors`).
- **Functions:** CamelCase (`StartRpc()`, `OpenFileOrDie()`).
  - Accessors/Mutators: snake_case allowed (`count()`, `set_count()`).
- **Namespaces:** snake_case, top-level matches project/team. No nested `std`.
- **Macros:** UPPER_SNAKE_CASE (Macros are mostly used for changing seams/unit tests).

### Formatting
- **Indentation:** 2 spaces (inferred from `AType.h`).
- **Braces:** K&R style / Google style (attached to the line definition).
- **Line Length:** generally 80 chars, but be reasonable.
- **Pointer/Ref:** Attached to type or variable? `AType.h` shows `void* begin` (attached to type).
- **Control Flow:** `if`, `for`, `while`, `switch` have a space between keyword and parenthesis. `else` goes on same line as closing brace.

### File Structure
- **Headers:** `.h` for declarations.
- **Implementation:** `.hpp` or `.hxx` (often included at bottom of `.h` or compiled separately).
- **Guards:** `#ifndef Crabs_SEAM_H` style.
- **Copyright:** Include `// Copyright AStarship <https://astarship.net>.` at the top followed by one blank line.
- **Includes:** Use `""` for local project files, `<>` for system/config files (e.g., `#include <_Config.h>`).

### Error Handling
- **No Exceptions:** The project likely relies on error codes or assertions (`IsError`).
- **Debugging:** Use `TBRK()` for breakpoints. Use `StdOut()` (wrapped) for logging if strictly necessary, but prefer the "Seam" test structure.

### Specific Types (from `AType.h` and `_Spec/Data`)
- 'CHA', 'CHB', 'CHC', 'CHR` are char, char16_t, char32_t, and the default/OS character type. Prefer CHR unless otherwise specified.
- `DTW`, `DTB`: Data Type Word/Byte.
- `IUW`, `IUE`, `IUD`: Integer Unsigned Word/128-bit/64-bit.
- `ISW`, `ISE`: Integer Signed Word/128-bit.
- `FPC`, `FPD`, `FPE`: Floating Point types.
- `PTR`: Pointer macros or wrappers may exist.

### Classes
- **Constructors:** Do no work in constructors that can fail. Use `Init()` if needed.
- **Explicit:** Mark single-arg constructors `explicit`.
- **Copy/Move:** Explicitly declare or delete copy/move operations (`= default`, `= delete`) in public section.
- **Structs vs Classes:** Use `struct` only for passive data (public members, no logic). Use `class` for everything else (private data with trailing underscore).
- **Inheritance:** Public inheritance only. Composition preferred.

### Autojects and RAMFactory
- **Autoject:** An "Automatic Object" that manages memory dynamically via a `RAMFactory`. It consists of a pointer (`origin`) and a memory management function (`ram`).
- **RAMFactory:** A function pointer type (`IUW* (*RAMFactory)(IUW* origin, ISW bytes)`) used to allocate, resize, or delete memory.
- **Seam Injection:** The `Autoject` design facilitates dependency injection and testing "seams" by allowing you to swap out the memory factory (`RAMFactory`) or the underlying data buffer (`origin`). This is crucial for isolating components during testing.

## 3. Important Directories
- `_Seams/`: Test suites (Seams).
- `_Config.h`: Project configuration.

## 4. AI Behavior Rules
- **Verify before Implement:** Always read `_StyleGuide` files if unsure about a naming or structural convention.
- **Mimic Context:** The codebase uses heavily customized types. Do NOT introduce standard `int`, `char`, `float` without checking if a project specific type (`ISN`, `CHR`, `FPC`) should be used instead.
- **Seam Tests:** When adding functionality, look for the corresponding "Seam" (test file) in `_Seams/` and add a test case there.

## Code Style — Chimera+ (AStarship Standard)

All **code and filenames** in the AStarship ecosystem follow the **Chimera+** style guide
(`~/AStarStarship/ASCIICrabs/__ChimeraPlus.md`). The naming philosophy applies to code
identifiers and filenames fleet-wide.

### Core Rule: immutable vs mutable
- **Immutable** (can't change after init) → **CamelCase**
- **Mutable** (changes at runtime) → **lower_snake_case**

### Naming Summary
| Element | Convention | Example |
|---|---|---|
| Type aliases (fixed-width) | 3-letter CAPS codes | `CHA` char8, `ISC` int32, `IUD` uint64, `BOL` bool, `FPC` float32, `FPD` double64 |
| Structs / classes | CamelCase + T(POD)/A(utoject) prefix | `TArray`, `AMap`, `Crabs` |
| Struct members (mutable) | lower_snake_case | `socket_bytes`, `header_bytes` |
| Struct members (immutable) | CamelCase | `StackTotalMin`, `ColumnWidth` |
| Free functions / methods | CamelCase, type-prefixed | `CrabsInit`, `TArrayBytes`, `TMapFind` |
| Function suffixes | `_NC` (no-check), `C` (const/count), `T` (POD), `A` (autoject) | `TArrayInsert_NC`, `CSizeMin` |
| Local variables | lower_snake_case (always) | `bytes_data`, `read_cursor` |
| Macros / #define | UPPER_SNAKE_CASE | `CRABS_RUN_TESTS`, `CPU_X64` |
| Private/protected members | trailing underscore | `aobj_`, `array_` |
| Namespaces | `namespace _ { ... }` | single shared underscore namespace |
| DB names / tables / columns | lower_snake_case (mutable) | `user_sessions`, `order_items` |
| Client-facing API / headers | CamelCase (immutable contract) | `OrderService`, `PaymentGateway` |
| Filenames (client contract) | CamelCase | `OrderService.h` |
| Filenames (host/storage) | lower_snake_case | `user_sessions.py` |

### Header Boilerplate (C/C++)
```
// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_<NAME>_<H|HPP>
#define CRABS_<NAME>_<H|HPP>
#include "..."
#if SEAM >= CRABS_<NAME>
... body ...
#endif
#endif
```

### Formatting
- 2-space indent
- Opening brace same line (functions/control), next line (structs/classes)
- `D_ASSERT()`, `D_COUT()`, `D_RETURNT(type, val)` for debug/assert/return
- `NILP` = nullptr
- `alignas(ACPUCacheLineSize)` for hot structs
- Doxygen `/** @param @return @pre @link @see @code */` comments

### The One-Sentence Version
**Immutable → CamelCase, mutable → lower_snake_case; macros UPPER_SNAKE; private/protected
get a trailing `_`; types are short CAPS width-codes (CHA/ISC/IUD/BOL); structs are
CamelCase with T(POD)/A(utoject) prefix; functions are CamelCase with T/A kind prefixes
and _NC/C markers; locals are snake_case; everything lives in `namespace _`; every file
starts with AStarship copyright, `#pragma once`, a `CRABS_*` guard, and `#if SEAM >= CRABS_*`
gating.** The name tells you the kind, width, access level, and mutability — before you
read the body.