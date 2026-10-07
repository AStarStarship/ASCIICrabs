# Strings

Unicode strings can be created two different ways, using ASCII Strings and ASCII Ropes (String Rope). Strings can be created out of an ASCII Stack of Characters (i.e. CHA, CHB, or CHC POD Types), and this string type doesn't keep track of the number of Unicode characters. SRopes are created using the Extended Mappable Types, and they are used to keep track of the number of Unicode Characters and thus are only valid for UTF-8 and UTF-16 strings but not valid for UTF-32 strings because they are not encoded. SRopes are composed of an ISZ datum that can be used to store anything, typically a neural network value, followed by an ISZ chars (character count), followed by an ASCII String.

```C++
/* A ASCII string created from an ASCII Stack that doesn't keep track of the 
number of Unicode characters. */
template<SND_A>
struct TString {
  ISZ total,  //< Total elements in the boofer.
      count;  //< Count of characters in the boofer.
};

/* A UTF-8 or UTF-16 ASCII string created from an ASCII Stack that keeps track of the number of Unicode characters. */
template<STR_A>
struct TSRope {
  ISZ datum,  //< Memory alignment datum can be used for multiple purposes.
      chars,  //< Unicode Character Count.
      total,  //< Size of the string in bytes including the header.
      count;  //< Count of elements in the boofer.
};
```

```Crabs
STA utf8_example: "🥜Dez nutz🥜"
STB utf16_example: "🥜Dez nutz🥜"
STC utf32_example: "🥜Dez nutz🥜"

SRA utf8_example: "🥜Dez nutz🥜"
SRB utf16_example: "🥜Dez nutz🥜"
SRC utf32_example: "🥜You guessed it bois... dez nutz!🥜"
```

## Requirements

1. Unicode strings must be created using either ASCII Strings or ASCII Ropes (String Rope).
2. ASCII Strings created from ASCII Stacks of Characters (CHA, CHB, or CHC POD Types) must not track the number of Unicode characters.
3. SRopes created using Extended Mappable Types must track the number of Unicode Characters.
4. SRopes must be valid only for UTF-8 and UTF-16 strings, and must not be valid for UTF-32 strings.
5. SRopes must be composed of an ISZ datum, followed by an ISZ chars (character count), followed by an ASCII String.
6. The TSRope structure must maintain memory alignment for the datum field.
7. The TSRope structure must track the total size of the string in bytes including the header.
8. The TSRope structure must track the count of elements in the buffer.
