# Autojects

An Autoject is the control record for an ASCII object's contiguous backing
storage and memory-management callback. The object format determines its
header, offsets, types, and values; an Autoject is not a universal serialized
header added to every value.

## RAMFactory contract

The current declaration in [Autoject.h](../../Autoject.h) is:

```C++
typedef IUW* (*RAMFactory)(IUW* origin, ISW bytes);

struct Autoject {
  IUW* origin;
  RAMFactory ram;
};
```

`origin` points to the object's backing block. `ram` is its memory-management
boundary. The callback takes two arguments; it has no `DTW` argument and no
specified operation for returning a data type encoded as a pointer.

## Fixed storage and optional growth

A fixed Wall can use caller-supplied static or program-stack memory with no
heap allocation. The caller keeps the storage alive for the Room, its installed
Doors and Slots, and any data references. The Floor is this initial fixed Wall,
not another storage abstraction.

Optional dynamic storage uses a RAMFactory with an explicit ownership policy.
Growth must preserve a contiguous object, validate the new capacity, and
account for references into the old block. The no-dynamic-memory configuration
must reject exhausted capacity rather than silently fall back to the heap.

## Current factory implementations

[Autoject.hxx](../../Autoject.hxx) supplies `ObjectFactoryStack` and
`ObjectFactoryHeap`. With a nil origin and positive byte count they allocate
word-aligned heap storage, rounding the allocation up to the CPU word size.
`ObjectFactoryHeap` deletes a non-error origin. The word **Stack** in a factory
name does not mean that allocation is heap-free, and caller-owned stack memory
must not be passed to the heap deleter.

[Autoject.hpp](../../Autoject.hpp) also provides templated object factories
and `Autobuffer`. The templated object factory writes an object-size header;
the standalone allocation functions do not. The template's nil-origin,
zero-size callback query returns its heap function for the current growth
mechanism. That implementation detail is not a uniform RAMFactory query
protocol, a data-type query, or a portable wire representation.

## Ownership and errors

- Pair every owned heap block with the factory that can release it. Do not
  delete caller-owned fixed storage through a heap factory.
- Check requested byte counts, word alignment, header size, and capacity before
  writing. Rounding allocation capacity up does not authorize writes beyond
  the object's declared size.
- Check results with the project's `IsError` convention, not just a null test.
- Keep the old object valid if growth fails. On successful relocation, update
  its owner and rebind or invalidate cached data, Door, and Slot addresses.
- Treat type-specific headers and counts as authoritative; do not substitute
  an invented shared size layout or claim all factory variants behave alike.

This page records the existing callback and the required ownership boundary,
not a claim that all growth paths have been implemented or verified. See
[ASCII List Chinese Room](ASCIIListChineseRoom.md) for integration status.
