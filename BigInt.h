// Copyright AStarship <https://astarship.net>.
#pragma once

/* Arbitrary-precision integer types: ISQ (signed) and IUQ (unsigned).
Modeled on JavaScript BigInt — no fixed upper/lower bound.
Internal layout:
  - Digit array stored in base 10^9 (1e9) so every digit fits in 30 bits.
  - IUQ stores magnitude only (always positive).
  - ISQ stores magnitude + a sign flag.
  - Digits stored least-significant-first (index 0 = ones digit).
  - Zero is represented as a single digit 0 with no leading zeros.
  - Memory managed via Autoject + RAMFactory (no naked new/delete,
    no raw pointer null checks — use IsError/PtrIsError). */

#ifndef CRABS_BIGINT_STANDALONE
#include <_ConfigHeader.h>
#include <_ConfigFooter.h>
#include <Autoject.h>
#endif

namespace _ {

/* IUQ — unsigned arbitrary-precision integer */
class IUQ {
public:
  // Digit base = 10^9.  Fits in 30 bits; two digits multiply to 60 bits
  // which still fits in IUD (64-bit) without overflow.
  static constexpr IUD Base() { return 1000000000u; }
  static constexpr IUB BaseDigits() { return 9; }  // log10(Base) == 9

  // Default-construct to 0 (heap-allocated, 1 digit).
  IUQ();

  // Construct from fixed-width types.
  IUQ(IUA value);
  IUQ(IUB value);
  IUQ(IUC value);
  IUQ(IUD value);
  IUQ(ISA value);
  IUQ(ISB value);
  IUQ(ISC value);
  IUQ(ISD value);

  // Construct from a null-terminated decimal string.
  // Leading whitespace is skipped.  Non-digit characters terminate parsing.
  explicit IUQ(const CHA* str);

  // Copy.
  IUQ(const IUQ& other);
  IUQ& operator=(const IUQ& other);
  ~IUQ();

  // True when the Autoject origin is an error code (allocation failed).
  BOL IsError() const;

  // -- Accessors --

  // True when value == 0.
  BOL IsZero() const;

  // True when value < 0.  (IUQ is always >= 0, so this is always false.)
  BOL IsNegative() const;

  // True when value > 0.
  BOL IsPositive() const;

  // Absolute value — returns *this for IUQ.
  IUQ Abs() const;

  // Compare: returns -1, 0, +1.
  ISB CompareTo(const IUQ& other) const;

  // Equality / inequality.
  BOL operator==(const IUQ& other) const;
  BOL operator!=(const IUQ& other) const;

  // -- Arithmetic --

  // Addition.
  IUQ operator+(const IUQ& other) const;
  IUQ& operator+=(const IUQ& other);

  // Subtraction: *this - other.  Result saturates at 0 for IUQ
  // (no underflow, matching JS BigInt clamp behaviour).
  IUQ operator-(const IUQ& other) const;
  IUQ& operator-=(const IUQ& other);

  // Multiplication.
  IUQ operator*(const IUQ& other) const;
  IUQ& operator*=(const IUQ& other);

  // Division: q = *this / other, r = *this % other.
  // Sets *this = q, returns r.  If other == 0, q = 0, r = *this (original).
  IUQ DivMod(const IUQ& other, IUQ& remainder);

  // Division (convenience).  Sets *this = *this / other.
  IUQ& operator/(const IUQ& other);

  // Modulo (convenience).  Returns *this % other, *this unchanged.
  IUQ operator%(const IUQ& other) const;

  // -- Conversion --

  // Convert to string (decimal, null-terminated).
  // Caller must provide a buffer of at least StrMaxChars(DigitCount()) bytes.
  // Returns pointer to start of string.
  CHA* ToStr(CHA* buf, ISW buf_size) const;

  // Estimate max string length for allocation.
  static ISW StrMaxChars(IUB digit_count);

  // Number of internal digits.
  ISW DigitCount() const;

  // Raw digit access (read-only, little-endian order).
  const IUD* Digits() const;
  IUD* DigitsMutable();

  // Internal storage size in digits.
  ISW Capacity() const;

private:
  Autoject aobj_;  //< RAMFactory-managed digit buffer.
  ISW  length_;    //< Number of digits in use (1 = zero).

  // Get the IUD digit array from the Autoject origin (after size header).
  IUD* DigitsRaw();
  const IUD* DigitsRaw() const;

  // Get the digit capacity from the Autoject allocation size.
  ISW DigitsCapacity();
  ISW DigitsCapacity() const;

  // Grow the Autoject to hold at least `needed` digits.
  void Grow(ISW needed);

  // Remove leading zeros (keep at least one digit).
  void Trim();

  // Internal multiply-accumulate: *this += multiplicand * multiplier.
  void MulAdd(const IUQ& multiplicand, IUD multiplier);

  // Helper: create a new IUQ with `count` digits from a digit array.
  static IUQ Make(IUD* digits, ISW count);
};

// -- Free functions for IUQ --

// Parse decimal string to IUQ.
IUQ IUQFromStr(const CHA* str);

// IUQ to decimal string (caller-owned buffer).
CHA* IUQToStr(const IUQ& n, CHA* buf, ISW buf_size);

// -- Relational operators --

BOL operator<(const IUQ& a, const IUQ& b);
BOL operator<=(const IUQ& a, const IUQ& b);
BOL operator>(const IUQ& a, const IUQ& b);
BOL operator>=(const IUQ& a, const IUQ& b);

// -- TypeOf for IUQ --
DTW TypeOf(const IUQ&);

/* ISQ — signed arbitrary-precision integer */
class ISQ {
public:
  ISQ();

  // Construct from fixed-width types.
  ISQ(IUA value);
  ISQ(IUB value);
  ISQ(IUC value);
  ISQ(IUD value);
  ISQ(ISA value);
  ISQ(ISB value);
  ISQ(ISC value);
  ISQ(ISD value);

  // Construct from a null-terminated decimal string (may have leading '-').
  explicit ISQ(const CHA* str);

  // Copy.
  ISQ(const ISQ& other);
  ISQ& operator=(const ISQ& other);
  ~ISQ();

  // True when the magnitude Autoject is an error code.
  BOL IsError() const;

  // -- Accessors --

  BOL IsZero() const;
  BOL IsNegative() const;
  BOL IsPositive() const;

  // Absolute value — returns ISQ with same magnitude, positive.
  ISQ Abs() const;

  // Compare: returns -1, 0, +1.
  ISB CompareTo(const ISQ& other) const;

  BOL operator==(const ISQ& other) const;
  BOL operator!=(const ISQ& other) const;

  // -- Arithmetic --

  ISQ operator+(const ISQ& other) const;
  ISQ& operator+=(const ISQ& other);

  ISQ operator-(const ISQ& other) const;
  ISQ& operator-=(const ISQ& other);

  ISQ operator*(const ISQ& other) const;
  ISQ& operator*=(const ISQ& other);

  // Division: q = *this / other (truncates toward zero), r = *this % other.
  // Sets *this = q, returns r.  If other == 0, q = 0, r = *this (original).
  ISQ DivMod(const ISQ& other, ISQ& remainder);

  // Division (convenience).  Sets *this = *this / other.
  ISQ& operator/(const ISQ& other);

  // Modulo (convenience).  Returns *this % other, *this unchanged.
  ISQ operator%(const ISQ& other) const;

  // -- Negation --
  ISQ operator-() const;

  // -- Conversion --

  CHA* ToStr(CHA* buf, ISW buf_size) const;
  static ISW StrMaxChars(IUB digit_count);

  ISW DigitCount() const;
  const IUD* Digits() const;
  IUD* DigitsMutable();
  ISW Capacity() const;

  // Access magnitude as IUQ.
  const IUQ& Magnitude() const;
  IUQ& MagnitudeMutable();

private:
  BOL  sign_;     // false = positive (or zero), true = negative
  IUQ  mag_;      // magnitude (always >= 0)
};

// -- Free functions for ISQ --

ISQ ISQFromStr(const CHA* str);
CHA* ISQToStr(const ISQ& n, CHA* buf, ISW buf_size);

// -- Relational operators --

BOL operator<(const ISQ& a, const ISQ& b);
BOL operator<=(const ISQ& a, const ISQ& b);
BOL operator>(const ISQ& a, const ISQ& b);
BOL operator>=(const ISQ& a, const ISQ& b);

// -- TypeOf for ISQ --
DTW TypeOf(const ISQ&);

}  // namespace _
