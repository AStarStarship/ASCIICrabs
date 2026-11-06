# Low-Level Agent Context
Dated 2026-11-05 (UTC, round 6 — FULL RELEASE BUILD GREEN + RUNTIME CLEAN).
Workspace: /home/astar/AStarStarship/ASCIICrabs, branch Issue376.
_Seam: _CrabsRelease (seam 24) — flipped from _CrabsCore this session.
The user said: don't touch _Seams.cpp's TTestTree; the fix is to change the
_Seam macro so the full tree (where every test fn exists) compiles.

## STATUS (round 6 — RELEASE BUILDS 0 ERRORS AND RUNS CLEAN, exit 0)
Trajectory this whole session: 700 → 426 → 253 → 80 → 10 → 9 (Core) →
flipped _Seam to _CrabsRelease → fixed Release-only bugs → **0 errors, binary
runs, "Unit tests completed successfully!", exit 0** (verified under ASan AND
plain release build).

### Round-6 fixes (after flipping _Seam to _CrabsRelease in _Seams/_Config.h)
1. **AType.h + AType.hxx — IUE/ISE struct shadowed the native 128-bit typedef.**
   `_ConfigFooter.h:163` typedefs global `IUE = unsigned __int128` + defines
   `HAS_128_BIT_INT` on GCC. But `AType.h:108` defined `struct IUE {lsb,msb}`
   (and `struct ISE`) UNCONDITIONALLY inside `namespace _`, shadowing the
   typedef. Ryu.hxx's `(IUE)m * mul[0]` then resolved to the struct (no
   operator*) → 3 "no match for operator" errors. FIX: wrapped the
   `struct IUE`/`struct ISE` (AType.h:108-129) AND their constructor defs
   (AType.hxx:80-98) in `#if !defined(HAS_128_BIT_INT)`. On GCC they're the
   native 128-bit types; the structs are only for MSVC (mirrors the
   `#if USING_VISUAL_CPP_X64` guard already at AType.h:25).
2. **24.Release.hxx + 22.Script.hxx — hallucinated `ADoor`.** Both had
   `ADoor door; Slot slot;` but `ADoor` is defined NOWHERE (real type is
   `TDoor<DOR_A>` in AType.hpp:298). Also `TDoor<DOR_A>` is wrong: `DOR_A=ISZ`
   and `ISZ` is only a *template param name* (DOR_P=`typename ISZ=ISN`), not a
   global type → "ISZ was not declared". FIX: `TDoor<> door;` (default template
   arg) + added `#include "../Door.hpp"` to 24.Release.hxx (22.Script.hxx had it).
3. **Random.hxx — stray `#else` (autocomplete) made the public Random API vanish.**
   The file had `#ifndef CRABS_RANDOM_HXX` (3) ... `#else` (64) ... `#endif`
   (155). The `#else` paired with the INCLUDE GUARD, not a real `#if`, so at
   Release the top branch (simple std-random `TRandom`/`TRandomBits`/
   `TRandomSignedProbility`) won and the bottom branch — which has the REAL
   overflow-safe versions PLUS the only defs of `IUCRandom`/`IUDRandom`/
   `Random(...)`/`RandomProbability*` — was DROPPED → 20+ link errors. FIX:
   deleted the 3 obsolete top-branch duplicates (lines 26-63) and the stray
   `#else` (line 64); the bottom branch's complete impls are now the sole defs.
4. **Puff.hpp — used Puff.hxx's include guard.** `Puff.hpp:3-4,1152` used
   `#ifndef/#define _CrabsPuffHXX` — the SAME guard as `Puff.hxx`. Since
   Puff.hpp is included before Puff.hxx (via ROut.hxx:8, 02.ItoS.hxx:4), the
   guard was pre-set and Puff.hxx's body (incl. `BinaryLUTDecimals()` at :100)
   was skipped → link error. FIX: Puff.hpp now uses its own guard
   `_CrabsPuffHPP` (lines 3,4,1152).
5. **ROut.hxx — infinite recursion (runtime SIGSEGV/stack-overflow, found via ASan).**
   `Print(CHA)` did `_ROut << item` → `operator<<(ROut,CHA)` (line 371) →
   `o.Print(item)` → `Print(CHA)` → ∞. The whole ROut char path cycled:
   TPrint (Uniprinter.hpp:20 `p << c`) → operator<< → Print → operator<< → ∞.
   ASan backtrace confirmed `ROut::Print(char)` ⇄ `operator<<(ROut&,char)`.
   FIX: made `Print(CHA)` the LEAF via `fputc(item, stdout)` (fputc/stdout are
   already in the TU). Also fixed the CHB + CHC paths that recursed the same
   way: `Print(CHB)` now `fputc(CHA(item & 0x7f), stdout)`; `Print(CHC)` now
   funnels to `Print(CHB)`/`Print(CHA)` (was `_ROut << CHN(...)` where CHN=
   wchar_t=CHC on this box → operator<<(ROut,CHC) → Print(CHC) → ∞).
   The integer/float Print overloads (high-seam #else branches) already
   converted to `CHA buffer[]` → `Print(const CHA*)` → TPrint → operator<< →
   Print(CHA) → fputc (acyclic). NOTE: house is "ASCII Data Types", console is
   CHA/8-bit, so CHB/CHC are emitted as their low CHA byte (lossy for non-ASCII
   — matches the ASCII-only design; revisit if real Unicode console is needed).

### BUILD + RUN (verified working, round 6)
```
cd /tmp/crabs-build && rm -f CMakeCache.txt
cmake /home/astar/AStarStarship/ASCIICrabs/_Seams
cmake --build .        # 0 errors at _CrabsRelease
./Crabs                # exit 0, "Unit tests completed successfully!"
# ASan check (catches the recursion/overflow the plain build hides):
g++ -std=gnu++23 -x c++ -g -fsanitize=address -fno-omit-frame-pointer \
  -I _Seams -I . -o /tmp/crabs-asan _Seams/_Seams.cpp
ASAN_OPTIONS=detect_leaks=0 /tmp/crabs-asan   # exit 0, clean
```

### The earlier round-5 fixes (still in place)
Puff.hxx namespace-gate move; Stringf.hxx (Linef ctor init `element(value,…)`,
missing `}` in Rightf::Hex(CHB), FToS gating on float SPrint + Print(FPC/FPD));
BigInt.h/.hxx gated on _CrabsUniprinter; Stringf::Pointer()→Ref() rename (the
YOLO `boofer`→`buffer` rename by the user subsumed several of these). See git.

### REMAINING / TO VERIFY
- The `boofer`→`buffer` YOLO rename (user) was verified clean at Core (0
  leftover, BOut.hxx's pre-existing `buffer`/`buffer_bytes` params intact) but
  has NOT been re-verified at every seam level — spot-check Uniprinter/Release
  if new errors appear.
- CHB/CHC console output is lossy (low CHA byte). Fine for ASCII tests; if a
  test expects real UTF-8/UTF-16 console output, Print(CHB)/Print(CHC) need a
  proper encoder, not `& 0x7f`.
- Run each seam individually (01..24) to confirm the progressive TTestTree in
  the CANONICAL _Seams.cpp (full 22-name list) is correct at Release — it is,
  since all 22 fns exist at Release. Lower seams are debug-only (the user
  confirmed: don't gate the TTestTree, just use the right _Seam).
### Round-5 detail (historical, superseded by round 6 above)
The single biggest round-5 win: a missing `}` in `Rightf::Hex(CHB,...)`
(Stringf.hxx:~1478) made the enclosing `namespace _` look unmatched, cascading
into the "definition not in namespace enclosing _::X" + "expected '}' at end of
input" family (hundreds of errors). One brace fixed it.

### Files edited in round 5 (NO commits — user commits himself)
1. **Puff.hxx** — moved `#if _Seam >= _CrabsIToS` to wrap the `namespace _ {`.
   Before: `namespace _ {` opened OUTSIDE the gate, but `}  //< namespace _`
   was INSIDE the gate → at Core (seam 1 < 2) the namespace never closed.
   Now open+close are both inside the IToS gate. Balanced.
2. **Stringf.hxx** — THREE fixes:
   a. Line 1514: `Linef::Linef(CHA value, ISW count) : element(format, count)`
      → `element(value, count)`. Autocomplete used the member `format` (not
      declared yet in the init list) as the arg to init `element`. Sibling
      constructors all use `element(value, count)`.
   b. **THE BIG ONE** — `Rightf& Rightf::Hex(CHB value, ISW count) {` was
      MISSING its closing `}` (autocomplete dropped it); the next function
      `Rightf::Hex(CHC...)` started right after `return *this;`. Added the `}`.
      This was the root cause of the namespace-mismatch cascade.
   c. Gated the CHB + CHC float `SPrint` overloads (FPC/FPD, ~lines 617-637 and
      742-762) on `#if _Seam < _CrabsFToS → return string; #else →
      TSPrint<CHS>(...)`. They called `TSPrint<CHB/CHC>` float overloads that
      only exist at _CrabsFToS (Puff.hpp:1132-1148, inside its FToS block).
3. **BigInt.h + BigInt.hxx** — gated the whole body on
   `#if _Seam >= _CrabsUniprinter`. ROOT CAUSE: Autoject.h gates EVERYTHING
   (`RAMFactory` typedef, `namespace _`, `ObjectFactoryHeap`, the `Autoject`
   class) behind `#if _Seam >= _CrabsUniprinter` (line 6 → 84). At Core those
   are undefined, but BigInt used them unconditionally → ~50 errors. BigInt is
   only included by _Package.hxx and no Core/IToS seam test references ISQ/IUQ,
   so gating on Uniprinter (where Autoject exists) is safe. Added `#if` after
   the includes and the matching `#endif` before each file's guard `#endif`.
   (User also fixed Random.hxx + BOut.hxx namespace gates themselves mid-session.)

### The 9 REMAINING errors (all at _CrabsCore, pre-existing/known)
1. **Stringf.hxx:172/177** — `TStringFloatStop` not declared. The float
   `SScan(const CHA*, FPC&/FPD&)` overloads (lines 170-178) call
   `sscanf(start,"%f"/"%lf",&value)` then `TStringFloatStop<CHA>(start)`.
   `TStringFloatStop` is at Stringf.hpp:1031 INSIDE the
   `#if _Seam >= _CrabsUniprinter` block → invisible at Core. USER'S PREFERENCE
   (round 4): replace sscanf with the in-house string-to-float algorithm, NOT
   sscanf, NOT TStringFloatStop. That algorithm is NOT in the tree yet
   (searched: no TSScanFloat/ParseFloat/StrToFloat; TDecimalFloat + Ryu are
   float→STRING only). NEEDS USER to name it, or we write one. Interim unblock
   (if user OKs): gate the two SScan float overloads on
   `#if _Seam >= _CrabsUniprinter`.
2. **Stringf.hxx:1202/1209** — `TSPrint<CHA>(buffer, buffer+LengthMax, FPC/FPD)`
   in `Stringf::Print(FPC)`/`Print(FPD)` (lines 1200-1211). Float TSPrint only
   exists at FToS. FIX: gate the TSPrint call on `#if _Seam >= _CrabsFToS`
   (or `#if _Seam < _CrabsFToS return;`), mirroring the SPrint fix.
3. **Stringf.hxx:1126** — `void* Stringf::Pointer()` "no declaration matches".
   The `Stringf` class (Stringf.h) has NO `Pointer()` member declared, but
   Stringf.hxx:1126 defines it out-of-line. Either add `void* Pointer();` to
   the Stringf class decl in Stringf.h, or delete the def if dead. CHECK the
   Stringf class member list in Stringf.h.
4. **_Seams/01.Core.hxx:17** — `no match for operator<< (_::ROut, _::ROut)`.
   Line 17: `_DROut("FAIL: " << name << _DROutNL)`; `_DROutNL` is
   `::_::RefOut().NL()` (an ROut) → `ROut << ROut`. The
   `operator<<(ROut&, ROut&)` overload is at ROut.hxx:437, gated
   `#if _Seam >= _CrabsUniprinter` → absent at Core. Fix: add that overload
   outside the Uniprinter gate, or restructure line 17 to not append an ROut
   (e.g. `... << name; _DROutNL;`).
5. **_Seams/_Seams.cpp:64/66** — `Regex was not declared` + `template argument
   9 is invalid`. `TTestTree<..., Regex, ...>` (line 64) references
   `CRTest::Regex`, defined at `_Seams/09.Regex.hxx:15`. Check whether
   09.Regex.hxx's `Regex` fn is gated on a seam > Core (e.g. `#if _Seam >=
   _CrabsRegEx` = 9) so it's invisible at Core while still listed in
   TTestTree. Fix: gate the `Regex` entry in the TTestTree list, or make the
   fn unconditional.

### Build commands (VERIFIED working this session)
```
# CMake (canonical, _Seams/CMakeLists.txt wraps _Seams.cpp into a single TU):
cd /tmp/crabs-build && rm -f CMakeCache.txt
cmake /home/astar/AStarStarship/ASCIICrabs/_Seams
cmake --build . 2>&1 | grep -cE " error:"
# clang 19 installed mid-session — sharper diagnostics:
clang++ -std=gnu++23 -x c++ -I _Seams -I . -fsyntax-only -fpermissive \
  -ferror-limit=30 _Seams/_Seams.cpp
# direct g++ also works:
g++ -std=gnu++23 -x c++ -I _Seams -I . -o /tmp/crabs _Seams/_Seams.cpp
```
NOTE: `_Seams/_Main.hxx` (referenced in round-4 notes) NO LONGER EXISTS. The
single-TU entry is `_Seams/_Seams.cpp` (CMake wraps it into
build/generated/_Seams.cpp). Use /tmp/crabs-build as the reusable build dir.

### DIAGNOSTIC TOOLING (in scratch, reuse)
- **cb2.py** (brace matcher, handles comments+strings properly) — THE tool that
  found every missing brace this session. Usage:
  `python3 .../cache/scratch/cb2.py File1 File2...` → lists unclosed `{` + line.
- **checkif.py** (#if/#ifdef/#ifndef vs #endif balance per file). All 134 files
  report 0 unbalanced as of this dump.
- Reliable stray-brace hunt: `g++ -E` the file ALONE at the seam, run cb2.py's
  stack matcher on the preprocessed output, map the unmatched `{` to its source
  line. The compiler's "expected '}' at end of input ... to match this '{'"
  note is the fastest pointer. A "col-0 `}` that leaves depth at 2 instead of 1"
  = the line AFTER the function whose `}` is missing.

### PITFALLS (round 5, hard-won)
- **A missing single `}` in a function body makes the ENCLOSING `namespace _`
  look unmatched.** Compiler then reports "definition of X::Y not in namespace
  enclosing _::Y" for every out-of-line def after the stray + "expected '}' at
  end of input ... to match this '{' (namespace _ line 68)". Do NOT chase the
  namespace — find the missing `}` (cb2.py).
- **Regex-based brace checkers LIE** (string/char-literal stripping eats braces,
  e.g. a `'~'` char). Always use a real tokenizer (cb2.py) or the compiler.
- **A `namespace _` opened inside a seam gate MUST have its CLOSE in the same
  gate.** Puff/Slot/BOut/Random all had `namespace _ {` outside `#if _Seam >= X`
  but `}  //< namespace _` inside it → unclosed at lower seams.
- **`#endif` comments are LIES.** Don't trust `#endif  //< _Seam >= _CrabsFToS`
  — trace real `#if` nesting with a stack (checkif.py).
- **Cross-seam type dependency:** BigInt needs Autoject; Autoject.h is
  Uniprinter-gated. If a Core file needs a type from a higher-seam-gated header,
  gate the consumer OR move the type decl down. We gated BigInt (safe).
- **Float print (TRyuF2S/TRyuD2S, TSPrint float, Stringf::Print float) =
  _CrabsFToS (seam 3). Float STOP-pointer (TStringFloatStop) = _CrabsUniprinter
  (seam 4).** Both invisible at Core. Core code touching them must be gated.
- **User's rules:** NO git (agent does not stage/commit/push; user does
  `git add --all` + YOLO merge). NO <cstring>/<cstdlib> (banned; use house
  TSScan/SScan). NO TStringFloatStop for float PARSING (wrong tool +
  Uniprinter-gated). The in-house string-to-float parser the user references
  does NOT exist in the tree yet — ASK before writing one.

### What's NOT done (carry to next session)
1. Fix the 9 errors above. Suggested order: #2 (Stringf::Print float gate),
   #3 (Stringf::Pointer decl), #4 (ROut<<ROut at Core), #5 (Regex in TTestTree),
   #1 (TStringFloatStop / in-house float parser — needs user input).
2. Rebuild at _CrabsUniprinter and _CrabsRelease and repair the same bug classes
   there (float overloads + BigInt/Autoject become live at those seams).
3. Run the actual seam test suite (`./Crabs` after a clean build) — first real
   functional verification this session.
4. Verify the _Yes0/_No0 strip (user, round 4) didn't break other headers at the
   Release seam (no "unterminated #ifndef" seen at Core this session).

### Git / session state
- Branch Issue376; ~149 modified / 16 untracked at session start (more now).
- NO commits by agent. User commits.
- clang 19.1.7 installed mid-session (user: "Clang is installed now").

### Type / seam facts (this box)
- CHA=char, CHR=char, CHD=char, ISC=int32_t, IUC=uint32_t, ISD=int64_t,
  IUD=uint64_t, ISW=intptr_t, FPC=float, FPD=double, IUE=128-bit unsigned.
- Seam numbers (_Seams/_Seams.cpp): Core=1, IToS=2, FToS=3, Uniprinter=4,
  Clock=5, Array=6, Stack=7, String=8, RegEx=9, Loom=10, List=11, Book=12,
  Map=13, Table=14, Dictionary=15, Op=16, Room=17, MathCore=18, Random=19,
  File=20, Promise=21, Script=22, Password=23, Release=24.
- Stringf.hxx (post round-5, ~1629 lines): `namespace _` blocks at ~9, ~68,
  ~1592; `#if _Seam >= _CrabsCore` wraps the main body (~61 → ~1582);
  `#if _Seam >= _CrabsFToS` is a SIBLING section (~1584 → end) with its own
  `namespace _` for the CHA float SPrint (TRyuF2S/TRyuD2S).
- Stringf.hpp: TWO seam blocks — `#if _Seam >= _CrabsCore` (63-565) and
  `#if _Seam >= _CrabsUniprinter` (566-1545). TStringFloatStop + TSDecimalEnd
  live in the Uniprinter block (TStringFloatStop at 1031).
- Autoject.h: everything `#if _Seam >= _CrabsUniprinter` (6-84). Autoject.hpp
  same (10-460). Autoject.hxx: `#if _Seam >= _CrabsStack` (5+).
- Puff.hpp: `#if _Seam >= _CrabsIToS` (7-507) + `#if _Seam >= _CrabsFToS`
  (509-end). Float TSPrint overloads at 1132-1148 are in the FToS block.
- ROut.hxx: `RefOut()` at 346 (Core-visible); `operator<<(ROut&,ROut&)` at 437
  is Uniprinter-gated.
- Key API: `TSScanSigned<IS,IU,CHS,CHT>(start,item)`,
  `TSScanUnsigned<IU,CHS,CHT>(start,item)`, public `TSScan(start, ISC&/ISD&/
  IUD&)`. `TSPrint<CHS>(start,stop,item)` generic; float specializations only
  at FToS (Puff.hpp). `TStringFloatStop<CHS>(start)` stop-pointer finder,
  Uniprinter-gated.

### Historical (rounds 1-4, condensed)
Round 1: size_t→ISW, TMC→TME, malloc/free→RAMFactory, memset/memcpy→
ArrayFill/ArrayCopy, OS includes → File.h. Round 2: qsort → in-house
FolderSort/FolderCompare. Round 3: File real I/O (fopen/fread/fwrite/fseek);
float print via Ryu (TRyuF2S/TRyuD2S); TDecimalFloat 7 bugs fixed (dormant);
TSConcat template fix; seam round-trip. Round 4: Stringf.hxx StringC block
deleted (was hallucinated C-lib wrappers + recursive self-calls); <cstring>/
<cstdlib> removed; File.h missing `#endif` restored after _Yes0/_No0 strip;
SScan float overloads left on sscanf+TStringFloatStop (now item #1 above).
See git history for details.
