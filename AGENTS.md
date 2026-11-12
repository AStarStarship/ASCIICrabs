# Agent Rules for Crabs



## 1. Build, Lint, and Test Commands

### Build
- **Compile:** `g++ -g -o Crabs _Seams/_Seams.cpp` (single translation unit)
- **Output Binary:** `Crabs` (Windows: `Crabs.exe`)
- The single-TU entry is **`_Seams/_Seams.cpp`** (it defines the seam table,
  then includes `_Config.h` + the package + all seam test files). There is no
  `_Main.hxx` — an older name that no longer exists.

### Test (canonical: `_Seams/CMakeLists.txt`)
The project uses a custom seam tree testing framework located in `_Seams/`.
The canonical build entry point is **`_Seams/CMakeLists.txt`** — configure a
separate build tree from it and build the `Crabs` target; do not hand-roll the
`g++` line for the test binary, the CMake target wires up the single-TU rule
correctly (one generated `.cpp` wrapper = one `.exe`).

**Selecting the seam:** the active seam is set in **`_Seams/_Config.h`** via
`#define _Seam _Crabs<Name>` (e.g. `_CrabsRelease`, `_CrabsCore`). Do **NOT**
edit `_Seams/_Seams.cpp` to change the seam or the `TTestTree` — change the
`_Seam` macro in `_Config.h` instead. The full release tree (seam 24,
`_CrabsRelease`) is where every seam test function exists, so the canonical
`TTestTree<...>` in `_Seams.cpp` compiles cleanly there. Lower seams are
debug-only slices; if a lower seam can't link a `TTestTree` entry, that's
expected — build at `_CrabsRelease` for the real run.

Example (out-of-source build tree, reusable across runs):
```
cmake -S _Seams -B build/crabs
cmake --build build/crabs
./build/crabs/Crabs            # run the full seam tree (Release)
```

- **Run All Tests:** `./Crabs` (Runs the compiled executable)
- **Verify clean at runtime (catches infinite recursion / overflow the plain
  build hides):** build with ASan and run:
  ```
  g++ -std=gnu++23 -x c++ -g -fsanitize=address -fno-omit-frame-pointer \
    -I _Seams -I . -o /tmp/crabs-asan _Seams/_Seams.cpp
  ASAN_OPTIONS=detect_leaks=0 /tmp/crabs-asan
  ```
  A `stack-overflow` backtrace = a function calling itself (directly or via
  `operator<<`/`Print` cycles). This is how ROut's `Print`/`operator<<` mutual
  recursion was caught.
- **Debugging:**
  - Breakpoints: `Test.hpp` contains a `TBRK` function intended for one breakpoint.
  - No gdb on the box by default; ASan + the compiler's `-E` preprocessed dump
    are the primary diagnostics.

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
- **Minimal Standard Library:** The project aims to replace the C++ std library. **Do not add `#include <string>`, `<vector>`, `<iostream>`, `<errno.h>`, `<unistd.h>`, or any other C++/C stdlib header** unless a file already depends on it and the task is to fix that file in place. Use project-specific containers (`Array`, `List`, `Stack`, `String`) instead.
  - **Sanctioned exception:** `Random.hxx` includes `<random>`, `<cstdlib>`, `<cmath>` — these MUST stay before the `namespace _ {` line (stdlib headers inside `namespace _` poison `_::std` / `_::__gnu_cxx`). Do not move them inside the namespace.
  - `fputc`/`stdout` (and `<cstdio>` transitively) are available in the TU and are the sanctioned leaf for `ROut` console output (see ROut pitfalls below).
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
- **Macros:** _UpperCamelCase, i.e. leading underscore + CamelCase (Macros are mostly used for changing seams/unit tests). LLM tokenizers have lowercase and capitalized tokens but no UPPERCASE words, so all-caps names are avoided.

### Formatting
- **Indentation:** 2 spaces (inferred from `AType.h`).
- **Braces:** K&R style / Google style (attached to the line definition).
- **Line Length:** generally 80 chars, but be reasonable.
- **Pointer/Ref:** Attached to type or variable? `AType.h` shows `void* begin` (attached to type).
- **Control Flow:** `if`, `for`, `while`, `switch` have a space between keyword and parenthesis. `else` goes on same line as closing brace.

### File Structure
- Do not create files unless you have approval. This is a microframework, it is designed to be token efficient and each file has very specific purposes.
- **Headers:** `.h` for declarations.
- **Implementation:** `.hpp` or `.hxx` (often included at bottom of `.h` or compiled separately).
- **Guards:** `#ifndef Crabs_SEAM_H` style.
- **Copyright:** Include `// Copyright AStarship <https://astarship.net>.` at the top followed by one blank line.
- **Includes:** Use `""` for local project files, `<>` for system/config files (e.g., `#include <_Config.h>`).

### Error Handling
- **No Exceptions:** The project likely relies on error codes or assertions (`IsError`).
- **Debugging:** Use `TBRK()` for breakpoints. Use `RefOut()` (wrapped) for logging if strictly necessary, but prefer the "Seam" test structure.

### Specific Types (from `AType.h` and `_Spec/Data`)
- 'CHA', 'CHB', 'CHC', 'CHR' are char, char16_t, char32_t, and the default/OS character type. Prefer CHR unless otherwise specified. `CHN` = `wchar_t` (which is `char32_t`/CHC on a 4-byte-wchar platform) — do NOT assume `CHN` is 8-bit.
- `DTW`, `DTB`: Data Type Word/Byte.
- `IUW`, `IUE`, `IUD`: Integer Unsigned Word/128-bit/64-bit.
- `ISW`, `ISE`: Integer Signed Word/128-bit.
- `FPC`, `FPD`, `FPE`: Floating Point types.
- `PTR`: Pointer macros or wrappers may exist.
- **`IUE`/`ISE` are the native 128-bit types on GCC/Clang** (`unsigned __int128` / `__int128`, typedef'd in `_ConfigFooter.h` when `HAS_128_BIT_INT` is set). On MSVC they are the `{lsb, msb}` structs in `AType.h`. The struct definitions are gated `#if !defined(HAS_128_BIT_INT)` — do not re-add them unconditionally, or they shadow the native typedef inside `namespace _` and break 128-bit arithmetic (e.g. Ryu's `IUE * IUD`).

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
- **Minimal changes:** If you don't have to do it, don't. Remove before you add. No drive-by refactors or renames — the user does bulk renames himself (e.g. the `boofer`→`buffer` rename) and will YOLO find-replace them.
- **Don't touch git.** Agents only edit files. The user commits via `git add --all` + YOLO merge himself. Read-only git (`status`/`log`/`diff`/`show`) is fine; no writes.
- **Don't edit `_Seams/_Seams.cpp`** to change the seam or the `TTestTree`. Change the `_Seam` macro in `_Seams/_Config.h` instead.

## 5. Hard-Won Pitfalls (autocomplete-refactor damage, 2026-11)
These classes of bug were introduced by AI autocomplete tab-insertion during a
large refactor and cost many hours. Check for them when editing:
- **Include-guard collisions:** A `.hpp` must use its OWN guard (`_Crabs<Name>HPP`),
  never its sibling `.hxx`'s guard (`_Crabs<Name>HXX`). `Puff.hpp` used
  `_CrabsPuffHXX` (Puff.hxx's guard), so when Puff.hpp was included before
  Puff.hxx the impl file's body was silently skipped → link errors on
  `BinaryLUTDecimals` etc.
- **Stray `#else` / dropped `}` / mis-nested `#if`:** autocomplete inserts stray
  `#else` (pairs with the include guard, not a real `#if` → drops a whole code
  branch, e.g. Random.hxx's public `Random()` API), drops a function-body `}`
  (makes the enclosing `namespace _` look unmatched → hundreds of "definition
  not in namespace enclosing" errors), and corrupts constructor init lists
  (e.g. `element(format, count)` where it should be `element(value, count)`).
  **Diagnose with a real brace tokenizer on `g++ -E` output, not a regex** —
  regex checkers lie on string/char literals. The compiler's "expected '}' at
  end of input ... to match this '{'" note points at the real culprit.
- **`namespace _` opened inside a seam gate must close in the same gate.**
  Puff/Slot/BOut/Random all had `namespace _ {` outside `#if _Seam >= X` but
  the close inside it → unclosed at lower seams.
- **ROut print recursion:** `ROut::Print(CHA/CHB/CHC)` must be the LEAF
  (`fputc` to stdout) — do NOT write `_ROut << item` inside a `Print` body, or
  it cycles `Print` → `operator<<` → `Print` → ∞ (stack overflow at runtime,
  invisible to the plain build, caught by ASan). Integer/float `Print`
  overloads convert to a `CHA buffer[]` and call `Print(const CHA*)`, which is
  the acyclic path.
- **Cross-seam type dependency:** if a lower-seam file needs a type from a
  higher-seam-gated header (e.g. BigInt needs Autoject, which is
  `_CrabsUniprinter`-gated), gate the consumer on that seam — don't pull the
  type down.
- **Float print is `_CrabsFToS` (seam 3); float stop-pointer is
  `_CrabsUniprinter` (seam 4).** Core-seam code touching either must be
  seam-gated.
- **`#endif` comments are LIES** — trace real `#if` nesting with a stack, don't
  trust the label.

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
| Macros / #define | _UpperCamelCase (leading `_` + CamelCase) | `_CrabsRunTests`, `_NilP` |
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
#include "_Debug.h"
#else
#include "_Release.h"
#endif
//... body ...
#endif
#endif
```

### Formatting
- 2-space indent
- Opening brace same line (functions/control), next line (structs/classes)
- `_DAssert()`, `_DROut()`, `_DReturnT(type, val)` for debug/assert/return
- `_NilP` = nullptr
- `alignas(ACPUCacheLineSize)` for hot structs
- Doxygen `/** @param @return @pre @link @see @code */` comments

### The One-Sentence Version
**Immutable → CamelCase, mutable → lower_snake_case; macros _UpperCamelCase (LLM
tokenizers lack UPPERCASE tokens); private/protected
get a trailing `_`; types are short CAPS width-codes (CHA/ISC/IUD/BOL); structs are
CamelCase with T(POD)/A(utoject) prefix; functions are CamelCase with T/A kind prefixes
and _NC/C markers; locals are snake_case; everything lives in `namespace _`; every file
starts with AStarship copyright, `#pragma once`, a `CRABS_*` guard, and `#if SEAM >= CRABS_*`
gating.** The name tells you the kind, width, access level, and mutability — before you
read the body.

## Git Protocol

- **Do NOT undelete files from git.** A deleted or renamed file was done so on
  purpose — it wasn't supposed to be there, it got merged into another file, or
  it wasn't named correctly. Fix the current files in place; do not try to
  rescue or restore stale files from git history (no `git checkout -- <path>`,
  `git restore <path>`, `git show <ref>:<path>` written back to disk, or
  `git revert` to bring a deleted path back).
- **Only `master-agent` and the Captain may modify git.** No other agent or
  profile may run git write operations on a tracked repo — no `git commit`,
  `git add`, `git push`, `git reset`, `git rebase`, `git merge`, `git rm`,
  branch creation, force-push, or PR changes. Read-only inspection
  (`git status`, `git log`, `git diff`, `git show`, `git ls-files`) is allowed
  for anyone.
