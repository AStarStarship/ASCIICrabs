# ASCII Data Spec — Ideas

Working notes for improving the ASCII Data Type specification. These are
proposals, not decisions. The Captain has final say on all type ordering and
bit allocation.

## 1. Unused PC Remap Bits

The PC (Plain Context) remap word is a 64-bit integer that stores 12 five-bit
values (one per PC type, _PCa through _PCl), packed contiguously:
_PCa in b0:b4, _PCb in b5:b9, ..., _PCl in b55:b59.

That uses 60 bits. **4 bits go unused (b63:b60).**

Each 5-bit remap value encodes a POD type 0-31, but values 20-31 are PC
types, not valid remap targets. **Values 20-31 (12 per remap slot) are
unused as remap targets** — that's 12 slots × 12 unused values = 144 unused
5-bit codes across the whole PC word.

**Captain's resolution (Types.md):** "PC Types are disabled by remapping it
to a NIL type." Remapping a PC type to NIL (0) disables it.

**Captain's proposal (APPROVED, moved to Types.md): 12 global
arbitrary-width POD types.** Since values 20-31 are invalid as PC remap
targets (they *are* the PC types), they are repurposed as 12 global POD
types of arbitrary bit width — not restricted to 8, 16, 32, 64, or 128 bits.
The 12 types can be swapped out per-context by remapping them. If a global
type is remapped to a valid POD type 0-19, it takes on that width for the
current context. If remapped to an invalid value, the Crabs Machine throws
an error. See Types.md "Global Arbitrary-Width POD Types" section for the
full table and context-swapping rules.

### Remaining: 4 spare bits (b63:b60)

These 4 bits are still unassigned. Candidates:

| Bits | Mode | Description |
|------|------|-------------|
| 0000 | Default | PC types remap to POD types 0-19 (current behavior). |
| 0001 | Extended | PC types can remap to Extended Standard Types. |
| 0010 | Packed | PC types remap to packed sub-word types (bitfields). |
| 0011 | Vector | PC types remap to vector/homotuple types. |
| 0100 | Custom | PC types remap to user-defined types (via ACTX handler). |
| 0101-1111 | Reserved | Future expansion. |

This gives the Crabs Machine a way to switch the *class* of type that PC types
map to, without reconfiguring the entire 60-bit remap word. The mode bits are
set once per context (per Door, per Room, per expression) and read on every
PC type lookup.

## 2. MOD Bit Simplification (Captain's Proposal)

Current: MOD is 2 bits (b14:b13). MOD 0 = unsigned void char, MOD 1 = signed
void int, MOD 2 = Data Types Block 1 (empty), MOD 3 = Data Types Block 2
(empty).

Proposal: Use 1 bit (b14) for "is void integer type" and 1 bit (b13) for
"is signed." This gives:

| b14 | b13 | Meaning |
|-----|-----|---------|
| 0   | 0   | Normal type (not void). b13 is part of MT. |
| 1   | 0   | Unsigned void integer. Value in b12:b0. |
| 1   | 1   | Signed void integer. Value in b12:b0 (two's complement). |
| 0   | 1   | Available — could extend MT to 5 bits (b13:b9). |

This frees the old MOD 2/3 blocks for other uses and makes the void type
encoding self-documenting. The tradeoff: MT loses 1 bit (becomes 3 bits,
b12:b10) when b14=0 and b13=1, or gains 1 bit (becomes 5 bits, b13:b9)
when b14=0 and b13=0. The Captain should decide if the variable-width MT
is acceptable or if MT should stay fixed at 4 bits (b12:b9) and b13 should
be reserved when b14=0.

## 3. SPC/SPD Type Tables (32-bit and 64-bit Patterns)

The 4-byte (DTC) and 8-byte (DTD) ASCII Data Type patterns have SPC (Super32)
and SPD (Super64) bit fields, but the type tables for what SPC/SPD codes map
to are not defined in the spec. The bit layout is:

- 4-byte: `b31=CNS, b30:b29=MD, b28:b13=SPC, b12:b9=MT, b8:b7=SW, b6:b5=VT, b4:b0=POD`
- 8-byte: `b63=CNS, b62:b61=MD, b60:b13=SPD, b12:b9=MT, b8:b7=SW, b6:b5=VT, b4:b0=POD`

SPC is 16 bits, SPD is 48 bits. These need type tables defining what the
Super types are. Candidates:

- 128-bit float (FPE) and 128-bit int (IUE/ISE) — already in the 16-bit
  pattern as POD 16-19, but the SPC/SPD fields could provide *extended
  precision* variants or *vector* variants of the 16-byte types.
- Large-object types: 256-byte, 512-byte, 1K, 2K contiguous blocks
  (currently in Extended Block EBI1K/EBI2K but could be promoted to
  SPC/SPD for direct addressing).
- Promise types (PMS, RES, VAL, ERR) — currently Plain Context Types,
  could be promoted to SPC/SPD for direct 32/64-bit addressing.

## 4. Room and Floor AType Assignments

The spec's Types.md says "Room and Floor are not correct or missing from the
ASCII Data Spec." The office metaphor (Room, Floor, Door, Stack, List, Map,
Book, Dic) needs AType assignments. Proposal:

| Type | AType | Description |
|------|-------|-------------|
| RMA | _PCa remap | Room — contiguous memory block. Remap to IUD (64-bit offset+size). |
| RMB | _PCb remap | Floor — sub-region of a Room. Remap to IUC (32-bit offset+size). |
| DRA | _PCc remap | Door — named entry point. Remap to IUC (Door number + Slot count). |
| SCA | _PCd remap | Stack — undo-able allocation spine. Remap to IUD (checkpoint offset). |

These use PC types because Room/Floor/Door/Stack are context-dependent:
their width depends on the Room size (8-bit for tiny Rooms, 64-bit for
gigabyte Rooms). The PC remap word sets the width per-context.

## 5. B-Sequence Type Codes for String Encoding

The BIn states include `BInStatePackedUTF8`, `BInStatePackedUTF16`,
`BInStatePackedUTF32`. These should have corresponding B-Sequence type codes
so that a B-Sequence can declare its string encoding explicitly. Currently
the encoding is implicit in the string type (STA/STB/STC). Proposal: add
B-Sequence header flags or use the SW bits to encode the encoding:

- SW=1 (SWB) + POD=CHA → UTF-8
- SW=2 (SWC) + POD=CHB → UTF-16
- SW=3 (SWD) + POD=CHC → UTF-32

This is already implicit but should be documented in BSequences.md.

## 6. Multi-Code-Point String Flag

The Captain's design: a flag in the TString/TRope header indicating whether
the string contains any multi-code-point characters (surrogate pairs in
UTF-16, code points > U+FFFF in UTF-32). This allows fast paths for
pure-BMP strings.

Proposal: Add a 1-bit flag to the TRope header. The TRope already has a
`datum` field (ISZ, 64 bits) that the Captain is unsure about keeping.
If the datum is kept, use bit 0 of datum as the multi-code-point flag.
If the datum is removed, add an explicit `IUA flags` field:

```
struct TRope {
  IUA flags;   // bit 0: has_multi_code_point, bits 1-7: reserved
  ISZ charc;   // Unicode character count
  ISZ total;   // size in bytes including header
  ISZ count;   // count of elements in buffer
};
```

This adds 1 byte to the header (or reuses the datum). The flag is set at
string creation time by scanning for surrogates (UTF-16) or code points
> U+FFFF (UTF-32). Once set, it is never cleared (strings are immutable
in the Room).

## 7. Token Type for LLM Training

The Captain mentioned training an LLM and making all ASCII Types tokens.
Proposal: Define a `TOK` (Token) type as an Extended Standard Type that
maps a string to a token ID (ISC or IUC). The token table is a TMap or
TDic in the Room. The B-Sequence type code for TOK would be in the
Extended Standard block (EB 3), after the existing STA/STB/STC entries.

This is relevant to the JSX engine (Phase 5) and the IMUL/Script2
languages. The token type would be:

```
TOK = ATypeESBase + 20  // after STA, STB, STC, and other ES types
```

The token value is an ISC (4-byte) token ID that indexes into the token
table. The token table maps token ID → string offset in the Room.

## 8. Assembly Line Boundary Type

For cross-process (DLL/SO → exe/bin) data passing, the Captain noted that
a RAMFactory function pointer may need to be passed so the receiving side
can free the memory. Proposal: Define an `ALB` (Assembly Line Boundary)
type that wraps a pointer + size + RAMFactory:

```
struct ALB {
  void* data;         // Pointer to the data
  IUD size;           // Size in bytes
  void* (*free_fn)(void*);  // RAMFactory free function
  DTB type;           // ASCII Data Type of the data
};
```

This is not an AType itself but a transport wrapper. The AType of the
*contents* is carried in the `type` field. The ALB is passed by value
across the assembly line boundary (it's a fixed-size struct, 32 or 40
bytes depending on pointer width).

## 9. CHE → CHD Rename (Done)

CHE (formerly "largest char") has been renamed to CHD (Char Default) across
all of ASCIICrabs. The rename was a simple case-sensitive whole-word
replacement. 164 occurrences in ASCIICrabs, 10 in AGENT_PLAN.md. Build
verified.

The character type family is now:
- CHA — 1-byte character (UTF-8 code unit or ASCII)
- CHB — 2-byte character (UTF-16 code unit)
- CHC — 4-byte character (UTF-32 code point)
- CHN — platform-native character (wchar_t)
- CHS — string character type (compile-time config, = CHR)
- CHR — current string character type (compile-time selected)
- CHD — default character type (formerly CHE, for printing)
- CHT — Chat Template character type (scan cursor / delimiter type)
- CHL — largest character type for the platform
