# Token Map

A Token Map (TKM) is a input text tokenizer.

## Memory Layout

The TKM uses the Table memory layout with some extra variables.

```C++
template<typename CHT = CHR, typename ISZ = ISR, typename ISY = ISQ, typename HSH = IUN>
struct TTable {
  ISZ bytes,          //< Size of this object in bytes.
      stop;           //< Keys buffer stop offset or start if count == total.
  TStack<SCK_P> map;  //< A Stack of offset mappings to strings.
```