# ASCIICrabs Refactor Plan

Refactor items to mull over. Consult master-agent before executing.
Status: DRAFT — nothing here has been approved for execution.

## 1. Consolidate ASCII Data Type POD Structs

### Problem

The core 128-bit and multi-word POD structs (IUE, ISE, FPD, FPE, FPC) are
defined in `AType.h`, but that file is 433 lines of mixed content: function
declarations, ACTXFrame, ATypeValue class, ATypePtr, Nil class, and 16
overloads each of AlignmentMask() and TypeOf(). Including `AType.h` to get
`struct IUE` pulls in the entire ASCII Type function API.

### Current struct locations

| Struct | Defined in | Also in |
|--------|-----------|---------|
| IUE (128-bit uint) | AType.h:25 (x64 MSVC), AType.h:96 (generic) | — |
| ISE (128-bit int) | AType.h:108 | — |
| FPC (32-bit float, SW) | AType.h:122, AType.h:130 (dup for 16-bit) | — |
| FPD (64-bit float, SW) | AType.h:140 | — |
| FPE (128-bit float) | AType.h:151 | — |
| IUE (via __int128) | _ConfigFooter.h:172 (GCC/Clang) | — |
| ISE (via __int128) | _ConfigFooter.h:173 (GCC/Clang) | — |
| TMS (timestamp) | _ConfigFooter.h:253 | — |
| ACTXFrame | AType.h:42 | — |
| ATypePtr | AType.h:251 | — |
| Nil | AType.h:259 | — |
| ATypeValue | AType.h:283 | — |
| TATypeValue, TATypeKV, etc. | AType.hpp | — |
| IUQ, ISQ (BigInt) | BigInt.h | — |

### Proposed solution

Create `ATypePOD.h` — a single file containing only the POD struct
definitions. No function declarations, no ACTXFrame, no ATypeValue, no
ATypePtr, no Nil. Just the data types:

```
ATypePOD.h
├── IUE (128-bit unsigned) — MSVC x64 struct + GCC __int128 fallback
├── ISE (128-bit signed)   — MSVC x64 struct + GCC __int128 fallback
├── FPC (32-bit float SW)  — 16-bit and 32-bit CPU variants
├── FPD (64-bit float SW)  — <64-bit CPU variant
├── FPE (128-bit float)
└── TMS (timestamp)
```

Include chain: `_Config.h` → `ATypePOD.h` → (nothing else).

This makes the POD types available to any file that includes `_Config.h`
without pulling in the full AType.h API. Files that currently include
`AType.h` just to get `IUE` or `FPE` can switch to `ATypePOD.h`.

### Files affected

- New: `ATypePOD.h`
- Modified: `AType.h` (remove struct defs, add `#include "ATypePOD.h"`)
- Modified: `_ConfigFooter.h` (remove IUE/ISE typedefs, add `#include "ATypePOD.h"`)
- Modified: Any file that includes `AType.h` only for POD structs

### Risk

LOW. The structs are pure data with no logic. Moving them to a new file
changes nothing about their layout or semantics. The only risk is forgetting
to update an include, which the compiler will catch immediately.

### Why not a .cpp?

The project is header-only with single-translation-unit builds. A .cpp file
would break the single-TU model and slow rapid compilation. A single header
with only struct definitions compiles in microseconds.

### Tree-shaker note

When the single-file header output with tree-shaker is implemented,
`ATypePOD.h` will be one of the first files to be inlined into the output.
The tree-shaker should recognize that POD structs are leaf nodes (no
dependencies beyond `_Config.h` typedefs) and can be emitted unconditionally.

---

## 2. Duplicate #ifndef Guard in _ConfigFooter.h

`CRABS_STACK_COUNT_MAX_DEFAULT` is defined twice (lines 74 and 80), with
different values (32 then 16). The second definition is silently ignored
because the `#ifndef` guard already saw the first. This means the "default"
stack count is 32, not 16, which may or may not be intended.

**Fix:** Remove one of the two definitions. Determine which value is
intended (32 or 16) and keep only that one.

**Risk:** LOW. One-line change. But verify the intended value before
removing.

---

## 3. #undefine Instead of #undef

`_ConfigFooter.h` uses `#undefine` (lines 337, 338, 354, 370, 390) instead
of the correct `#undef`. `#undefine` is not a valid preprocessor directive —
it will either cause a compiler error or be silently ignored (depending on
compiler). In GCC it produces a warning. In MSVC it may be silently ignored.

**Fix:** Replace all `#undefine` with `#undef`.

**Risk:** LOW. But this may be masking a bug: if the CT5_STOP/CT4_STOP/etc.
macros are not actually being undefined, subsequent `#ifndef` checks may
behave differently than intended.

---

## 4. BIn/BOut/Op/Slot/Promise Mutual Forward Declarations

Five files form a tight forward-declaration web:

```
BIn.h     → forward-declares BOut, Op
BOut.h    → forward-declares BIn, Op
Op.h      → forward-declares BOut
Slot.h    → forward-declares BIn, BOut, Op
Promise.h → forward-declares BIn, BOut, Op, Slot
```

This is not a circular include (no cycles detected), but it means that any
change to one of these five types requires checking all five files. The
forward declarations work because they only need the type name (pointer or
reference), not the full definition.

**Options:**
1. **Leave as-is.** The forward declarations work, no cycles, low risk.
2. **Create `CrabsCore.h`** with all five forward declarations in one
   place. Reduces the web to a single include.
3. **Consolidate into fewer files** (e.g., merge BIn.h + BOut.h + Op.h
   into a single `BStream.h`).

**Recommendation:** Option 1 for now. Revisit if the forward-declaration
web grows beyond these five files.

---

## 5. AType.h is a God File (433 lines)

`AType.h` contains:
- 16 function declarations (ATypeRemapEP, ATypeIsPOD, ACTX, ACTXHandle, etc.)
- ACTXFrame struct (context frame)
- IUE, ISE, FPC, FPD, FPE struct definitions (→ move to ATypePOD.h, item 1)
- 16 AlignmentMask() overloads
- 16 TypeOf() overloads
- ATypePtr struct
- Nil class
- ATypeValue class (148 lines of Set/Get methods)
- ATypeBytes, ATypeValueEnd, ATypeToEXT, ATypeIsCTX, ATypeToCTX_NC,
  ATypeRemapEP_NC

**Proposal:** After extracting ATypePOD.h (item 1), split AType.h into:
- `ATypePOD.h` — POD struct definitions only (new)
- `ATypeFunc.h` — function declarations (ATypeRemapEP, ATypeIsPOD, etc.)
- `ATypeValue.h` — ATypeValue class, ATypePtr, Nil
- `ATypeAlign.h` — AlignmentMask() overloads
- `ATypeTypeOf.h` — TypeOf() overloads

Or leave as one file if the tree-shaker handles it well. The tree-shaker
can trim unused overloads from a single header.

**Risk:** MEDIUM. Splitting AType.h touches every file that includes it.
The compiler will catch missing includes, but the diff is large.

---

## 6. BigInt.h / BigInt.hxx — ISQ and IUQ

**DONE (rewritten 2026-09-11).** The agent-written BigInt module was
completely broken. Full rewrite applied:

Original bugs:
- `SDL_malloc`/`SDL_free` called 48 times, defined **nowhere** in the codebase
- `#include <cstdlib>` — violated the no-stdlib rule
- String parsing used two sequential algorithms where one sufficed
- Bitwise operators (and/or/xor/not/shift) treated base-10^9 decimal digits
  as binary — semantically meaningless
- `operator<<` returned zero for shift==0 (should return *this)
- `ISQ::DivMod` used mathematical remainder convention instead of C++
- `TypeOf` returned wrong type codes
- `CRABS_BIGINT_STANDALONE` guard never used

Rewrite:
- Memory: `Autoject` + `RAMFactory` (`ObjectFactoryHeap`) — no naked new/delete
  for persistent storage. Scratch buffers in DivMod/ToStr use `new[]`/`delete[]`
  (freed before return).
- Error checking: `IsError()` method using `::_::IsError(const void*)` from
  Binary.hpp — no raw `ptr == nullptr` checks.
- Layout: `[ISW size_header | IUD digits[0..N-1]]` in the Autoject buffer.
  `length_` member tracks digit count. `DigitsRaw()` returns `origin + 1`.
  `DigitsCapacity()` computed from the Autoject size header.
- Removed all 8 bitwise operators.
- Added `operator/` and `operator%` convenience operators.
- Fixed ISQ remainder sign to C++ convention (sign of dividend).
- Fixed `TypeOf` to return 13/14 (placeholder pending AType table
  finalization with the 12 global POD types).

Verification (ad-hoc, not suite-green):
- ASan: no heap overflows after fix (two overflow bugs found and fixed:
  missing `+1` offset in DigitsRaw, overwriting Autoject size header).
- 25 ad-hoc checks passed: construction, addition, subtraction (with
  saturation), multiplication, division (10^21/3), string conversion
  round-trip, ISQ sign handling, C++ remainder convention, copy semantics.
- CMake build: `cmake --build build` → 100% Built target Crabs.
- Remaining: 4 pre-existing File test failures (missing Test.crabs,
  unrelated to BigInt). Memory leaks in test temporary objects (expected
  for a smoke test without RAII cleanup).

---

## 7. _Package.hxx — 22 Includes (God File)

`_Package.hxx` is the migration API entry point with 22 outgoing includes.
It's the "import everything" file. This is by design (it's the package
facade), but it means any change to any included file triggers a recompile
of everything that includes `_Package.hxx`.

**No action needed** unless compile times become a problem. The tree-shaker
will handle this by only emitting the symbols actually used.

---

## 8. _ConfigFooter.h — 485 Lines of Mixed Concerns

`_ConfigFooter.h` contains:
- Overridable configuration macros (lines 9-119)
- Platform-specific enums (lines 124-160)
- BYT typedef (lines 162-166)
- 128-bit int typedefs for GCC (lines 171-177) → move to ATypePOD.h
- SEAM-based feature gates (lines 179-191)
- FPW typedef (lines 193-197)
- CPU size macros (lines 199-214)
- CHR/CHD/STR typedefs (lines 216-232)
- ISR/IUR/FPR typedefs (lines 234-246)
- ISQ/IUQ forward declarations (line 251)
- TMS typedef (line 253)
- CHL typedef (lines 257-261)
- ISM/UIL/ISX/IUX/ISV/IUV typedefs (lines 263-294)
- TME typedef (lines 296-300)
- STR_ enum (lines 302-321)
- CT5/CT4/CT3/CT2 typedefs (lines 328-398)
- PRIME constants (lines 400-402)
- SIZEOF_ARRAY macro (line 404)
- AC* constant enums (lines 406-451)
- SZA-SZE size enums (lines 454-460)
- #undef cleanup (lines 463-483)

**Proposal:** After ATypePOD.h extraction, consider splitting into:
- `_ConfigFooter.h` — overridable macros + platform enums (keep)
- `_ConfigTypes.h` — all the derived typedefs (CHR, CHD, STR, ISR, IUR,
  FPR, CHL, ISM, UIL, ISX, IUX, ISV, IUV, TME, TMS, FPW, BYT)
- `_ConfigConstants.h` — AC* constant enums, PRIME constants, SZA-SZE

This reduces the recompile blast radius: changing a constant doesn't
trigger recompilation of files that only need the typedefs.

**Risk:** MEDIUM. The typedef chain is delicate — `_ConfigTypes.h` must
include `_Config.h` first, and the CT5/CT4/CT3/CT2 typedefs depend on the
E*STOP enums from `_Config.h`.

---

## 9. Single-File Header Output with Tree Shaker (Long-Term)

The end goal is a single-file header output with a tree shaker that:
1. Starts from the entry point (`_Package.hxx` or a user-specified file)
2. Follows includes to collect all reachable symbols
3. Emits only the symbols actually referenced (functions, classes, structs)
4. Inlines template code from `.hpp` files
5. Strips unused overloads, unused #if branches, and dead enums

This eliminates the circular include problem entirely: the output is a
single flat file with no `#include` directives. The tree-shaker resolves
all dependencies at build time.

**Prerequisites:**
- Item 1 (ATypePOD.h) must be done first — the tree-shaker needs clear
  leaf-node boundaries to work with.
- Item 5 (AType.h split) helps the tree-shaker by giving it smaller
  units to work with.
- The tree-shaker itself needs to be built (likely a Python or C++ tool
  that parses C++ headers and emits a filtered single file).

**Status:** FUTURE. Not in scope for the current overhaul. But the file
splitting in items 1, 5, and 8 are prerequisites that make the
tree-shaker tractable.

---

## Execution Order

If the Captain approves, execute in this order:

1. **Item 3** — `#undefine` → `#undef` (trivial, unblocks everything)
2. **Item 2** — Remove duplicate `CRABS_STACK_COUNT_MAX_DEFAULT` (trivial)
3. **Item 1** — Create `ATypePOD.h`, extract POD structs (medium)
4. **Item 6** — BigInt.h stdint resolution (DONE — 2026-09-11)
5. **Item 5** — Split AType.h (large, after ATypePOD.h is stable)
6. **Item 8** — Split _ConfigFooter.h (medium, after ATypePOD.h is stable)
7. **Item 4** — BIn/BOut/Op/Slot/Promise (defer, low priority)
8. **Item 7** — _Package.hxx (defer, tree-shaker will handle)
9. **Item 9** — Tree shaker (future)

Each step should be followed by a full build + test run to verify no
regressions.
