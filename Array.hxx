// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_ARRAY_HXX
#define CRABS_ARRAY_HXX
#include "Array.hpp"
#if SEAM >= CRABS_UNIPRINTER
#include "Binary.hpp"
namespace _ {

/* @ingroup Array
*/

/* Creates a CPU word from the repeated fill_char. */
inline IUW FillWord(CHA fill_char) {
  IUW value = (IUW)(IUA)fill_char;
#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
#if CPU_SIZE == CPU_8_BYTE
  return value | (value << 8) | (value << 16) | (value << 24) | (value << 32) |
    (value << 48) | (value << 56);
#elif CPU_SIZE == CPU_4_BYTE
  return value | (value << 8) | (value << 16) | (value << 24);
#else
  return value | (value << 8);
#endif
#else
#error Your CPU is in poopy mode. Change to little endian mode.
#endif
}

CHA* ArrayFill(void* origin, ISW count, CHA fill_char) {
  if (IsError(origin) || count < 0)
    return NILP;
  CHA* start = TPtr<CHA>(origin);

  CHA* stop = start + count;

  IUW fill_word = FillWord(fill_char);

  // 1.) Align start pointer up to a word boundry and fill the unaligned bytes.
  CHA* success = stop,
     * aligned_pointer = TPtrUp<>(start);
  while (start < aligned_pointer && start < stop) *start++ = fill_char;
  // 2.) Align stop pointer down to a word boundry and copy the midde words.
  IUW* words = TPtr<IUW>(start),
    * words_end = TPtrDown<IUW>(stop);
  while (words < words_end) *words++ = fill_word;
  // 3.) Copy remaining unaligned bytes.
  start = TPtr<CHA>(words_end);
  while (start < stop) *start++ = fill_char;

  return success;
}

/* Overwrites the memory with fill_char; functionally equivalent to memset. */
inline CHA* ArrayFill(void* start, void* stop, CHA fill_char = 0) {
  return ArrayFill(start, TDelta<>(start, stop) + 1, fill_char);
}

template<typename ISZ = ISW>
inline CHA* TObjectFill(void* start, void* stop, CHA fill_char = 0) {
  return ArrayFill(TPtr<>(start, sizeof(ISZ)), stop, fill_char);
}

template<typename ISZ = ISW>
inline CHA* TObjectFill(void* origin, ISW bytes, CHA fill_char = 0) {
  return ArrayFill(TPtr<>(origin, sizeof(ISZ)), bytes - sizeof(ISZ), fill_char);
}

template<typename ISZ = ISW>
inline CHA* TObjectFill(void* origin, CHA fill_char = 0) {
  return TObjectFill<ISZ>(origin, *TPtr<ISZ>(origin), fill_char);
}

/* Compares the two blocks of memory byte by byte to check if they're identical.
@return a_bytes if the two blocks of memory are idential, 0 if either the
pointers are null or the size in bytes are not the same, and any negative result
is the index + 1 of the first byte that was not the same. */
ISW ArrayCompareSlow(const void* a, const ISW a_bytes, const void* b,
  const ISW b_bytes) {
  if (a_bytes != b_bytes || a_bytes <= 0) return 0;
  const CHA* a_cursor = TPtr<CHA>(a),
    * a_end = a_cursor + a_bytes,
    * b_cursor = TPtr<CHA>(b);
  while (a_cursor < a_end)
    if (*a_cursor++ != *b_cursor++) {
      return -TDelta<>(a, a_cursor);
    }
  return a_bytes;
}

/* Compares the two blocks of memory in words see if they're identical
@return a_bytes if the two blocks of memory are idential, 0 if either the
pointers are null or the size in bytes are not the same, and any negative result
is the index + 1 of the first byte that was not the same. */
ISW ArrayCompareFast(const void* a, const ISW a_bytes, const void* b,
  const ISW b_bytes) {
  if (a_bytes != b_bytes || a_bytes <= 0) return 0;
  const IUA* a_cursor = TPtr<const IUA>(a),
    * a_end = a_cursor + a_bytes,
    * b_cursor = TPtr<const IUA>(b),
    * b_end = b_cursor + b_bytes;
  // 18-byte Examples:
  // v-- 0x0   v--------------------------- a_offset_msb
  // xABCDEFG_HIJKLMNP_QR                <- a_offset = 1
  // xxxxABCD_EFGHIJKL_MNPQR             <- b_offset = 4
  //               v----------------------- a_offset_msb
  // xxxABCDE_FGHIJKLM_NPQR              <- a_offset = 4
  // xABCDEFG_HIJKLMNP_QR                <- b_offset = 1
  // 26-byte Example:              v------- a_end
  // xABCDEFG_HIJKLMNP_OQRSTUVW_XYZ   v---- b_end
  // xxxxABCD_EFGHIJKL_MNPOQRST_UVWXYZ
  // Step 1: Compare the words...
  const IUW* a_begin_word = TPtrDown<IUW>(a_cursor),
    * b_begin_word = TPtrDown<IUW>(b_cursor);
  const IUA* a_end_word = TPtrDown<const IUA>(a_end);
  IUW        a_offset = TDelta(a_begin_word, a_cursor) * 8,
    b_offset = TDelta(b_begin_word, b_cursor) * 8,
    a_offset_msb = IUW(ACPUBytes) * 8 - a_offset,
    b_offset_msb = IUW(ACPUBytes) * 8 - b_offset;
  a_cursor = (IUA*)a_begin_word;
  b_cursor = (IUA*)b_begin_word;
  IUW a_lsb = *TPtr<IUW>(a_cursor),
    b_lsb = *TPtr<IUW>(b_cursor);
  while (a_cursor < a_end_word - ACPUBytes) {
    a_cursor += ACPUBytes;
    b_cursor += ACPUBytes;
    const IUW a_msb = *(IUW*)a_cursor,
      b_msb = *(IUW*)b_cursor,
      a_lsb_shifted = a_lsb >> a_offset,
      a_msb_shifted = a_offset == 0 ? 0 : a_msb << a_offset_msb,
      b_lsb_shifted = b_lsb >> b_offset,
      b_msb_shifted = b_offset == 0 ? 0 : b_msb << b_offset_msb,
      a_word = a_lsb_shifted | a_msb_shifted,
      b_word = b_lsb_shifted | b_msb_shifted;
    if (a_word != b_word) {
      /*
      D_COUT("\n\nStep 1 failed!" <<
             "\na_offset       : " << a_offset <<
             "\nb_offset    : "    << b_offset <<
             "\na_offset_msb   : " << a_offset_msb <<
             "  b_offset_msb: " << b_offset_msb <<
             "\na_word         : " << Hexf(a_word) <<
             "\nb_word         : " << Hexf(b_word) <<
             "\na_lsb          : " << Hexf(a_lsb) <<
             "\na_lsb_shifted  : " << Hexf(a_lsb_shifted) <<
             "\na_msb          : " << Hexf(a_msb) <<
             "\na_msb_shifted  : " << Hexf(a_msb_shifted) <<
             "\nb_lsb          : " << Hexf(b_lsb) <<
             "\nb_lsb_shifted  : " << Hexf(b_lsb_shifted) <<
             "\nb_msb          : " << Hexf(b_msb) <<
             "\nb_msb_shifted: " << Hexf(b_msb_shifted) <<
             "\nTDelta<>(a, a_cursor    ): " << TDelta<>(a, a_cursor    ) <<
             "\nTDelta<>(a_cursor, a_end): " << TDelta<>(a_cursor, a_end) <<
             "\nTDelta<>(b, b_cursor    ): " << TDelta<>(b, b_cursor    ) <<
             "\nTDelta<>(b_cursor, b_end): " << TDelta<>(b_cursor, b_end));*/
      IUW index = 0;
      while (((a_word << index) & 0xff) == ((b_word << index) & 0xff)) 
        ++index;
      return -TDelta<>(a, a_cursor - ACPUBytes + index);
    }
    a_lsb = a_msb;
    b_lsb = b_msb;
  }
  if (a_cursor == a_end) 
    return a_bytes;
  a_cursor += ACPUBytes;
  b_cursor += ACPUBytes;
  // Step 2: Compare the MSB.
  const IUW a_msb = *(IUW*)a_cursor,
    b_msb = *(IUW*)b_cursor,
    a_lsb_shifted = a_lsb >> a_offset,
    a_msb_shifted = a_offset == 0 ? 0 : a_msb << a_offset_msb,
    b_lsb_shifted = b_lsb >> b_offset,
    b_msb_shifted = b_offset == 0 ? 0 : b_msb << b_offset_msb,
    word_mask = ~IUW(0) >> (ACPUBytes - TDelta(a_cursor, a_end)),
    a_word = (a_lsb_shifted | a_msb_shifted) & word_mask,
    b_word = (b_lsb_shifted | b_msb_shifted) & word_mask;
  if (a_word != b_word) {
    /* D_COUT(
        "\n\nStep 2 failed!"
        <<
           "\na_offset     : "  << a_offset <<
           "  a_offset_msb : "  << a_offset_msb <<
           "\nb_offset     : "  << b_offset <<
           "  b_offset_msb : "  << b_offset_msb <<
           "\nword_mask    : "  << Hexf(word_mask) <<
           "\na_lsb        : "  << Hexf(a_lsb) <<
           "  b_lsb        : "  << Hexf(b_lsb) <<
           "\na_msb        : "  << Hexf(a_msb) <<
           "  b_msb        : "  << Hexf(b_msb) <<
           "\na_lsb_shifted: "  << Hexf(a_lsb_shifted) <<
           "  b_lsb_shifted: "  << Hexf(b_lsb_shifted) <<
           "\na_msb_shifted: "  << Hexf(a_msb_shifted) <<
           "  b_msb_shifted: "  << Hexf(b_msb_shifted) <<
           "\na_word       : "  << Hexf(a_word) <<
           "  b_word       : "  << Hexf(b_word) <<
           "\na_cursor[-1] : " << Hexf(*(a_cursor - 1)) <<
           "  b_cursor[-1] : " << Hexf(*(b_cursor - 1)) <<
           "\na_cursor[ 0] : " << Hexf(*(a_cursor    )) <<
           "  b_cursor[ 0] : " << Hexf(*(b_cursor    )) <<
           "\na_cursor[+1] : " << Hexf(*(a_cursor + 1)) <<
           "  b_cursor[+1] : " << Hexf(*(b_cursor + 1)) <<
           "\nTDelta<>(a, a_cursor    ): " << TDelta<>(a, a_cursor    ) <<
           "\nTDelta<>(a_cursor, a_end): " << TDelta<>(a_cursor, a_end) <<
           "\nTDelta<>(b, b_cursor    ): " << TDelta<>(b, b_cursor    ) <<
           "\nTDelta<>(b_cursor, b_end): " << TDelta<>(b_cursor, b_end));*/
    return -TDelta<>(a, a_cursor);
  }
  return a_bytes;
}

ISW ArrayCompare(const void* a, ISW a_bytes, const void* b,
  const ISW b_bytes) {
  return ArrayCompareFast(a, a_bytes, b, b_bytes);
}

ISW ArrayCopySlow(void* write, ISW w_size, const void* read, const ISW r_size) {
  if (IsError(write) || IsError(read) || r_size <= 0 || w_size < r_size)
    return 0;
  CHA* w_cursor = TPtr<CHA>(write);
  const CHA* r_cursor = TPtr<const CHA>(read),
    * r_end = r_cursor + r_size;
  while (r_cursor < r_end) *w_cursor++ = *r_cursor++;
  return r_size;
}

/* Copies the block of memory in word-sized chunks. */
ISW ArrayCopyFast(void* write, ISW w_size, const void* read,
  const ISW r_size) {
  if (r_size < ACPUBytes)
    return ArrayCopySlow(write, w_size, read, r_size);
  if (IsError(write) || IsError(read) || r_size <= 0 || w_size < r_size)
    return 0;
  // Algorithm:
  // 64-bit Memory Layout that grows 3 bytes: 
  // b=byte bbbbbbbb_=64-bit word x=illegal memory address ?=legal memory byte
  //       0xN   v---cursor
  // Write: xxxxxbbb_bbbbbbbb_bbbbbbbb_bbbbbbbb_bbb?????
  //       0xN+M     begin_word        end_word
  //  Read: xxxxxxxb_bbbbbbbb_bbbbbbbb_bbbbbbbb_bb??????
  //               ^--start                stop--^
  // 
  // Step 1: Copy to the unaligned lower write bytes.
  // We start out with a socket full of memory taht we're not allowed to access
  // any of the x memory bytes else the compiler will yell at us.
  const IUA* r_start = TPtr<IUA>(read),
    * r_stop = r_start + r_size;
  IUA* w_cursor = TPtr<IUA>(write);
  IUW* w_start_word = TPtrUp<IUW>(w_cursor);

  while (w_cursor < TPtr<IUA>(w_start_word)) *w_cursor++ = *r_start++;

  const IUW* r_start_word = TPtrDown<const IUW>(r_start);
  IUW w_offset = TDelta(w_cursor, w_start_word),
    r_offset = TDelta(r_start_word, r_start);

  // Only whole words strictly inside the write region may be written as
  // words; the leftover bytes are copied one by one in step 3. (Writing a
  // word that overhangs the region would clobber up to 7 legal bytes past
  // it.)
  const ISW words_count = (r_size - w_offset) / ACPUBytes;
  IUW* w_stop_word = w_start_word + words_count;

  if (r_offset == 0) {
    //D_COUT("\nr_offset == 0  w_stop_word - w_start_word: " << 
    //       w_stop_word - w_start_word);
    while (w_start_word < w_stop_word)
      *w_start_word++ = *r_start_word++;
  }
  else {

    // Step 2: Copy the Word aligned data.
    // Write: bbbbbbbb_bbbbbbbb_bbbbbbbb_bbb?????
    //     -->  <-- offset_lower=2
    //  Read: ??bbbbbb_bbbbbbbb_bbbbbbbb_bb??????
    //       -->      <-- offset_upper=6
    /*
    D_COUT("\nStep 1 wrote "  << TDelta(write, w_cursor) << " bytes." <<
           "\nr_offset    : " << r_offset <<
           "\nw_start_word: " << Hexf(w_start_word) <<
           "  w_stop_word: "  << Hexf(w_stop_word) <<
           "  delta: "        << TDelta(w_start_word, w_stop_word));*/

    IUW       r_word_lsb = *r_start_word++;
    const IUW r_offset_lsb = r_offset * 8,
      r_offset_msb = (ACPUBytes - r_offset) * 8;

    while (w_start_word < w_stop_word) {
      const IUW r_word_msb = *r_start_word++;
      IUW r_lsb = r_word_lsb >> r_offset_lsb,
        r_msb = r_word_msb << r_offset_msb,
        word = r_lsb | r_msb;
      //D_COUT("\nr_lsb: " << Hexf(r_lsb) << "  r_msb: " << Hexf(r_msb));
      *w_start_word++ = word;
      r_word_lsb = r_word_msb;
    }
  }
  // Step 3: Copy the unaligned upper bytes.
  //  Write: bbbbbbbb_bbbbbbbb_bbbbbbbb
  // Bytes consumed so far: step 1's w_offset plus the whole words written
  // above; the first unread byte is exactly that many past r_start.
  r_start = r_start + ISW(words_count) * ACPUBytes;
  w_cursor = TPtr<IUA>(w_start_word);
  //D_COUT("\nr_start: " << Hexf(r_start) << "  r_stop:" << Hexf(r_stop));
  while (r_start < r_stop) {
    //D_COUT("!");
    *w_cursor++ = *r_start++;
  }
  return r_size;
}

inline ISW ArrayCopy(void* write, ISW write_size, const void* source,
  ISW source_size) {
  //@todo Why is this not ArrayCopyFast?
  return ArrayCopySlow(write, write_size, source, source_size);
}

inline ISW ArrayCopy(void* write, void* write_end, const void* source,
  ISW source_size) {
  return ArrayCopy(write, TDelta<ISW>(write, write_end), source, source_size);
}

inline ISW ArrayCopy(void* write, ISW write_size, const void* read,
  const void* read_end) {
  return ArrayCopy(write, write_size, read, TDelta<ISW>(read, read_end));
}

inline ISW ArrayCopy(void* write, void* write_end, const void* read,
  const void* read_end) {
  return ArrayCopy(write, TDelta<ISW>(write, write_end), read,
    TDelta<ISW>(read, read_end));
}

inline ISW ArrayCopyFast(void* write, void* w_end, const void* read,
  const ISW r_size) {
  return ArrayCopyFast(write, TDelta(write, w_end), read, r_size);
}

inline ISW ArrayCopySlow(void* write, void* w_end, const void* read,
  const ISW r_size) {
  return ArrayCopySlow(write, TDelta(write, w_end), read, r_size);
}

ISW ArrayShiftUp(void* origin, void* end, ISW count) {
  if (IsError(origin) || origin <= end || count <= 0) return 0;
  CHA* start = TPtr<CHA>(origin),
    * stop = TPtr<CHA>(end);
  if (count < 3 * sizeof(void*)) {
    for (ISC i = 0; i < count; ++i) *(stop + count) = *stop;
    return 0;
  }
  IUW lsb_mask = sizeof(IUW) - 1;
  // Align the pointer down.
  IUW value = IUW(stop);
  CHA* pivot = TPtr<CHA>(value - (value & lsb_mask));

  // Shift up the top portion.
  for (ISC i = 0; i < count; ++i) *(stop + count) = *stop--;
  IUW* stop_word = TPtr<IUW>(stop);

  // Align the pointer up.
  value = CAlignUp(value, lsb_mask);
  IUW* start_word = TPtr<IUW>(value);

  // Shift up the moddle portion.
  while (stop_word >= start_word) *(stop_word + count) = *stop_word--;

  // Shift up the bottom portion.
  pivot = TPtr<CHA>(stop_word + 1);
  while (pivot >= start) *(pivot + count) = *pivot--;
  return count;
}

ISW ArrayShiftDown(void* origin, void* end, ISW count) {
  if (IsError(origin) || origin <= end || count <= 0) return 0;
  CHA* start = TPtr<CHA>(origin),
    * stop = TPtr<CHA>(end);
  if (count < 3 * sizeof(void*)) {
    for (ISC i = 0; i < count; ++i) *(stop + count) = *stop;
    return 0;
  }
  IUW least_significant_bits_mask = sizeof(IUW) - 1;
  // Align the pointer down.
  IUW value = IUW(stop);
  CHA* pivot = TPtr<CHA>(value - (value & least_significant_bits_mask));

  // Shift up the top portion.
  for (ISC i = 0; i < count; ++i) *(stop + count) = *stop--;
  IUW* stop_word = TPtr<IUW>(stop);

  // Align the pointer up.
  value = IUW(start);
  IUW* start_word = TPtr<IUW>(
    value + ((-(ISW)value) & least_significant_bits_mask));

  // Shift up the moddle portion.
  while (stop_word >= start_word) *(stop_word + count) = *stop_word--;

  // Shift up the bottom portion.
  pivot = TPtr<CHA>(stop_word + 1);
  while (pivot >= start) *(pivot + count) = *pivot--;
  return count;
}
//< @endingroup Array

} //< namespace _
#endif  // SEAM >= CRABS_UNIPRINTER
#endif  // CRABS_ARRAY_HXX
