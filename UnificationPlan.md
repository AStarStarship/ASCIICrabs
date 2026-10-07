# ASCIICrabs — Unification Plan (Milestone 3)

> **Status:** DRAFT for Captain's edit (VS Code Server).
> **Author:** master-agent (orchestration) · 2026-09-09.
> **Branch:** `Issue244` · **Repo:** AStarStarship/ASCIICrabs
> **Goal:** Close out the 10 open issue tickets, land them as one coherent
> **Milestone 3**, and put the codebase on a stable footing so work can move to
> the *next* milestone without re-litigating the data model or the wire protocol.
>
> This file is the single source of truth for *what Milestone 3 is and in what
> order*. The kanban board (`astarship`) tracks *who is doing what right now*.
> The two stay in sync via the GitHub issue number on every card.

---

## 0. Why "unification"

Milestone 2 was just completed. Since then three things have drifted apart and
they must be re-joined before anything new is built on top:

1. **The data model** (ASCII Data Types / `DTB`) was remapped but the Crabs
   seam wasn't updated to match → **#244**.
2. **The wire layer** (BSeq + Protocol + RPC) still assumes the old types and
   has integrity gaps → **#328, #353, #358**.
3. **The surface area** Carabs and the model work (MUSE / spectral tokenizer)
   need is missing or untested → **#267, #325, #331, #338, #351, #355**.

"Unification" = get all three back in agreement, prove it compiles and the seams
pass, and cut Milestone 3.

---

## 1. The 10 open tickets, grouped by theme

### A — Data-model foundation (must land first)
| GH# | Title | Files | Notes |
|---|---|---|---|
| #244 | Crabs: Update with remapped ASCII Data Types | Crabs seam, `_Spec/Data` | Milestone 2 follow-up. **Compile is the bar; unit test need not pass yet.** Spec updated alongside. |
| #267 | Add ISQ / IUQ (n-bit signed/unsigned integer) | `BigInt.h`, `BigInt.hxx` (new) | Behave identically to JS `BigInt`. New files — greenfield. |

### B — Wire / protocol layer (depends on A)
| GH# | Title | Files | Notes |
|---|---|---|---|
| #328 | BSeq: sequence length → VUC (32-bit varint) | `BSeq.hxx` | Replaces `ISC` length so DTB-using algorithms work again; matches ISY/ISZ half-size. |
| #353 | Protocol: transfer elements by fundamental type, not all map types | `_Spec/Protocol/*` | Kills the "exponential explosion." Each element type bounded + validated; offsets must resolve into a given socket. |
| #358 | Update RPC protocol (frame integrity, wire format) | `Crabs/_Spec` RPC RFC | **Biggest spec lift.** CRC (algo/poly/init/reflect/xor/coverage/failure), integrity-vs-authenticity (CRC vs MAC), normative packet framing, ≥3 test vectors. |

### C — Surface area & performance (parallel with B)
| GH# | Title | Files | Notes |
|---|---|---|---|
| #355 | File.Fix: TextFile untested + compile errors | `File.h`, `File.hxx`, `_Seams/19.File.hxx` | `TextFile` = string File (binary + string overloads). Fix the constexpr URI-array decl/impl issue. |
| #351 | Add missing functions required for Carabs | (see issue body for the list) | **Body currently lists the problem but not the function list** — needs the list filled in before it's shippable. |
| #325 | Ability: parse regex string | `Regex.hpp` (exists) | "Fastest Unicode library on Earth, but no regex parser." |
| #338 | Write-combining buffer cache optimization | (TBD) | Read a memory address without polluting / waiting on cache sync. |
| #331 | Tokenizer.Describe: Spectral Tokenizer | (new) | Angles on S¹ → lift to S², 12-token window covariance (3×3 SPD), monoid of 3×3 matrices. Feeds MUSE / dot-product attention. Research-heavy. |

**Theme counts:** A=2 · B=3 · C=5 · total=10.

---

## 2. Sequencing (dependency-ordered)

The rule: **the data model is settled before the wire layer is written on top of
it, and Carabs's needs are satisfied before we declare the surface complete.**

```
PHASE 0  Ground-truth & branch hygiene
   - Confirm Milestone 3 scope = these 10 tickets (Captain sign-off here).
   - Confirm the #351 missing-function list is actually written into the issue.
   - Decide: one branch (Issue244 continues) or a fresh wt/milestone3?

PHASE 1  Data-model foundation        [low-level]   (gate: compile green)
   #244  →  #267
   Exit: Crabs seam compiles against remapped types; ISQ/IUQ present and BigInt-parity
         unit test passes. SPEC/Data updated.

PHASE 2  Wire / protocol layer         [low-level + attorney? no — low-level]
   #328  →  #353  →  #358
   Exit: BSeq uses VUC; protocol transfers by fundamental type; RPC RFC has normative
         frame format + CRC spec + ≥3 test vectors. (Spec-first: write the RFC, then code.)

PHASE 3  Surface area & perf           [low-level]
   #355  →  #351  →  #325  →  #338
   Exit: TextFile tested + compiles; Carabs's missing fns present; Regex parses;
         write-combining path in place.

PHASE 4  Model / research (can run in parallel from PHASE 1)
   #331  [low-level, research-heavy — allow longer runtime]
   Exit: Spectral tokenizer described in _Spec + a working seam prototype.

PHASE 5  Integration gate (the "Milestone 3 done" bar)
   - `g++ -g -I. -I./_Seams -o Crabs _Seams/_Main.cpp` → clean compile.
   - `./Crabs` → all seams in the tree pass (or each failure is a known, documented
     exception with a tracking note).
   - _Spec updated to match code (Data + Protocol + RPC).
   - No C++ std-lib violations introduced (Chimera / no-std rule holds).
```

**Parallelism note:** PHASE 4 (#331) is independent of the wire layer and can run
side-by-side with PHASE 2/3. PHASE 1 is the hard gate — nothing in PHASE 2 merges
until #244's types are settled, or we'll be rewriting BSeq/RPC twice.

---

## 3. Definition of Done — Milestone 3

A ticket is **done** when:
- [ ] It is implemented on the branch and the Crabs binary **compiles clean**.
- [ ] Its seam test passes (or the test is explicitly waived in the issue with a reason).
- [ ] `_Spec` is updated to match (for any spec-bearing ticket: #244, #328, #353, #358, #331).
- [ ] No `#include <string|vector|iostream|map|set|algorithm>` added to Crabs-family code
      (ASCIICrabs itself may use std for dev/testing tooling only — per AGENTS.md).
- [ ] The GitHub issue is closed with a commit reference; the kanban card is marked complete.

**Milestone 3 is done** when: all 10 issues closed · `./Crabs` green · `_Spec`
consistent · a tag (e.g. `v0.7` / `Milestone3`) is cut and pushed.

---

## 4. Risks & open questions (decide before PHASE 1)

1. **#351 is underspecified.** The issue body says "Crabs is missing the following
   functions:" and then stops. **Blocker for PHASE 3.** → Action: fill the list
   (low-level, from the Carabs build errors) before PHASE 3 starts.
2. **#358 is the largest single lift** (RPC frame integrity + normative wire format +
   test vectors). It is a spec-first task; scope it as its own sub-epic if it
   threatens to swallow the rest of the milestone.
3. **#331 is research, not plumbing.** Spectral tokenizer touches the MUSE
   transformer design. Give it its own longer runtime and don't gate the
   milestone on a *finished* model — gate it on a working seam + spec description.
4. **Compile-vs-pass distinction.** #244's bar is *compile*, not *pass*. Make sure
   PHASE 5's "seams green" bar doesn't silently require #244's test to pass.
   Reconcile: either raise #244 to "pass" or document the known-failing seam.
5. **Branch strategy.** Currently on `Issue244`. Decide now: continue there, or cut
   a clean `wt/milestone3` and land #244 first. (Affects worktree + kanban `--project`.)

---

## 5. Ownership & cadence

- **Owner (all tickets):** `low-level` (ASCII Crabs C++ role) — assigned on the
  `astarship` kanban board, one card per GitHub issue, idempotency key
  `github-ASCIICrabs-<n>`.
- **Reviewer / integration gate:** master-agent (this profile) verifies the PHASE 5
  bar before anything is declared done — I check the compile + seam output, not
  just the "agent says it's done."
- **Cadence:** land PHASE 1 first (it unblocks everything), then run PHASE 2/3/4
  in parallel where independent.

---

## 6. Ticket → card → GitHub crosswalk

| Kanban card | GH# | Theme | Phase |
|---|---|---|---|
| t_46f1e1e7 | #244 | A | 1 |
| t_16982064 | #267 | A | 1 |
| t_3a65f750 | #328 | B | 2 |
| t_59e97ddc | #353 | B | 2 |
| t_3bd74cde | #358 | B | 2 |
| t_a5d5b241 | #355 | C | 3 |
| t_09f747e8 | #351 | C | 3 |
| t_e3bdd1ab | #325 | C | 3 |
| t_cca35103 | #338 | C | 3 |
| t_930c9f88 | #331 | C (research) | 4 |

---

## 7. The escape hatch (what "lift the rock, unchain me" actually means)

The rock is the *open* backlog. The chain is the *sequence* you have to hold in
your head. This plan externalizes both:

- The **rock** is now 10 named, grouped, sequenced cards with a clear gate.
- The **chain** is PHASE 1 → 5. You don't have to remember the order — the board
  and this file do.
- **To go outside and play:** you only need to make the §4 decisions (they're
  short), then the fleet runs the phases. You get pings at phase gates, not
  mid-grind. The next action is *yours and small*: answer the 5 open questions in
  §4. Everything after that is the agents' problem.

> Edit this in VS Code Server. The 5 §4 questions are the only things that need
> your hand before PHASE 1 starts.
