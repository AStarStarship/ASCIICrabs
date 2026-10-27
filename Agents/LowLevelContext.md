# Low-Level Agent Context — File.hxx ASCII Data Type Migration
Dated 2026-10-27 (UTC, round 3, final context dump).
Workspace: /home/astar/AStarStarship/ASCIICrabs, branch Issue376 (ahead 2 of origin/master).

## STATUS (round 3, DONE + verified 2026-10-27)
The LowLevelFileAPITask.md increment is COMPLETE: File does REAL I/O.
`file << 123 << "hello"` writes bytes to the filesystem; `in.Read(buf, sz)` reads
them back; OpenRead/OpenWrite/Seek/Size exist; round-trip seam test passes.
Float rendering now goes through the house's verified Ryu wrapper (TRyuF2S/TRyuD2S)
instead of the stubbed TSPrint — shortest-round-trip scientific format ("4.5E0"),
matching the 03.FtoS known-answer style.

### Final change set (git diff --stat HEAD)
    File.h             |  63 ++++++--
    File.hxx           | 414 ++++++++-----
    Puff.hpp           |  54 +++----
    Stringf.hpp        |  13 +-
    _Seams/20.File.hxx |  86 +++++++++++
    5 files changed, 397 insertions(+), 233 deletions(-)
Nothing committed (standing instruction). Untracked (pre-existing round work):
Ryu.h/.hpp/.hxx, _Seams/Test.crabs, _Seams/_Main.hxx, pch.h, _Seams/_Config.h.

### Verification evidence (fresh, this session)
Canonical commands (no make/cmake target works — CMake is stale, it references
_Seams/_Main.cpp before the rename to _Main.hxx):
    # SEAM=20 File seam (debug; includes write/read round-trip test)
    g++ -std=gnu++23 -x c++ -g -DSEAM=20 -I _Seams -I . -o /tmp/crabs20 _Seams/_Main.hxx
    /tmp/crabs20
    # Full release tree (default SEAM_N)
    g++ -std=gnu++23 -x c++ -g -I _Seams -I . -o Crabs _Seams/_Main.hxx && ./Crabs
Both build clean and print "Unit tests completed successfully! (:-)+=<"; the
SEAM=20 run lists: Testing File("_Seams/Test.crabs"), Testing Folder
("_Seams/TestFolder"), Testing File write/read round-trip("_Seams/TestIO.crabs"),
File test complete.

Ad-hoc verification script (ALL GREEN, exit 0, re-run 2026-10-27):
    /home/astar/.hermes/cache/scratch/hermes-verify-file-api.sh
Covers: (1) SEAM=20 build+run with FAILURE-line grep, (2) release tree build+run,
(3) float battery + File API end-to-end probe, (4) fixture hygiene (no
TestIO.crabs / TestFolder / temp file left behind). Self-deletes its temp
binaries/logs. Scratch is pruned after 24h idle.
Float battery (probe at
/home/astar/.hermes/profiles/low-level/cache/scratch/floatprobe.cpp): 24 FPD +
10 FPC values rendered via TSPrint<CHA>, bit-exact round-trip through
strtod/strtof, 0 failures. Includes denormals, DBL_MAX/FLT_MAX, 2^53 boundary
(2^53+1 correctly renders the same as 2^53). Known-answer style formats
confirmed: 0.1 -> "1E-1", 0.25 -> "2.5E-1", 8.999999E9f -> "9E9",
-2.47E-43f -> "-2.47E-43", 4.5 -> "4.5E0", 100.0 -> "1E2".
End-to-end user story: `out << 123 << "hello" << 4.5;` read back -> exactly the
13 bytes `123hello4.5E0` in the file.

## API shape decisions (report to Captain)
- READ call is the MEMBER `ISW File::Read(void* buf, ISW bytes)` (returns bytes
  read). The task's declared 3-arg `File& operator>>(File&, void* buf, ISW bytes)`
  is ILLEGAL C++ — overloaded operators must have exactly 2 parameters (compiler:
  "must have exactly two arguments"). Consumer pattern:
    in.OpenRead(uri); ISW sz = in.Size(); ISW got = in.Read(buf, sz); in.Close();
  (The task sketch `in >> (buf, sz)` is a C++ comma expression = `in >> sz` and
  could never call a 3-arg operator anyway.)
- WRITE stays `operator<<(File&, X)` free functions -> File::Write -> fwrite/fputc.
- Size() returns full file size and RESTORES the prior stream position
  (SEEK_END + ftell + SEEK_SET). Seek(offset, whence) returns the NEW ABSOLUTE
  position (fseek + ftell), whence = SEEK_SET/SEEK_CUR/SEEK_END from <stdio.h>.
- Float rendering = Ryu scientific-notation shortest round-trip ("4.5E0"), the
  house's verified canonical renderer (Ryu.h/Ryu.hpp/Ryu.hxx, verbatim port of
  Ulf Adams' ryu; original C source at ~/3P/ryu/, known-answer suite in
  _Seams/03.FtoS.hxx).

## Round 3 files changed
### File.h
- OS shim block: added `#include <stdio.h>` (FILE* lives with the decls; File.hxx
  stays stdlib-free).
- File struct: new public methods `ISN OpenRead(const CHR* path);` `ISN
  OpenWrite(const CHR* path);` `ISW Read(void* buf, ISW bytes);` `ISW Seek(ISW
  offset, ISN whence);` `ISW Size();` ; default ctor gains `fp_(NILP)`; private
  `FILE* fp_;`.
### File.hxx
- `File(const CHR*)` ctor gains `fp_(NILP)` (CAUTION: a fuzzy patch earlier
  mangled this ctor — `uri_(uri)` -> `uri_(NILP)` + dropped brace; caught and
  fixed; re-read the ctor after ANY patch to this region).
- `Close()`: fclose(fp_) + fp_=NILP before is_open_=0.
- OpenRead/OpenWrite: TSCodeCount length check -> -AErrorInvalidInput (EINVAL);
  fclose any live fp_ first; fopen "rb"/"wb"; fopen failure -> -1; is_open_=1.
- Read: EINVAL for bad buf/negative, EBADF when closed, returns ISW(fread(...)).
- Seek: EBADF when closed; fseek then ftell -> absolute position; 0 on error.
- Size: position via ftell/fseek(SEEK_END)/ftell, position restored; 0 on any error.
- Write overloads REWRITTEN (previously: all no-ops, or char->number, or
  sign-truncating, or float->int-truncating — none had ever flushed anywhere so
  nobody noticed):
  - Write(CHA): fputc; Write(const CHA*): fwrite + TSCodeCount<CHA,CHC,ISW>.
  - Write(ISA/ISB/ISC) -> Write(ISD): TSPrintSigned<ISD,IUD,CHA> into 24-byte
    socket (was: sign lost via IUC, then no-op).
  - Write(IUA/IUB/IUC) -> Write(IUD): TSPrintUnsigned<IUD,CHA> into 24-byte socket.
  - Write(FPC/FPD): TSPrint<CHA> into 32-byte socket (was: float TRUNCATED to int
    then no-op).
  - CHB/CHC (gated): widen to CHA single chars / loop for strings.
- TextFile::Print(CHA/CHB/CHC): NOW writes the character itself (was
  Print(IUA(item)) -> rendered the ASCII NUMBER, e.g. 'a' -> "97"). This is the
  drain point for every string/numeric << (TPrint loop calls Print(CHA) per char).
- TextFile::Print(ISC): latent dead-code bug `ISD(value)` -> `ISD(item)` (value
  undeclared; only compiled on non-8-byte CPUs).
- TextFile::Print(FPD): socket Size 24 -> 32 (TRyuD2S needs 25 incl. nil; 24
  ALWAYS returned NILP -> floats would silently write nothing).
### Puff.hpp
- `#include "Ryu.hpp"`; the four STUBBED `TSPrint(..., FPC/FPD value)` overloads
  (previously `return start;` — wrote NOTHING, leaving callers to read stack
  garbage) now route to `TRyuF2S<CHS>` / `TRyuD2S<CHS>` (capacity-checked,
  widening, nil-terminated wrappers in Ryu.hpp). Ryu is the canonical float path.
- TDecimalFloat class (the OLD Puff float renderer, NEVER COMPILED before this
  session — templates are lazy) had 7 latent bugs fixed:
  1. `Size = sizeof(Float) >= SizeMax ? 0 : ...` -> `>`: with SizeMax=8, double got
     Size=0 -> every derived constant (MSb, MantissaSize, ExponentBias, ...)
     wrapped to garbage.
  2. ExponentMaskUnshifted used `sizeof(Size)` (always 4, the enum constant!) ->
     `sizeof(Float)`.
  3. TMiddleBits<IS, MSb_, LSb_> was an IDENTITY function (both shifts computed
     equal -> value<<n>>n = value) -> rewritten as a real inclusive bit-range
     extract: `(value >> LSb) & ((1<<count)-1)`, full-width mask guard.
  4. ctor exponent field: `TMiddleBits<IU, MSb-1, MantissaSize-1>` -> `MantissaSize`
     (bit MantissaSize is the TOP MANTISSA bit, not exponent — off-by-one field).
  5. ctor implicit bit: `coefficient + (IU(1) << ExponentSizeBits)` ->
     `<< MantissaSize` (IEEE754 implicit bit sits above the stored mantissa).
  6. operator* -> Multiply(IU, IS): ambiguous for Binary64 (IUD,ISC) -> added
     `Multiply(IUD, ISC)` exact-match overload routing to (IUD, ISD).
  7. NormalizeBoundary() gcc branch: `res.f <<= (CoefficientSize - CoefficientSize - 2)`
     (= shift by -2, UB) -> `MSb - MantissaSize - 1` (matches the MSVC branch's
     "normalize top bit to MSb" semantics; e adjusted by the same amount).
  NOTE: TDecimalFloat is now DORMANT (Ryu is canonical; IEEE754Pow10F4 float LUT
  in Puff.hxx is still an empty `{0}` stub — Binary32 TDecimalFloat remains
  unverified and unneeded).
### Stringf.hpp
- TSConcat(CHS*, CHS*, const CHS*): `TSPrint<CHS*>(...)` -> `TSPrint<CHS, CHT>(...)`
  — the explicit `CHS*` template arg passed a POINTER as the char-type parameter,
  failing every TSPrint overload. Latent: only instantiated on WIN32 File::Open
  paths, so Linux builds never saw it.
### _Seams/20.File.hxx
- Added write-then-read round-trip before "File test complete.": OpenWrite
  `_Seams/TestIO.crabs`, `out << "abc" << 123 << 4.5`, Close; OpenRead; expected =
  "abc" + TSPrintSigned(123) + TSPrint<CHA>(4.5) built with SPrint + TSConcat
  (independent of the write path's internals, same renderer); Size() > 0;
  Seek(0, SEEK_SET) == 0; Read(buf, 127) got > 0; Close; TSEquals<CHR>(buf,
  expected). Cleans up with ::remove.
- Test gotchas hit: A_ASSERT macro takes ONE arg — template commas need an outer
  paren: `A_ASSERT((TSPrintSigned<ISD, IUD, CHA>(...) != NILP));`; declare statics
  before first use.

## Verified API facts (carried from rounds 1-2, still true)
- Types (this box): CHA=char, CHR=char, CHD=char, ISC=int32_t==int, ISD=int64_t,
  ISW=intptr_t, IUD=uint64_t; FPD=double, FPC=float; TME=TMD=ISD; ISN=int.
  `file << 123` binds to operator<<(File&, ISC) (int==ISC), `<< 4.5` to
  (File&, FPD).
- RAMFactory: `typedef IUW* (*RAMFactory)(IUW* origin, ISW bytes);` (Autoject.h);
  ObjectFactoryHeap alloc/delete; Folder still uses it for files_.
- TSEquals: NILP = NOT equal, pointer = equal.
- TSCompare(x, y) returns y-x (POSITIVE when x < y) — do not "fix"; negate at call.
- Seam table: CORE=1, ITOS=2, FTOS=3, UNIPRINTER=4, ..., FILE=20, RELEASE=23.
- At SEAM=20 the `#if SEAM <= CRABS_ITOS/FTOS -> STD_COUT` mirror branches in
  TextFile::Print are INACTIVE (the socket path is live).

## Known gaps / risks (round 3)
- SEAM=3 (FtoS known-answer seam) DOES NOT BUILD — pre-existing working-tree
  drift, NOT caused by round 3 (proven: with Puff.hpp/Stringf.hpp reverted to
  HEAD the identical errors appear): Stringf.hxx:200/206 `TStringFloatStop` not
  declared; BigInt.h:127 `Autoject` does not name a type; BigInt.hxx:30
  RAMFactory/ObjectFactoryHeap undeclared. Ryu's known-answer suite will run once
  the tree is repaired; until then Ryu correctness rests on the battery probe
  (bit-exact round-trip, formats matching the known-answer style).
- Windows/MSVC paths untested (Linux GCC only; _WIN32 branches in File.h shim).
- A_AVOW does NOT return on failure — grep test output for FAILURE lines; fixture
  leftovers after a run are a failure signal.
- CMake is stale (references _Seams/_Main.cpp before rename to _Main.hxx) — use
  the direct g++ commands above, not make.
- Git workflow (user rule, 2026-10-27): agents only edit files; user (Cale) does
  `git add --all` + YOLO merge/commit himself, no advanced git. No staging,
  scoped commits, or pushes by agents (a scoped agent commit was made and
  reverted this session — `git reset --mixed HEAD~1`).

## Pitfalls (rounds 1-3, still relevant)
- astar-mary (sibling agent) may be live-editing this tree. Re-read any file
  immediately before patching; keep scope tight.
- Working tree has ~40 pre-existing modified files on Issue376 (branch work, NOT
  ours): don't commit, don't fix them, don't be surprised.
- `A_AVOW(expected, actual)`, `A_ASSERT(cond)`; `D_(code)` = code; D_COUT only in
  debug seam builds.
- Patch tool fuzzy matching: on File.hxx it once "context_aware"-matched the File
  ctor and silently swapped `uri_(uri)` for `uri_(NILP)`. Always read back fuzzy
  matches; on the third fuzzy failure of a region, rewrite with write_file.
- GCC14 PCH trap: g++ given a .h/.hxx MAIN source saves a .gpch and exits 0 —
  always pass `-x c++`.
- Probe gotcha: never give a probe binary the same /tmp path as its fixture dir
  (EEXIST -> opendir NILP -> segfault in readdir).
- AGENTS.md: "Do not create files unless you have approval" — the Captain approved
  Agents/LowLevelContext.md (this file) and the scratch probes in the Hermes
  cache scratch dirs (both /home/astar/.hermes/cache/scratch and the profile
  scratch /home/astar/.hermes/profiles/low-level/cache/scratch).
- git status at session start: branch Issue376 ahead 2, 1 staged (rename
  _Main.cpp -> _Main.hxx), 40 modified, untracked: Ryu.h/.hpp/.hxx,
  _Seams/Test.crabs, _Seams/_Main.hxx, pch.h, _Seams/_Config.h.

## Historical (rounds 1-2, condensed)
Round 1: size_t->ISW, TMC->TME, malloc/free -> RAMFactory(ObjectFactoryHeap),
memset/memcpy -> ArrayFill/ArrayCopy, OS include block moved File.hxx -> File.h,
TSEquals polarity fixes in File::Open/CompareNames.
Round 2: qsort/_og_file_cmp removed -> in-house FolderSort/FolderCompare (Hoare
quicksort, dirs-first, TSCompare sign negated at call site); CompareNames made a
pure BOL; seam test asserts full sorted order with an out-of-order fixture;
verified with a single-TU probe.
