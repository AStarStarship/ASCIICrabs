# Chimera+ Style Guide

Derived from the ASCIICrabs codebase (`~/AStarStarship/ASCIICrabs/` — `*.h`, `*.hxx`, `*.hpp`).
This is the authoritative form for AStarship C/C++ (and the naming philosophy extends to the
whole fleet: filenames, DB objects, config, etc.). The one-sentence version is at the bottom.

## The Core Rule (immutable vs mutable)
- **Immutable members → CamelCase.**
- **Mutable members → lower_snake_case.**
- Why it's memorable: you only have to answer one question — *can this change after init?*
  Yes → snake_case. No → CamelCase.

### Filenames resolve via the functional paradigm
A filename is **immutable to the client** (the client binds to a fixed path/contract) but
**mutable to the host** (the host can rename/recreate it). Classify by the side whose
mutability the rule is protecting. For a *client-facing contract* (header a client imports,
a DB name a client connects to, an API the client is compiled against) → immutable →
**CamelCase**. For a *host/storage detail* (a table or column holding mutable rows, a temp
scratch file) → mutable → **lower_snake_case**.

> Ruling from the Captain (2026-09): **databases are mutable** → DB *names* and all
> tables/columns/rows are **lower_snake_case**. (This is also native Postgres idiom —
> unquoted identifiers fold to lowercase — so the rule and the platform agree.) The
> CamelCase discipline lives on the *client-facing API surface* the code binds to.

---

## The Full Naming Grammar (as actually used in ASCIICrabs)

### 1. Type aliases — the ASCII Data Type codes (UPPER_SNAKE / short caps)
Fixed-width primitive types are 3-letter caps codes. The letter encodes *kind*, the suffix
letter encodes *width* (A=8, B=16, C=32, D=64, E=128):
- `CHA/CHB/CHC/CHN` — char 8/16/32/wide
- `ISA/IUA` (8), `ISB/IUB` (16), `ISC/IUC` (32), `ISD/IUD` (64), `ISE/IUE` (128) — signed/unsigned int
- `ISN/IUN` — native int/uint; `ISW/IUW` — pointer-width int/uint (word)
- `ISM/IUM` — short/ushort
- `BOL` — bool (4-byte); `FPC` (float 32), `FPD` (double 64), `FPB` (16), `FPE` (128)
- `ERC` — error code (= ISW); `PTR`/`PTC` — void*/const void*
- `DTA/DTB/DTC/DTD/DTW` — ASCII Data Type 8/16/32/64/word
- `TMC`/`TMD` — 32/64-bit seconds-since-epoch timestamp
Rule: **these are immutable type identities → short CAPS codes**, not CamelCase. They're a
fixed vocabulary, referenced everywhere. Do not invent new width letters outside A–E.

### 2. Structs / classes — CamelCase, with a kind prefix letter
- `T`-prefix = the plain **T**emplate POD struct (the raw memory layout): `TArray`, `TMap`, `TMapBuf`.
- `A`-prefix = the **A**utoject (the owning class with a BOF/heap management layer): `AArray`, `AMap`.
- Domain structs: `Crabs`, `Slot`, `Boofer`, `Operand`, `Op`, `BIn`, `BOut`, `CIn`, `COut`.
- Rule: **type names are CamelCase** (immutable type identity). The `T`/`A` prefix is
  meaningful (POD layout vs owning autoject) — keep it.

### 3. Struct members — lower_snake_case (mutable) / CamelCase (immutable)
From `struct Crabs` and `struct TMap`:
- Mutable state → `socket_bytes`, `header_bytes`, `num_states`, `bytes_left`, `params_left`,
  `bout_state`, `bin_state`, `last_bin_state`, `last_byte`, `current_char`, `timeout_us`,
  `last_time`, `domain_value`, `codomain_value`, `total`, `count`.
- These are all **lower_snake_case** because they change at runtime.
- Anonymous `enum { ... }` constants inside a struct (immutable) → **CamelCase**:
  `StackTotalMin`, `BooferTotalMin`, `ColumnWidth`, `DomainColumns`, `BytesInit_`.

### 4. Free functions / methods — CamelCase, often with a type prefix
- Free functions are prefixed by the type they operate on: `CrabsInit`, `CrabsReset`,
  `CrabsStack`, `TArrayBytes`, `TArrayBegin`, `TMapDomain`, `TMapAdd`, `TMapFind`,
  `TMapSizeBytes`, `CMapOverheadPerIndex`.
- Method names on the `A`-autoject classes are short CamelCase verbs/nouns: `Domain()`,
  `Codomain()`, `Add()`, `Remove()`, `Find()`, `Clear()`, `Size()`, `Count()`, `PrintTo()`,
  `This()`, `AJT()`.
- Suffix conventions (all CamelCase, part of the name):
  - `_NC` = **N**o **C**heck (caller guarantees preconditions): `TArrayInsert_NC`, `TMapDomain_NC`, `TArrayShift_NC`.
  - `C`-prefixed or `C...` = **C**onstant/constexpr or "count/size" query: `CSizeMin`, `CMapOverheadPerIndex`, `CBytes()`, `CSizeCodef`.
  - `T`-prefix on the free function = operates on the `T` POD: `TArray*`, `TMap*`.
  - `A`-prefix on the free function = operates on the `A` autoject / ASCII object.
- Rule: **function names are CamelCase** (immutable — a function's identity doesn't change).
  The `_NC` / `C` / `T` / `A` markers are part of the name and carry meaning; keep them.

### 5. Local variables — lower_snake_case (always mutable)
`bytes_data`, `items_end`, `dest_start`, `dest_stop`, `src_count`, `read_cursor`,
`dez_nutz`, `element_count`, `domain_value`, `codomain_mapping`, `a_begin`, `end_a`.
Rule: **locals are lower_snake_case** (they're mutable by definition).

### 6. Macros / #define constants — UPPER_SNAKE_CASE
- Config: `CRABS_RUN_TESTS`, `CRABS_ROOM_BYTES`, `CPU_SIZE`, `USING_FPC`, `BOL_SIZE`,
  `PLATFORM_LINUX`, `CPU_X64`, `NILP`, `YES_0`, `NO_0`.
- Template-arg shorthand macros: `ARY_A` (the `typename T=CHA, typename ISZ=ISN` arg list),
  `ARY_P` (the `T, ISZ` pack), `ARY` (`TArray<T,ISZ>`), `MAP_A`, `MAP_P`, `MAP`.
  Pattern: `<TYPE>_A` = arg-list, `<TYPE>_P` = param-pack, `<TYPE>` = the instantiated type.
- Rule: **macros and macro-like constants are UPPER_SNAKE_CASE.** The `_A`/`_P`/bare
  triad for template args is a load-bearing convention — reuse it for new templates.

### 7. Private / protected members — trailing underscore
From `class AArray` / `class AMap`:
- `Autoboofer<...> aobj_;` and `AArray<...> array_;` — private members get a **trailing `_`**.
- `enum { BytesInit_ = Total_ * 32 };` — private enum constant also trailing-underscored.
- Rule: **private and protected members use a trailing underscore** (`aobj_`, `array_`).
  Public members (struct fields) do not. This is how you tell access level at a glance.

### 8. Namespaces
- Everything lives in `namespace _ { ... }` — a single shared underscore namespace,
  closed with `}  //< namespace _`.
- Rule: keep the `_` namespace; it's the project's collision-avoidance wrapper.

### 9. Header structure (the boilerplate, in order)
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
- `// Copyright AStarship <https://astarship.net>.` is line 1 of every file.
- `#pragma once` + a `#ifndef/#define` guard named `CRABS_<UPPERNAME>_<EXT>`.
- **SEAM gating**: every real header wraps its body in `#if SEAM >= CRABS_<NAME>` (and
  `.hpp` impls often add `#if SEAM == CRABS_<NAME>` to include `_Debug.h` vs `_Release.h`).
  The `_Seams/*.hxx` files are numbered (`01.COut.hxx` … `23.Release.hxx`) and define the
  `CRABS_<NAME>` seam ordering. New modules get a seam number.
- Include the `.h` from the matching `.hpp` (`Array.hpp` includes `Array.h`).

### 10. Doc comments
- Doxygen-style `/* ... */` block above each public function/struct, with `@param`,
  `@return`, `@pre`, `@link`, `@see`, `@code ... @endcode` for ASCII memory-layout diagrams.
- Trailing member comments use `//< description` (Doxygen member comment):
  `ISC total;  //< Number of elements in the array.`
- ASCII art memory layouts in `@code` blocks are a first-class doc convention — keep them.

### 11. Formatting
- 2-space indent. Opening brace on the same line for functions/control, next line for
  structs/classes (as in `struct Crabs {` / `class AMap {`).
- Multiple declarators share a type, comma-aligned:
  `const DTB* header, * header_start;`
- `inline` on small header-defined functions; `constexpr` for compile-time constants
  (`constexpr ISZ CSizeMin()`, `constexpr ISZ CMapOverheadPerIndex()`).
- `D_ASSERT(...)`, `D_COUT(...)`, `D_RETURNT(type, val)` are the debug/assert/return macros
  (from `_Debug.h`/`_Release.h`); `NILP` = nullptr.
- Alignment: `struct alignas(ACPUCacheLineSize) Crabs { ... }` — cache-line-align hot structs.

---

## The One-Sentence Version
**Immutable → CamelCase, mutable → lower_snake_case; macros UPPER_SNAKE; private/protected
get a trailing `_`; types are short CAPS width-codes (CHA/ISC/IUD/BOL); structs are
CamelCase with a T(POD)/A(utoject) prefix; functions are CamelCase with T/A kind prefixes and
_NC (no-check) / C (const-count) markers; locals are snake_case; everything lives in
`namespace _`; every file starts with the AStarship copyright, `#pragma once`, a
`CRABS_*` guard, and `#if SEAM >= CRABS_*` gating.**

The whole system reduces to: *the name tells you the kind, the width, the access level, and
whether it can change — before you read the body.*
