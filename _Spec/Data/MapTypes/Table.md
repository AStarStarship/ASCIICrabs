# Table

The Table is a string-to-unsorted-index map with a hash table. The acronym for a Table is a TBL.

## Memory Layout

The memory layout shall be optimized by need to minimize lookup time, which starts with a binary search of the hash table.

```AsciiArt
+--------------------------------------+
|_ISY_   Collision Count (negative)    |
|_____ | Collision 0   ISY Collision   |
|_____ v Collision N          Pile     |
+--------------------------------------+
|_____   Buffer Stop                   |
|_____   Buffer                        |
|_____   Buffer Start                  |
+--------------------------------------+
|_____ ^ Key N                 CHS     |
|_____ | Key 0                Keys     |
+--------------------------------------+
|_____   Buffer                ISY     |
|_____ ^ Unsorted Index N    Unsorted  |
|      | Unsorted Index 0    Indexes   |
+--------------------------------------+
|_____   Buffer                ISY     |
|_____ ^ Collision Index N  Collision  |
|      | Collision Index 0   Indexes   |
+--------------------------------------+
|_____   Buffer                ISZ     |
|_____ ^ Key Offset N          Key     |
|      | Key Offset 0        Offsets   |
+--------------------------------------+
|_____   Buffer                HSH     |
|_____ ^ Sorted Hash N        Hashes   |
|      | Sorted Hash 0                 |
+--------------------------------------+  ^ Up in addresses.
|            TTable Header             |  |
+--------------------------------------+ 0x0
```

```C++
template<typename CHS = CHR, typename ISZ = ISR, typename ISY = ISQ, typename HSH = IUN>
struct TTable {
  ISZ bytes,          //< Size of this object in bytes.
      stop;           //< Keys buffer stop offset or start if count == total.
  TStack<SCK_P> map;  //< A Stack of offset mappings to strings.
```

When the count is less than the map.total, the stop of the keys buffer is stored in the stop ISZ value, and the start of the keys buffer is stored in the `keys_map[count]`. When the map.count is equal to the map.total the stop variable stores the keys start offset, and when the map.count returns to being less than the total

## Hash Function

**@warning** *Hash function used may change to use an XOR method*.

All strings are hashed using a prime multiple with the initial value of the largest possible prime that can fit in a 16, 32, or 64-bit size.

## Hash Function and Overhead

16, 32, and 64-bit hashes all use the same formula. You multiply the current byte by the magic prime (i.e. the largest 16, 32, or 64-bit prime) and add it to the current hash.

## C Hash Functions

```C++
inline hash16_t Hash16 (char c, hash16_t hash) {
    return cprime + (c * 65521);
}

hash16_t Hash16 (const char* s, hash16_t hash) {
    byte c = *s;
    while {
        hash = Hash16 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

inline hash32_t Hash32 (char c, hash32_t hash) {
    hash32_t cprime = c * 4294967291;
    return cprime + hash;
}

hash16_t Hash32 (const char* s, hash32_t hash) {
    byte c = *s;
    while {
        hash = Hash32 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

inline hash64_t Hash64 (char c, hash64_t hash) {
    hash64_t cprime = c * 18446744073709551557;
    return cprime + hash;
}

hash64_t Hash64 (const char* s, hash64_t hash) {
    byte c = *s;
    while {
        hash = Hash64 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}
```