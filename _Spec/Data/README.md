|# ASCII Data Specification

Automaton Standard Code for Information Interchange (ASCII) Data Types are mimicked after the the American Standard Code for Information Interchange C0 Control Codes using 3 all capitol letters. There are 32 POD Data Types, mimicking the number of ASCII C0 Control Codes. Arrays, Stacks, multi-dimensional matrices, and 2, 3, and 4-homogenous-tuple (homo-tuple) types can be created using the Vector types bit. All data types are word-aligned leaving some illegal Vector types such as an 1-byte Array of 2-byte types. These illegal types are used in order to create 2, 3, and 4-tuple homogeneous Vectors of a type, such as a 4-tuple of 1-byte unsigned integers IUA). These data types are then used to create concrete implementations of Abstract Data Types (ADTs).

The Automaton Standard Code for Information Interchange (ASCII) Data Specification provides:

* Convenient and easy-to-read-and-type 3-letter all-caps ASCII C0 code mimicry abbreviations that POP out around lower_snake_case.
* Clock generates 64-bit and 128-bit Subsecond Id for hot-to-archive UUIDs optimized for SQL, distribued systems, and AI agents.
* Data types can be represented as 8-bits/256-types for microcontrollers that are forward compatible with 16-bit/64K-types for more powerful processors.
* UTF-8, UTF-16, UTF-32, and MSb Varint encoded Strings.
* B-Sequence that describes the order and maximum sizes of a Byte-Sequence of ASCII Data.
* Map types:
  * Vector types:
    * Homo-tuples - Create tuples of 2, 3, or 4 of the same POD types.
    * Array - An array with the first word being the element count.
    * Stack - A stack of POD types stored as an array where the second word is the stack count.
    * Matrix - A homogeneous multi-dimensional array with a Stack of dimensions.
  * Loom - A homogeneous array of UTF-8, UTF-16, or UTF-32 strings using a Stack of signed integer offsets.
  * Map - A sparse array of one 1, 2, 4, or 8-byte data type to another data type.
  * Table - A Prime Multiple Hash (PMH) associative array that maps unique strings to contiguous integers.
  * Dictionary - A dictionary of heterogeneous types with a Table.
  * Book - A key-value dictionary of heterogeneous types with a Loom rather than a Table for fastest push-back performance and later conversion to a Table.

### RPC Wire Format

When map types are transmitted over the RPC protocol, composite types (BO0-BO8, DI0-DI5, TB0-TB3, LS0-LS2) use the **decomposed 3-byte wire format** defined in [RPCProtocol.md](../Protocol/RPCProtocol.md#52-map-type-decomposition). Each composite type is represented as `[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5]` = 22 bits packed into 3 bytes.

See also:
* [RPC Protocol Specification](../Protocol/RPCProtocol.md)
* [Extended Types](ExtendedTypes.md) — Wire Format Note for composite type decomposition
* [BSequences](BSequences.md) — BSQ wire format for RPC
