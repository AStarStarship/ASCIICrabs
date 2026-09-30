# Socket

A **Socket** is a block of memory used to transceive data, typically
implemented as a **ring buffer**. A Socket is the *abstract* primitive: it is
contiguous memory shaped as a ring, **not** a hardware device. Binding a
Socket to a real endpoint (TCP, serial, file, pipe) is **hardware/OS-dependent**
and belongs in **CrabsTK**, not the core (see
`Crabs/ASCIIListChineseRoom.md`, "The hard rule: abstract, not hardware").

> This file was previously empty (3 blank lines). It is populated here as part
> of the ASCII List Chinese Room spec (2026-09-12). The Slot / BIn / BOut /
> Window memory layouts below are carried over from `Protocol/Slots.md`, which
> remains the authoritative source for the C data structures; this file states
> the *Socket-as-abstract-primitive* contract and how a Socket becomes a Door.

## Socket as a Ring Buffer

- A Socket is **one contiguous block of word-aligned memory** (LE-first, even
  powers of 2; the all-contiguous invariant). It is allocated like a Wall
  (an Autoject via the RAMFactory, `Crabs/Autojects.md`).
- The ring buffer has four bounds: `begin` (start of the buffer), `start`
  (current data start), `stop` (current data stop), `end` (end of the buffer).
- Each Socket "shall begin [at a] 64-bit aligned memory address and shall have
  a 64-bit aligned end address" (`Protocol/Slots.md`).
- A Socket can use **dynamic memory** (grown via the RAMFactory stack-to-heap
  auto-grow path), but it always remains **one contiguous block** — it never
  fragments.

## Two types of Socket

A **Slot** is a Ring Buffer Socket. There are two Slot types:

- **B-Input (BIn)** — a Slot for *incoming* Messages; uses offsets from the
  beginning of the data structure in memory.
- **B-Output (BOut)** — a Slot for *outgoing* data; reserves N bytes
  (0/8/16/24) at the end of the ring buffer for writing integers and
  floating-point numbers to string (overflow handling).

The `Slot`, `BIn`, `BOut`, and `Window` C data structures and memory layouts
are specified in `Protocol/Slots.md` (authoritative). This file does not
restate them.

## A Socket as a Door

A **Door** in a Chinese Room is a MAP type (`Crabs/RoomContents.md`) installed
in a **Wall** (`Crabs/ASCIIListChineseRoom.md`). The Door's actual message
carrying is done by one or more **Sockets (Slots)** carved into the Wall's
contiguous block. "The network socket door, which is all contiguous, but the
way Crabs work we can just create that contiguous block as a ring buffer" —
the socket door is *not* a special hardware primitive; it is a Wall region
shaped as a ring buffer.

> **How exactly a Door is "opened" in a Wall via BIn/BOut** is **[NEEDS
> CAPTAIN DEFINITION]** — see `Crabs/ASCIIListChineseRoom.md`, "How a Door is
> opened in a Wall via BIn/BOut" (Candidate 1/2/3). Once confirmed, this
> section will state the open-door operation concretely.

## Hardware equivalence (the one sanctioned exception)

BIn/BOut implementations written in a Hardware Development Language "may not
strictly comply with the contiguous memory operation but shall operate
functionally equivalent to the specified register stack machine implementation"
(`Protocol/Slots.md`). This freedom belongs to the **hardware layer**
(CrabsTK), not the abstract core. The core's Socket stays all-contiguous;
the hardware layer may adapt the interface to non-contiguous hardware
registers.

## Default slots

A Script implementation may provide a default BIn Slot named `In` and a
default BOut Slot named `Out` (`Protocol/Slots.md`). `In` streams bytes from a
Text keyboard, BOut Slot, or Text display input; `Out` streams bytes to an
Abstract text display, BIn Slot, or Abstract serial output. (The concrete
keyboard/serial backends are CrabsTK; the abstract `In`/`Out` slots are
core.)
