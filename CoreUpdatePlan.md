# ASCIICrabs — Core Update Plan (from the FVM probe)

> **Status:** DRAFT for Captain's edit.
> **Author:** master-agent (orchestration) · 2026-09-10.
> **Repo:** AStarStarship/ASCIICrabs
> **Trigger:** the Freedom Voting Machine "vibe-code slop" pass (see
> `~/FreedomGovernment/FreedomVotingMachine/Code/README.md`, 20-point
> shortcut list). The FVM was the forcing function: it revealed what the
> core must provide for a real product, and — once we tried to *link* the
> core — the core's true cross-compiler state on this host.
>
> This plan is the **ASCII Crabs core update plan** that the FVM's problems
> call for. It complements `UnificationPlan.md` (Milestone 3, the 10-ticket
> data-model/wire-layer re-join); where they overlap, this plan is the
> FVM-driven slice and `UnificationPlan.md` is the broader milestone.

---

## 0. The non-negotiable invariant (read first)

The all-contiguous stack machine is **the ABI strategy, not just a storage
trick.** The API was designed so that **switching between MSVC and GCC is
instant and painless** — that is a primary core design requirement, the
reason the data model is all-contiguous: multiple return values are packed
into the single-word return register. 128-bit integer types are emulated via
contiguous word pairs (MSVC has no native 128-bit int; GCC does, but the
types must behave identically on both).

**Every change in this plan must preserve that invariant.** A fix that makes
GCC cleaner but breaks the MSVC side, or that changes the contiguous packing
or the single-word-return contract, is **out of scope / rejected.** The dual
target is the acceptance gate, not a nice-to-have.

**Acceptance criterion for the whole plan:** the core builds **and** passes
its seam tests on **both** MSVC (v143) and GCC 13, with the build switching
between them by changing the toolchain (not the code), and the all-contiguous
single-word-return ABI unchanged.

### Architectural philosophy (the "why" behind the type system + endianness)

The load-and-store machine is **not** a clean abstraction — it's a physical
substrate, and all the ISAs that matter (x86, ARM, RISC-V) reduce to the same
load-and-store memory model at the level that matters. The core builds *on*
that shared ground rather than pretending the ISA is an idealized machine: a
contiguous POD layout that packs multiple return values into the single-word
return register is portable across them **because** they all reduce to the
same "move contiguous bytes between register and memory" primitive.

**CISC is on the way out; RISC-V is the emerging standard.** The design bets
on the RISC-V direction (regular, load-and-store, open, eating embedded +
new-server) rather than chasing x86 CISC complexity. RISC-V is
overwhelmingly little-endian in the ecosystem and has regular power-of-2
register/word widths — which is exactly why the core's type system lines up
with the hardware instead of fighting it:

- **Even powers of 2 only** (8/16/32/64/128-bit; no 7-bit). A 7-bit int would
  fight the substrate — no register is 7 bits, no load/store is 7 bytes. The
  *variable-length* need is met on the wire by the varint MSB-variant encoding,
  not by an in-memory 7-bit type.
- **Little-endian first; BE deferred.** Everyone uses LE. BE is a one-shot
  byte-swap conversion a subagent can do if a BE target ever shows up, but
  carrying it as a supported target now only slows the agents for a
  locked-down-ancient-pro-machine market.

The through-line: **align with the actual machine (contiguous, even-power-of-2,
LE, load-and-store), don't abstract it away.** A proposal to add a 7-bit type,
BE portability, or a non-power-of-2 width is a proposal to fight the substrate
and is out of scope by default.

### Design decisions (Captain-confirmed, 2026-09-10) — these are intent, not gaps

- **Little-endian first; big-endian explicitly deferred (not architecturally
  excluded).** BE support is **out of scope now** because carrying it as a
  supported target slows the agents down (extra branches, dual-endian seam
  coverage) for a market that's locked-down ancient pro machines. It is **not**
  a hard "BE is impossible" statement — it's a **one-shot byte-swap conversion
  a subagent can do in a single pass** if a BE target ever actually shows up.
  The `Array.hxx:20` `#error` ("poopy mode / little endian") is an
  **intentional guard, keep it** — the only defect there is the missing
  terminating quote in the `#error` string (a warning to fix, not a feature).
  LE-only now is what keeps the all-contiguous byte layout unambiguous and the
  single-word-return packing predictable. **Do NOT "fix" this into byte-order
  portability this pass.** If BE is ever needed, it's a separate one-shot
  conversion task, not a change to this plan.
- **No 7-bit integer type.** The old 7-bit int is dropped (the "BS"). The
  *essence* is preserved via the **varint MSB-variant encoding** on the wire
  (variable-length integers where the natural width is variable — BSeq
  lengths, etc.). Split: **fixed-width ASCII Data Types for in-memory POD**
  (8/16/32/64/128-bit, no 7-bit) + **varint MSB encoding for the wire
  layer.** Do not reintroduce a 7-bit in-memory type.

---

## 1. Verified current state (2026-09-10, this host: Ubuntu, GCC 13.3)

Grounded by direct build/run, not assumed. Correct build command:

```
g++ -std=c++23 -I. -I./_Seams _Seams/_Main.cpp -o Crabs && ./Crabs
```

(Note: the FVM subagent's finding #2 claimed the core "fails to compile under
GCC 13." That was **wrong** — it used a bad include flag (`-I_SeamS` instead
of `-I./_Seams`). With the correct flag the core **compiles clean, exit 0.**
Do not carry that finding into the plan as a compile blocker.)

- **Compiles on GCC 13:** ✅ (exit 0). **12 warnings, 0 errors.**
- **Runs, but a seam test FAILS:** the `TestMap` seam (`_Seams/12.Map.hxx`)
  asserts `map.Find(domain[i]) == i` and gets `0` (not found) for all
  entries → `A_AVOW_INDEX` fails → binary exits 141. **This is a real
  functional failure on the GCC side** (either `AMap::Find`/`Add` or
  `Random()`/endianness — see §3 work item C).
- **The 12 warnings** (block a clean `-Wall -Werror` dual-target build):
  - `Array.hxx:20` — `#error` message missing terminating quote (warning only
    because the non-LE branch isn't taken on x86-LE).
  - `Hash.hpp:28`, `Puff.hxx:33/143` — integer constant too large → unsigned
    (needs `ULL`/`__int128`-safe literals or typed casts).
  - `String.hpp:14` vs `_Release.h:28` — `D_COUT_STRING` macro redefined.
  - `_ConfigHeader.h:308-310` — deprecated enum-enum-conversion
    (`ATypeESBase + _CHA`); needs explicit cast.
  - one `format` warning + one `comparison` warning (see build log).

- **MSVC side:** not buildable on this Ubuntu host (no `cl.exe`). The MSVC
  build is the known-good reference (`__Crabs.vcxproj`, v143). The plan
  treats "MSVC stays green" as verified-by-construction until the Captain
  runs a Windows/WSL build; the GCC side is what we can fix here.

### What the FVM actually needs from the core (the demand side)

From the FVM's 20-point list, the core must provide (and currently the FVM
stubbed with local typedefs because it was being defensive about the core):

1. A **GCC-13-compiling + -Werror-clean** core (so the FVM can `#include
   <_Config.h>` + `namespace _` instead of `FVMTypes.h` stand-ins).
2. Real **`BIn`/`BOut`/`BSeq`** (the audit ring) — the FVM used an in-memory
   byte-buffer; the core must give it the real Script-protocol ring with
   B-Sequence headers, hash verify, and ring wraparound.
3. Real **`Hash`/`BigInt`** (not FNV-1a) — collision-resistant, and
   `BigInt` must be the 128-bit emulated type that works on MSVC.
4. Real **`COut`/`SSPrinter`/`Uniprinter`** (the FVM used `<cstdio>`).
5. A real **`Clock`** (TMC/TMS/TMT/SSD/SSE per `_Spec/Requirements.md` #24).
6. **NEW (FVM-revealed, not in the old plan): a real anonymization
   primitive** — a blinded one-way token for the semi-anonymous paper ledger.
   The FVM's `FvmMakeToken` = `FNV(voter_secret*K ^ counter)` is linkable if
   the secret is known; a voting machine needs a one-way/blinded token. This
   is a core concern (it's a primitive, not app logic) and is **new scope**
   the FVM surfaced.

---

## 2. Sequencing (dependency-ordered)

```
PHASE A  Make the GCC side clean + green (prerequisite for everything)
   A1. Fix the 12 warnings → -Wall -Werror clean on GCC 13.
   A2. ROOT-CAUSE + FIX the TestMap failure (AMap::Find/Add or Random/endianness).
   A3. Re-run full seam tree on GCC 13 → all seams pass, exit 0.
   A4. Captain: confirm MSVC side still green (Windows/WSL build) — invariant check.

PHASE B  Give the FVM the real core (the "link it" milestone)
   B1. Replace FVMTypes.h stand-ins with real <_Config.h> + namespace _.
   B2. Replace FVMAudit.h byte-ring with real BIn/BOut/BSeq (Script protocol).
   B3. Replace FVMHash.h FNV-1a with real Hash/BigInt.
   B4. Replace FVMOut.h <cstdio> with real COut/SSPrinter.
   B5. Wire real Clock (fixed timestamps → Crabs Clock).
   B6. Rebuild + re-run the FVM happy path + DISAGREE test against the REAL core.

PHASE C  Close the FVM's real-auditability gaps (product correctness)
   C1. Real persistence: paper ledger + blockchain → SubsecondDb (Postgres).
   C2. Make triple-count #3 (blockchain) INDEPENDENT: re-derive the tally from
      stored block bytes, not a side-tally updated at insert.
   C3. Real anonymization primitive in the core (§0 item 6); FVM uses it.
   C4. Real nonce policy + real timestamp (PoW or nonce-source decision).
   C5. AI-assisted verification: replace the stub with a real rule engine /
      model hook (scope: decide stub→rule-engine→model progression).
   C6. Real CrabsBed input (Button/Switch/RotaryKnob/Unicontroller) — firmware.

PHASE D  Hardening (after the FVM runs end-to-end on the real core)
   D1. Build a real seam-test tree under _Seams/ for the FVM (error paths,
      tamper cases, ring wraparound, ledger-malformed-line cases).
   D2. Ring wraparound + backpressure in BIn/BOut (the FVM's #10/#17).
   D3. Robust ledger-text parser + error reporting (FVM #16).
   D4. Configurable sizes (FVM #14: 4 candidates/64 lines/16 blocks are hardcoded).
   D5. Compile-time size guarantees on hash/hex buffers (FVM #18).
   D6. Consistent status-code printing (FVM's unsigned-rc bug: negative
      status codes render as unsigned — fix the print path).
```

**The gate between phases:** A must be fully green (GCC + MSVC, invariant
held) before B starts. B must link the FVM to the real core before C. C must
be product-correct before D hardens it.

---

## 3. Work items with the specific fixes

### A1 — 12 warnings → `-Werror` clean (GCC 13)
| File | Line | Fix |
|---|---|---|
| `Array.hxx` | 20 | Terminate the `#error` string literal (add the closing `'`). **Keep the BE guard** — LE-only is intended (see §0 design decisions); only fix the missing quote so it stops warning. |
| `Hash.hpp` | 28 | Make the 64-bit constant explicitly typed (`...ULL` or a `constexpr` of the right width) so it isn't "too large → unsigned." |
| `Puff.hxx` | 33, 143 | Same: `10000000000000000000` needs a typed suffix/cast (it's 10^19, fits in IUD but not in an untyped `int` literal). |
| `String.hpp` | 14 | Guard the `D_COUT_STRING` define against the `_Release.h:28` redefinition (`#ifndef D_COUT_STRING` or pick one canonical site). |
| `_ConfigHeader.h` | 308-310 | Cast the enum base explicitly (`(enum _::ATypeESBase)ATypeESBase + _CHA` or define `_STA` etc. as a single enum) to kill the deprecated enum-enum-conversion. |
| (format) | — | Locate the `format` warning in the build log; fix the `printf`-style format string / arg mismatch. |
| (comparison) | — | Locate the `comparison` warning; fix the signed/unsigned compare. |

**Done when:** `g++ -std=c++23 -I. -I./_Seams -Wall -Werror _Seams/_Main.cpp`
compiles with **zero warnings.**

### A2 — Root-cause the `TestMap` failure (the real GCC-side bug)
`_Seams/12.Map.hxx` `TestMap`: `map.Add(domain[i])` then
`A_AVOW_INDEX(i, map.Find(domain[i]), i)` fails (gets 0 = not found for all
i). **Investigate in this order:**
1. Is `AMap::Add` actually inserting (check `map.Size()` after the Add loop —
   the test prints it; if Size is 0, `Add` is the bug)?
2. Is `AMap::Find` comparing correctly (hash/bucket lookup under the current
   16-bit DTB layout from the Milestone 2 remap)?
3. Is `Random(d)` producing distinct values (if all `domain[i]` are 0, Find
   trivially misbehaves)?
4. LE byte layout: verify the Map's key/bucket byte layout matches the host
   (little-endian, the only supported target — see §0). This is a "does the
   LE layout match" check, NOT a byte-order-portability fix (BE is deferred).

**Done when:** `TestMap` passes and the **full seam tree** runs to exit 0 on
GCC 13. **This is the highest-value single fix** — a failing Map test means
the core's data structure is not trusted on the GCC side, which is exactly
the invariant the FVM depends on.

### B — Link the FVM to the real core
Mechanical once A is green; the FVM's README already lists the exact
substitutions (FVMTypes.h→`<_Config.h>`, FVMAudit.h→`BIn/BOut/BSeq`,
FVMHash.h→`Hash/BigInt`, FVMOut.h→`COut`, fixed timestamps→`Clock`).
**Done when:** the FVM builds against the real core (no `FVMTypes.h`
stand-ins), and the happy path + DISAGREE test still pass.

### C2 — Make triple-count #3 independent (auditability-critical)
The FVM's blockchain count uses a *side-tally* updated when ballots are added
(`FvmChainAddToTally`), so count #3 isn't independent of the input path. The
core/product fix: the blockchain count must **re-derive the tally from the
stored block bytes** (decode each block's ballot batch, re-tally) so it's a
genuinely independent check. This is the property that makes the triple count
a real audit, not a triple read of the same value.

### C3 — Real anonymization primitive (NEW, FVM-revealed)
Add to the core a blinded one-way token primitive (e.g. a keyed one-way
function / hash-commitment so the paper-ledger token cannot be reversed to
the voter identity even if the secret leaks, or a one-time pad blinding).
The FVM's current `FvmMakeToken` is linkable — unacceptable for a voting
machine. **This is new scope the FVM surfaced and the old UnificationPlan
doesn't cover.**

---

## 4. Explicitly OUT of scope (this plan)

- **Rebuilding the core from the `_Spec/` markdown tree via prompt
  engineering** (the S-tier research method) — that's a separate, larger
  effort. This plan *patches* the existing core to the dual-target green
  state; the markdown-tree rebuild is the longer-horizon path the FVM
  ultimately justifies but is not this plan.
- **The 10-ticket Milestone 3 unification** in `UnificationPlan.md` — this
  plan is the FVM-driven slice; the broader milestone proceeds on its own
  schedule and the two stay in sync via the GitHub issue numbers.
- **CrabsBed firmware bring-up** (real mbed-OS-takeover hardware) — C6 is the
  FVM-input stub→real-hardware handoff; the full CrabsBed/mbed reclaim is its
  own workstream.
- **SubsecondDb extension build** — C1 uses it for persistence; building the
  PG extension itself is SubsecondDb's own workstream.

---

## 5. Open decisions (need Captain)

1. **GCC `-Werror` bar:** is the dual-target gate "compiles + seams pass"
   (current, warnings allowed) or "compiles `-Wall -Werror` clean + seams
   pass" (stricter, A1 required)? Recommend the stricter bar — it's what makes
   "instant painless switching" real (a warning that's a warning on GCC but
   an error on MSVC, or vice versa, breaks the invariant).
2. **C3 anonymization design:** blinding/one-way function vs commitment vs
   one-time-pad — pick the primitive (security-critical; may want the
   `attorney`/`qa` profiles + a crypto review).
3. **C5 AI verification progression:** stub → rule engine → real model. Which
   step is in-scope for the FVM's next pass, and does it use the local
   Gemma-4-12B-QAT (192.168.50.221:9931) as the model hook?
4. **TestMap root-cause ownership:** is this a core bug to fix in ASCIICrabs
   (A2) or a test-only issue? Recommend treating it as a core bug until proven
   otherwise — a data-structure test failing is a core trust problem.
