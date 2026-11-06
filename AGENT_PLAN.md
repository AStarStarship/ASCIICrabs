# ASCIICrabs AGENT_PLAN — Gaps found by the iGeek RL-gym work (2026-10-07)

Status: PLAN / GAPS LOG. Owner: low-level engineer. Date: 2026-10-07.
Source: building the iGeek AI-gym API (`~/AStarStarship/iGeek/`) on top of the
latest ASCIICrabs no-stdlib C++23 type system. This file records the
ASCIICrabs-layer functions/fixes that the iGeek RL-gym work is **blocked on or
needs**. It is a gap log, not a full upstream plan — upstream (Crabs) stays
read-only for the iGeek teams; these are requests for a future upstream change.

## Gap 1 (CRITICAL, verified 2026-10-07) — `TRoom` does not instantiate

`TRoom<ROM_A>` (defined in `Room.hpp`) **fails to compile** when any concrete
instantiation is forced. Verified by compiling a probe TU that derives a class
from `TRoom<CHD, CHD>` against the latest tree (GCC 13, C++23). The errors:

```
Room.hpp:332:22: error: cannot convert ‘const ISC*’ {aka ‘const int*’} to
               ‘const DTB*’ {aka ‘const short int*’} in initialization
      "Room", OpFirst('A'), OpLast('A'), ...
Room.hpp:341:18: error: cannot convert ‘const ISC*’ ... to ‘const DTB*’ ...
Room.hpp:348:22: error: ‘OpPush’ was not declared in this scope
      return OpPush(this, crabs);
Room.hpp:310:18: error: ‘class _::TDoor<int>’ has no member named ‘ExecAll’;
               did you mean ‘Exec’?
      this_->ExecAll();
```

Three distinct upstream bugs in `TRoom::Main`/`Star`:
1. **`OpFirst`/`OpLast` arg-type mismatch** — they are being passed
   `const ISC*` where the caller signature wants `const DTB*` (short). The
   `'A'`/`'{'`/`';'` literals are `ISC`-typed; the Op header they flow into is
   `DTB`-typed. Either the literals need a `DTB` cast or the Op header type is
   wrong.
2. **`OpPush` not declared** — `TRoom::Star` calls `OpPush(this, crabs)` but
   `OpPush` has no declaration visible at that point (it is not in scope from
   `Op.hpp`/`Op.h` as included). Either it was renamed/moved or the include is
   missing.
3. **`TDoor` has no `ExecAll`** — `TRoom::Main` calls `this_->ExecAll()` but
   `TDoor<int>` only has `Exec`. `ExecAll` was presumably a batch method that
   got removed/renamed.

**Impact:** every iGeek env/gym that inherited `Room` (now `TRoom`) is broken.
This is the same blocker that forced `iGeekCardsWorld`'s `BlackjackEnv` to be
**standalone** (not `public TRoom`). The iGeek framework migration
(2026-10-07) made `Env`/`Gym`/`Multiverse`/`EnvGoal`/`EnvReward` standalone
for the same reason.

**Fix (upstream):** repair `TRoom::Main`/`Star` so a concrete
`TRoom<CHD, CHD>` (or any `TRoom<...>`) instantiates cleanly. Then the iGeek
env/gym base classes can be re-wrapped in `TRoom` to restore the Script2
data-node role (storing percepts/actions as Crabs datums). Until then, worlds
stay standalone.

**Ask:** either (a) fix the three bugs so `TRoom` instantiates, or (b) confirm
`TRoom::Main`/`Star` are intentionally broken/unused and we should drop the
Script2 data-node inheritance entirely and make `Env`/`Gym` permanently
standalone.

## Gap 2 (needed) — No tensor / linear-algebra primitive

The tensor format must be ported to be BLAS compatible. The inteiton of this library was for hetergenious multi-processing but had skill issues. It has to work with MLK, we're on Intel system. ASCII Crabs is CPU only, it's the Chinse Room ABSTRACT Stack Machine, it's a microframework to build other languages and frameworks with. CrabsTK is the Concrete framework. File doesn't count because it's a Linux primative stream IO, so we just emulate that with Crabs Door Sloot BIn/BOut by feeding B-Sequence Expressions through the slot in the door. These messages must be universally compatible, and BLAS is that. My storing of dynamic array dimensions isn't total trash, but it's unfinished, it's more for build-time tool.

ASCIICrabs has POD types (`FPC`/`FPD`/`ISC`/`CHA`...), containers (`TStack`,
`ALoom`, `AArray`), and RNG, but **no tensor type and no linear-algebra
kernels**. The iGeek policy (`TTransformer`, compact transformer) and
PPO loop (`TPPO`) need, at minimum:

- A row-major float buffer with shape: `TTensor{FPC* data_, ISC rows_,
  ISC cols_}` (the iGeek `Tensor.h` defines this POD; it currently lives in
  iGeek, but it is a generic primitive that belongs upstream).
- Kernels (CPU FPC, no CUDA): `MatMul`, `Softmax`, `LayerNorm`, `Relu`, `Add`,
  `Scale`, `RowLogSumExp`. A fused GPU kernel does these in one pass; on
  CPU they can start as plain loops.

**Impact without it:** the iGeek policy/PPO has to hand-roll these in
iGeek-space (they are declared in `iGeek/Tensor.h`; the implementations are
the next milestone). That works, but it duplicates a generic capability every
RL/ML world would need.

**Ask:** add a `TTensor` POD + the list of elementwise/linear kernels to
ASCIICrabs (a `Tensor.hxx`/`Tensor.hpp`), so any iGeek world doing ML can use
them without copying them. Keep them no-stdlib (plain `FPC*` loops, no
`<vector>`/`<cmath>`-beyond-`sqrt`/`exp`).

## Gap 3 (needed) — Deterministic seeded RNG (partially present)

Our Random number generator needs to be programmed, it's an LSTM RNG CPU only design using a running XOR after every floating point division instruction, so use templates for this. We don't need a large LSTM for this, we get the numbers by training a small number of perceptrons, each algoirhm feeding the running_xor in, you do Netwons algoirhm and we do some basic RNG stats.

## Gap 4 (nice-to-have) — bf16 / low-precision activation path

Reference GPU PPO implementations run the transformer in **bf16**
(`uint16_t` with `f32_to_bf16`/`bf16_to_f32`) for throughput. We are
CPU/console for now, so `FPC` (float32) is the starting precision. But if any
world later wants bf16 activations (or we move to GPU), a `bf16` type
(`IUB`-packed with the two conversion helpers) would belong upstream.

**Ask (low priority):** add a `bf16` POD + `f32_to_bf16`/`bf16_to_f32` if the
throughput case ever justifies it. Not a blocker for the current CPU milestone.

## Gap 5 (needed for console diagnostics) — `ROut` has no `operator<<` for `FPC`/`FPD`

Verified 2026-10-07 (iGeekCardsWorld M3, transformer policy): `RefOut() << <FPC>`
and `RefOut() << <FPD>` **print nothing** (empty). The `ROut` stream overloads
exist for the integer PODs (`ISC`, `IUD`, `IUC`, ...) and `CHA*`, but there is no
working overload for the float types. This breaks any console diagnostic that
wants to print a float (rewards, logits, losses, hyperparameters) — the iGeek
RL-gym work has to cast `FPC` to `ISC` and scale by 1000 as a workaround.

**Impact:** every RL/training world that logs floats to the console is affected.
The PPO loop (M5) and the train seam (M6) both want to print reward/loss/KL
curves; right now they can't print the actual float values.

**Ask:** add `ROut::operator<<(FPC)` / `operator<<(FPD)` (a fixed-decimal
printer, e.g. 6 significant digits) to `ROut.hxx`. Low risk, high value for any
numeric/console work.


## What is NOT an ASCIICrabs gap (handled in iGeek)
- The RL *algorithm* (PPO, GAE, clipped loss, value-clip, entropy) lives in
  iGeek (`PPO.h`, `Policy.h`) — it is domain logic, not a core primitive.
- The vectorized env contract (`Gym` batch arrays) lives in iGeek (`Gym.h`).
- The transformer policy specifics live in iGeek (`Policy.h` /
  `TTransformer`) once the tensor primitive lands.

## Verification note
The `TRoom` instantiation failure (Gap 1) was verified with a throwaway probe
TU that included the migrated iGeek headers and derived from
`TRoom<CHD, CHD>`; the compile errors above are the exact GCC 13 / C++23
output. After making the iGeek env/gym classes standalone (removing the
`TRoom` base), the same probe compiles clean (exit 0) with zero warnings from
the iGeek headers (the 10 warnings are all pre-existing upstream ASCIICrabs
noise). Probe artifacts were removed after verification.
