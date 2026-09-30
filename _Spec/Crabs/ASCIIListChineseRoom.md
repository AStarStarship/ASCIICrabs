# ASCII List Chinese Room

> **Status: DRAFT v2 for Captain review (2026-09-12).** Reframed from the
> first-principles source: `Crabs/MonoidChineseRoom.md` (the CRASM 7-tuple +
> monoid structure) and Searle's Chinese Room thought experiment. The
> #1 goal is **AI philosophy + ASCII C0 mimicry**: we are building the
> **ASCII Chinese Room** — the minimal symbol-manipulation system that an AI
> drives. Data-driven design. The JSX engine comes next (it is "just string
> manipulation," which is exactly what the Chinese Room is). Other APIs and
> languages will use our **ASCII Data Types + JSX + Crabs Machine RPC engine**
> as their base.

## The first principles (what the Chinese Room *is*)

The Chinese Room is **Searle's thought experiment, made into a machine.** In
Searle's room, a person (who doesn't understand Chinese) receives Chinese
symbols, manipulates them according to a rulebook, and produces Chinese
symbols — all *without understanding*. The point: **symbol manipulation is
not understanding.** The "understanding" is supplied by the *system* (the
person + the rulebook + the room) or by an *AI* driving the room, not by any
single symbol manipulation.

We build that room as the **Chinese Room Abstract Stack Machine (CRASM)**,
defined formally in `Crabs/MonoidChineseRoom.md` as a **7-tuple**:

```
CRASM = (Q, Σ, Γ, δ, q_0, Z_0, F)
  Q    — finite set of machine states
  Σ    — input alphabet (Unicode operation indexes; ASCII printable 32-126
         + control chars DC1-DC4 + ESC)
  Γ    — stack alphabet (an IUA stream of ASCII Data Types)   ← "the paper"
  δ    — transition function  δ: Q × (Σ ∪ {ε}) × Γ → P(Q × Γ*)
  q_0  — initial state
  Z_0  — initial stack symbol
  F    — accepting states (the "on" state, Group Automata Theorem)
```

The Room's contents are **four LIFO stacks** (the "pen and paper"):
the **Operation Stack** (free monoid F(Σ)), the **Operand Stack**, the
**Header Stack** (B-Sequence verification), and the **State Stack** (nested
expressions). The **monoid** structure is the engine: operations form the
free monoid F(Σ) under concatenation; expression evaluation is a monoid
morphism E: Σ_ops* → End(S); the **Reset Operation** is the identity element.

**C0 mimicry + AI philosophy (the #1 goal):** the ASCII Chinese Room is the
*minimal* symbol-manipulation system. It is **data-driven** (the rulebook is
data — Script Operations over ASCII Data Types — not hardcoded logic). An AI
(or a person) drives it by feeding operation strings (B-Sequences) in and
reading results out. The **string-in-string-out RPC** (Corollary 1 of
`MonoidChineseRoom.md`) is the interface: input string → operation sequence →
output string. This is the **Crabs Machine RPC engine** that other APIs and
languages will use as their base. The **Uniprinter Unicode API** renders the
symbols. The **JSX engine** (next) is a direct application — JSX is string
manipulation, and the Chinese Room *is* the monoid of operation strings.

**Why it's abstract (the hard rule, made literal):** the CRASM manipulates
*symbols* (ASCII Data Types) on *stacks* (contiguous memory). It has no
hardware, no OS, no I/O devices — it is a *pure symbol-manipulation system*.
When the room's symbols need to touch the real world (a network socket, a
file, a serial port), that is a **backend**, and it lives in **CrabsTK**,
not the core. "Chinese Room **Abstract** Stack Machine" literally means: the
room is abstract; the hardware is not in the room. (See "The hard rule"
below.)

## The roles: Room / Wall / Floor / Door / Portal

The Room/Door/Wall/Floor objects are **roles in the CRASM**, not arbitrary
memory fixtures. Each maps onto the 7-tuple:

| Role | CRASM term | What it is |
|------|-----------|------------|
| **Room** | the CRASM instance `(Q, Σ, Γ, δ, q_0, Z_0, F)` | The symbol-manipulating system. Opaque interior; the contract is its Doors. |
| **Wall** | **Γ storage** — the contiguous block of ASCII Data Types | The Room's *data*: the "paper" the symbols are written on. One contiguous word-aligned block (an Autoject). |
| **Floor** | **q_0 + Z_0** — the initial state + initial stack symbol | The Room's *initial condition*: the ground the machine starts from. A concept, not a fixture. |
| **Door** | **Σ interface** — where B-Sequences enter (BIn) and leave (BOut) | The boundary where the Room touches the outside. A MAP type, installed in a Wall. |
| **Portal** | a **δ transition to another Room** | A B-Sequence that moves the Room to another Room/Agent (changes Q). |
| **Slot** | a **ring-buffer Socket** (BIn and/or BOut) | The actual message-carrying ring buffer inside a Door. |
| **Interrupt** | a **δ transition to Reset** | Causes the Room to Reset (the identity element). |

### Room (ROO)

The **Room** is a CRASM instance — a Chinese Room that reads and writes
B-Sequences (operation strings).

- **Opaque interior.** The Room's stacks (Operation/Operand/Header/State) and
  its Walls are private. The only way in or out is a Door. (Searle: the
  outside only ever sees what comes through the Doors; it cannot inspect the
  rulebook or the stacks.)
- **Starts off with one Wall.** A Room is born with a single Wall — the
  initial Γ stack holding Z_0 (the initial stack symbol). This is the
  "starts off with one wall" rule, read as "the CRASM starts with one Γ stack
  at q_0 holding Z_0."
- **Root-scope Interrupt Operations.** At the root scope, the Interrupt
  Operations DC1-DC4 Script Operations are defined (`RoomContents.md`). An
  `Interrupt` object causes the Room to Reset (the identity element).
- **Leads to another Room or Agent** via a Door or Portal (`ChineseRoomObjects.md`).

### Wall (WAL)

A **Wall** is the **Γ storage** — the Room's data, the "paper."

- **One contiguous block of word-aligned ASCII Data Types.** The all-contiguous
  invariant applied to the Room's data: a Wall is never scattered, paged, or
  a linked list. One block, word-aligned (LE-first, even powers of 2 —
  `CoreUpdatePlan.md` §0).
- **A Wall is an Autoject** (`Autojects.md`): a word-aligned memory-managed
  object using the Socket ADT + RAMFactory. It **can use dynamic memory**
  (grown via the RAMFactory stack-to-heap auto-grow path), but it always
  stays **one contiguous block**.
- **Holds the Room's fixtures.** Walls are "used to install Doors, Filing
  Cabinets, Book Shelves, Windows, Mirrors, and Tables" (`RoomContents.md`).
  A Door, Window, Mirror, and Table are *carved into* a Wall's contiguous
  block.
- **Alignment.** Per `Slots.md`, each Slot in a Wall "shall begin [at a] 64-bit
  aligned memory address and shall have a 64-bit aligned end address."

### Floor (FLR)

> **Definition (derived from first principles; Captain to confirm):** a Floor
> is the **Room's initial condition** — the pair **(q_0, Z_0)**: the initial
> machine state **q_0** and the initial stack symbol **Z_0**. It is the
> "ground" the CRASM starts from. The Floor is *not* a memory container; it is
> the starting point of the symbol-manipulation process.

- **The Floor is set at Room creation.** When a Room is instantiated, its
  Floor (q_0, Z_0) is fixed. The Room then "starts off with one Wall" — the
  initial Γ stack holding Z_0.
- **Reset returns to the Floor.** The Reset Operation (the monoid identity)
  returns the Room to its Floor: state → q_0, stack → Z_0. (This is why Reset
  is the identity element — it restores the initial condition.)
- **One Floor per Room.** A Room has exactly one Floor. (Multiple Rooms can
  have different Floors; a Portal moves between Rooms, not between Floors.)

*This definition is derived from the CRASM 7-tuple (q_0 + Z_0) and the
"starts off with one wall" line. It matches the earlier "Candidate C" (the
stack base, a concept not a fixture) but is more precise: Floor = (q_0, Z_0).
**If the Captain's memory of Floor is different, correct this section — it is
the one definition I derived rather than was given.***

### Door (DOR)

A **Door** is the **Σ interface** — the boundary where B-Sequences (operation
strings) cross the Room's boundary.

- **Installed in a Wall.** A Door is carved into a Wall's contiguous block
  (`RoomContents.md`: Walls are "used to install Doors"). The Door occupies a
  word-aligned region in the Wall.
- **A Door is a MAP type** (`RoomContents.md`). It maps a Door-identifier to
  the Wall region that holds its Slots.
- **Contains Slots.** Each Door has one or more **Slots** (ring-buffer
  Sockets) — the actual BIn/BOut ring buffers that carry the messages. The
  Door is the named entrance; the Slot is the ring buffer inside it.
- **Two varieties:** **Front Door** (a *known* entrance) and **Backdoor** (an
  *unknown* entrance).
- **Leads to another Room or Agent** (`ChineseRoomObjects.md`).

#### How a Door is opened in a Wall via BIn/BOut

> **[NEEDS CAPTAIN DEFINITION — narrowed by the CRASM frame.]** The Captain:
> "the Chinese Room starts off with one wall, to which you must open up a door
> using a BIn/BOut." Read against the CRASM, "opening a Door" is **allocating
> the Σ interface** — installing a BIn (input side, where operation strings
> enter) and/or BOut (output side, where result strings leave) Slot at a
> word-aligned location in the Wall's Γ block. The most likely reading
> (**Candidate 1**): *the Door IS the BIn/BOut pair* installed in the Wall;
> the Room reads incoming B-Sequences from the BIn Slot and writes outgoing
> B-Sequences to the BOut Slot. The alternative (**Candidate 2**): the Door is
> a MAP that *references* the Wall region, and BIn/BOut are how you read/write
> through it. **Confirm 1 or 2 (or describe a third) so the open-door
> operation is stated concretely.**

### Portal (PTL)

A **Portal** is a **B-Sequence that transitions the Room to another
Room/Agent** — a δ transition that changes Q (`ChineseRoomObjects.md`: "A
B-Sequence portal to another Chinese Room"). Where a Door is the *boundary*
(Σ interface), a Portal is the *transition* (δ to another Q). Portals are how
Rooms compose into a system of Rooms (the "Internet of Rooms").

### Slot / Ring-Buffer Socket (SLT)

A **Slot** is a **Ring Buffer Socket** (`Slots.md`). This is the "network
socket door, all contiguous, but the way Crabs work we can just create that
contiguous block as a ring buffer" — the socket is *not* a hardware primitive;
it is a Wall region shaped as a ring buffer.

- **Two types:** **B-Input (BIn)** and **B-Output (BOut)**.
- **Memory layout:** see `Protocol/Slots.md` (authoritative) for the `Slot`,
  `BIn`, `BOut`, and `Window` C structs and layouts.
- **Window:** a BIn + BOut combined in contiguous memory (duplex Slot), for
  Interprocess IO Portals / Mirrors.
- **Alignment:** each Slot is 64-bit aligned at both ends (`Slots.md`).
- **The empty `Data/MapTypes/Socket.md` is populated** with the
  Socket-as-abstract-primitive contract (see that file).

## ASCII List (the Room's content organization)

The **"ASCII List"** in "ASCII List Chinese Room" refers to the Room's
contents being organized as an **ASCII List** data type (a List is one of the
Map Types, `Data/MapTypes/List.md`). The Room's four stacks (Operation,
Operand, Header, State) and its Contents (Tables, Books, Dictionaries, Maps,
**Lists**, Arrays, Stacks, Files, Boxes) are the data structures the CRASM
operates on. A Room whose primary content organization is a List is an
*ASCII List* Chinese Room.

> *If "ASCII List Chinese Room" means something more specific (a particular
> List-based structure that IS the Room, rather than "a Room whose contents
> are a List"), the Captain should confirm so this is precise.*

## The hard rule: abstract, not hardware

This is the boundary that makes "Chinese Room **Abstract** Stack Machine"
literal (the Captain: "it's a hard rule, when I say Chinese Room Abstract
Stack Machine, it literally means it's abstract").

**The core/microframework provides all the abstract stuff:**
- The Room / Wall / Floor / Door / Portal / Slot roles (the CRASM).
- The four stacks (Operation/Operand/Header/State).
- The monoid engine (operation strings, expression evaluation as morphism,
  Reset as identity).
- The BIn/BOut door mechanism (ring-buffer sockets).
- The ring-buffer-as-socket (a Socket is contiguous memory shaped as a ring).
- The API for an **in-RAM filesystem** (abstract file storage in memory).
- The Window / Mirror (duplex Slot, Interprocess IO Portals).
- The Interrupt Operations (DC1-DC4).
- The string-in-string-out RPC engine (Corollary 1).
- The Uniprinter Unicode API (symbol rendering).
- The JSX engine (next — string manipulation, a direct application of the
  monoid).

**The core CANNOT contain anything hardware-dependent:**
- No network drivers, no disk I/O, no serial port, no GPU, no OS syscalls, no
  `#include <sys/...>`, no `#include <winsock.h>`, no `read()`/`write()` on
  file descriptors, no `mmap`, no `ioctl`.
- The BIn/BOut *interface* (the `Slot`/`BIn`/`BOut` structs + ring-buffer
  semantics) is abstract. *Backends* that bind those interfaces to real
  hardware (a TCP socket, a serial port, a file, a pipe) are
  **hardware-dependent** and live in **CrabsTK**.

**Hardware goes to CrabsTK.** "Someone else might be using that on a
radically different language and OS" (the Captain). CrabsTK provides the
concrete backends that plug into the core's abstract BIn/BOut Slot interface.
The core defines *what* a Slot is and *how* the ring buffer works;
CrabsTK defines *where* the bytes come from and go to.

**The one sanctioned exception:** BIn/BOut implementations in a *Hardware
Development Language* "may not strictly comply with the contiguous memory
operation but shall operate functionally equivalent to the specified register
stack machine implementation" (`Slots.md`). This is the *hardware layer's*
freedom to implement the interface on non-contiguous hardware registers; it
does not permit the *abstract core* to depend on hardware. The core stays
all-contiguous; the hardware layer may adapt.

## Interprocess pipes

The Captain: "I never got up to the point of actually using the Interprocess
pipes." Interprocess pipes are **not yet specified or implemented.** They are
intended to be a Door/Slot variety (a Slot whose backend is an OS pipe), but
per the hard rule, the *pipe backend* is OS-dependent and belongs in
CrabsTK. The *abstract* part — a Slot that can be backed by an
interprocess pipe — is the core's concern; the concrete pipe I/O is not.

> *Placeholder. Fill (or remove) once the Captain decides whether interprocess
> pipes are in scope for the core's abstract API or purely a CrabsTK
> backend.*

## What this spec unblocks

Once the remaining **[NEEDS CAPTAIN DEFINITION]** items are resolved, this
spec is the source of truth for:
1. Implementing the Room/Wall/Floor/Door/Portal/Slot as C++ types (the
   "ASCII List Chinese Room" the fleet waits on) — built on the CRASM 7-tuple.
2. The **JSX engine** (next) — string manipulation over the monoid of
   operation strings, using the ASCII Data Types + Uniprinter.
3. The **Crabs Machine RPC engine** — string-in-string-out, the base other
   APIs/languages build on.
4. The FVM's real BIn/BOut/Hash/BigInt components — "implement the core's
   abstract Slot; let CrabsTK provide the backend."

## Open items (for the Captain)

1. **Floor definition** — I derived Floor = (q_0, Z_0) from the CRASM 7-tuple.
   **Confirm or correct** (your memory is fuzzy on this one).
2. **Door mechanism** — Candidate 1 (Door IS the BIn/BOut pair) vs. Candidate
   2 (Door is a MAP referencing the Wall region). **Pick one.**
3. **ASCII List scope** — "a Room whose contents are a List," or a specific
   List-based structure that IS the Room?
4. **Interprocess pipes** — core abstract API, or purely a CrabsTK
   backend?
5. **Class codes** — confirm `Floor = FLR` for the `ChineseRoomObjects.md`
   table (already added, marked PENDING).
