|# Map Types

ASCII Map Types shall be composed of contiguous memory and begin with an 16, 32 or 64-bit signed integer that specifies the object's size in bytes; this requirement is the need to reduce ROM size. There are 8 standard ASCII OBJ types:

|     Class       |      Type      |
|:---------------:|:--------------:|
|    Object       | User Definable |
|     Array       |      Array     |
|     Stack       |      Stack     |
|     Matrix      |      Matrix    |
|   B-Sequence    |     Metadata   |
|    B-Stream     |       Set      |
|      List       |       Set      |
|      Loom       |       Set      |
|      Book       |       Set      |
|   Dictionary    |       Set      |
|      Map        |       Set      |

***Caption:*** *Object Type Table*

## RPC Wire Format

When map types are transmitted over the RPC protocol, composite types use the **decomposed 3-byte wire format** defined in [RPCProtocol.md](../Protocol/RPCProtocol.md#52-map-type-decomposition). The following composite types are affected:

| Type  | EM IDs | Decomposed Format |
|-------|--------|-------------------|
| Book  | BO0-BO8 | `[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5]` |
| Dictionary | DI0-DI5 | `[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5]` |
| Table | TB0-TB3 | `[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5]` |
| List | LS0-LS2 | `[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5]` |

See [ExtendedTypes.md](../Data/ExtendedTypes.md#wire-format-note) for the full wire format table with hex values for all composite types.

## Why So Many Dictionary Types?

We are running in RAM, and a dictionary could contain millions of key-value
pairs. Adding extra bytes would added megabytes of data we don't need. Also,
on microcontrollers, especially 16-bit ones, will have very little RAM, so we
need a 16-bit object. It is easy to imagine a complex AI software using
more than 4GB RAM, or the need to attach a DVD ISO image as a key-value
pair, so we need a 64-bit dictionary.
