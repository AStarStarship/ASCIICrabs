// Copyright AStarship <https://astarship.net>.
//
// Arbitrary-precision integer implementation.
// Base = 10^9 (fits in 30 bits; two digits × 30 bits = 60 bits < 64-bit IUD).
// Digits stored least-significant-first.
//
// Memory: Autoject + RAMFactory (ObjectFactoryHeap).  No naked new/delete,
// no raw pointer null checks — use IsError() for error checking.
//
// Layout: Autoject origin points to [ISW size_header | IUD digits[0..N-1]].
//   - size_header: set by ObjectFactoryStack, do NOT modify.
//   - digits: start at origin + 1 word (after the size header).
//   - length_: member variable tracking how many digits are in use.

#include "BigInt.h"

namespace _ {

// ===========================================================================
// IUQ implementation
// ===========================================================================

// Helper: allocate an Autoject buffer for `count` digits.
// Returns the origin, or an error pointer on failure.
static inline IUW* AujAlloc(RAMFactory ram, ISW count) {
  IUW* origin = ram(NILP, 0);
  origin = ram(NILP, sizeof(ISW) + sizeof(IUD) * count);
  return origin;
}

IUQ::IUQ() : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 1);
  IUD* d = DigitsRaw();
  if (!IsError()) d[0] = 0;
}

IUQ::IUQ(IUA value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 1);
  IUD* d = DigitsRaw();
  if (!IsError()) d[0] = static_cast<IUD>(value);
}

IUQ::IUQ(IUB value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 1);
  IUD* d = DigitsRaw();
  if (!IsError()) d[0] = static_cast<IUD>(value);
}

IUQ::IUQ(IUC value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 2);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    d[0] = value % static_cast<IUC>(Base());
    IUC high = value / static_cast<IUC>(Base());
    if (high > 0) { d[1] = static_cast<IUD>(high); length_ = 2; }
  }
}

IUQ::IUQ(IUD value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 2);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    d[0] = value % Base();
    IUD high = value / Base();
    if (high > 0) { d[1] = high; length_ = 2; }
  }
}

IUQ::IUQ(ISA value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 1);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    IUA uval = (value < 0) ? static_cast<IUA>(-value) : static_cast<IUA>(value);
    d[0] = static_cast<IUD>(uval);
  }
}

IUQ::IUQ(ISB value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 1);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    IUB uval = (value < 0) ? static_cast<IUB>(-value) : static_cast<IUB>(value);
    d[0] = static_cast<IUD>(uval);
  }
}

IUQ::IUQ(ISC value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 2);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    IUC uval = (value < 0) ? static_cast<IUC>(-value) : static_cast<IUC>(value);
    d[0] = uval % static_cast<IUC>(Base());
    IUC high = uval / static_cast<IUC>(Base());
    if (high > 0) { d[1] = static_cast<IUD>(high); length_ = 2; }
  }
}

IUQ::IUQ(ISD value) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 2);
  IUD* d = DigitsRaw();
  if (!IsError()) {
    IUD uval = (value < 0) ? static_cast<IUD>(-value) : static_cast<IUD>(value);
    d[0] = uval % Base();
    IUD high = uval / Base();
    if (high > 0) { d[1] = high; length_ = 2; }
  }
}

IUQ::IUQ(const CHA* str) : length_(1) {
  aobj_.ram = ObjectFactoryHeap;
  aobj_.origin = AujAlloc(aobj_.ram, 4);
  if (IsError()) return;
  IUD* d = DigitsRaw();
  d[0] = 0;
  if (!str) return;
  const CHA* p = str;
  while (*p == ' ' || *p == '\t') ++p;
  if (*p == '\0') return;

  IUD base = Base();
  while (*p >= '0' && *p <= '9') {
    IUD digit = static_cast<IUD>(*p - '0');
    IUD carry = digit;
    for (ISW i = 0; i < length_; ++i) {
      IUD prod = d[i] * 10 + carry;
      d[i] = prod % base;
      carry = prod / base;
    }
    if (carry > 0) {
      Grow(length_ + 1);
      d = DigitsRaw();
      d[length_] = carry;
      ++length_;
    }
    ++p;
  }
  Trim();
}

IUQ::IUQ(const IUQ& other) : length_(other.length_) {
  aobj_.ram = other.aobj_.ram;
  aobj_.origin = AujAlloc(aobj_.ram, length_);
  if (IsError()) return;
  IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();
  for (ISW i = 0; i < length_; ++i) d[i] = od[i];
}

IUQ& IUQ::operator=(const IUQ& other) {
  if (this == &other) return *this;
  aobj_.ram = other.aobj_.ram;
  length_ = other.length_;
  aobj_.origin = AujAlloc(aobj_.ram, length_);
  if (IsError()) return *this;
  IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();
  for (ISW i = 0; i < length_; ++i) d[i] = od[i];
  return *this;
}

IUQ::~IUQ() {
  aobj_.origin = aobj_.ram(aobj_.origin, RAMFactoryDelete);
  aobj_.ram = NILP;
}

BOL IUQ::IsError() const {
  return ::_::IsError(static_cast<const void*>(aobj_.origin));
}

IUD* IUQ::DigitsRaw() {
  return TPtr<IUD>(aobj_.origin) + 1;
}

const IUD* IUQ::DigitsRaw() const {
  return TPtr<IUD>(aobj_.origin) + 1;
}

ISW IUQ::DigitsCapacity() {
  ISW bytes = *TPtr<ISW>(aobj_.origin);
  return (bytes - sizeof(ISW)) / sizeof(IUD);
}

ISW IUQ::DigitsCapacity() const {
  ISW bytes = *TPtr<ISW>(aobj_.origin);
  return (bytes - sizeof(ISW)) / sizeof(IUD);
}

BOL IUQ::IsZero() const {
  return (length_ == 1 && DigitsRaw()[0] == 0);
}

BOL IUQ::IsNegative() const { return false; }

BOL IUQ::IsPositive() const { return !IsZero(); }

IUQ IUQ::Abs() const { return *this; }

ISB IUQ::CompareTo(const IUQ& other) const {
  if (length_ != other.length_)
    return (length_ < other.length_) ? -1 : 1;
  const IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();
  for (ISW i = length_ - 1; i >= 0; --i) {
    if (d[i] != od[i]) return (d[i] < od[i]) ? -1 : 1;
  }
  return 0;
}

BOL IUQ::operator==(const IUQ& other) const { return CompareTo(other) == 0; }
BOL IUQ::operator!=(const IUQ& other) const { return CompareTo(other) != 0; }

void IUQ::Grow(ISW needed) {
  ISW cap = DigitsCapacity();
  if (needed <= cap) return;
  ISW new_cap = cap * 2;
  if (new_cap < needed) new_cap = needed;
  IUD* old = DigitsRaw();
  ISW old_len = length_;
  aobj_.origin = aobj_.ram(aobj_.origin, RAMFactoryDelete);
  aobj_.origin = AujAlloc(aobj_.ram, new_cap);
  if (IsError()) return;
  IUD* new_d = DigitsRaw();
  for (ISW i = 0; i < old_len; ++i) new_d[i] = old[i];
}

void IUQ::Trim() {
  IUD* d = DigitsRaw();
  while (length_ > 1 && d[length_ - 1] == 0) --length_;
}

void IUQ::MulAdd(const IUQ& multiplicand, IUD multiplier) {
  IUD base = Base();
  IUD carry = 0;
  ISW mlen = multiplicand.length_;
  const IUD* md = multiplicand.DigitsRaw();
  IUD* d = DigitsRaw();
  for (ISW i = 0; i < mlen || carry > 0; ++i) {
    IUD prod = (i < mlen ? md[i] : 0) * multiplier + carry;
    if (i >= length_) { Grow(i + 1); d = DigitsRaw(); d[i] = 0; ++length_; }
    IUD sum = d[i] + (prod % base);
    d[i] = sum % base;
    carry = prod / base + sum / base;
  }
  Trim();
}

IUQ IUQ::Make(IUD* digits, ISW count) {
  IUQ result;
  result.length_ = count;
  result.aobj_.origin = AujAlloc(result.aobj_.ram, count);
  if (result.IsError()) return result;
  IUD* d = result.DigitsRaw();
  for (ISW i = 0; i < count; ++i) d[i] = digits[i];
  result.Trim();
  return result;
}

IUQ IUQ::operator+(const IUQ& other) const {
  ISW max_len = (length_ > other.length_) ? length_ : other.length_;
  const IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();

  IUQ result;
  result.length_ = max_len + 1;
  result.aobj_.origin = AujAlloc(result.aobj_.ram, max_len + 1);
  if (result.IsError()) return result;
  IUD* rd = result.DigitsRaw();

  IUD base = Base();
  IUD carry = 0;
  for (ISW i = 0; i < max_len; ++i) {
    IUD a = (i < length_) ? d[i] : 0;
    IUD b = (i < other.length_) ? od[i] : 0;
    IUD sum = a + b + carry;
    rd[i] = sum % base;
    carry = sum / base;
  }
  if (carry > 0) rd[max_len] = carry;
  else --result.length_;
  result.Trim();
  return result;
}

IUQ& IUQ::operator+=(const IUQ& other) { *this = *this + other; return *this; }

IUQ IUQ::operator-(const IUQ& other) const {
  if (CompareTo(other) <= 0) return IUQ();
  const IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();

  IUQ result;
  result.length_ = length_;
  result.aobj_.origin = AujAlloc(result.aobj_.ram, length_);
  if (result.IsError()) return result;
  IUD* rd = result.DigitsRaw();

  IUD borrow = 0;
  for (ISW i = 0; i < length_; ++i) {
    IUD a = d[i];
    IUD b = (i < other.length_) ? od[i] : 0;
    IUD diff = a - b - borrow;
    borrow = (diff > a) ? 1 : 0;
    rd[i] = diff;
  }
  result.Trim();
  return result;
}

IUQ& IUQ::operator-=(const IUQ& other) { *this = *this - other; return *this; }

IUQ IUQ::operator*(const IUQ& other) const {
  if (IsZero() || other.IsZero()) return IUQ();
  const IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();
  ISW total_len = length_ + other.length_;

  IUQ result;
  result.length_ = total_len;
  result.aobj_.origin = AujAlloc(result.aobj_.ram, total_len);
  if (result.IsError()) return result;
  IUD* rd = result.DigitsRaw();
  for (ISW i = 0; i < total_len; ++i) rd[i] = 0;

  IUD base = Base();
  for (ISW i = 0; i < length_; ++i) {
    IUD carry = 0;
    for (ISW j = 0; j < other.length_ || carry > 0; ++j) {
      IUD prod = d[i] * (j < other.length_ ? od[j] : 0) + rd[i + j] + carry;
      rd[i + j] = prod % base;
      carry = prod / base;
    }
  }
  result.Trim();
  return result;
}

IUQ& IUQ::operator*=(const IUQ& other) { *this = *this * other; return *this; }

// Schoolbook long division: *this / other = quotient, remainder.
IUQ IUQ::DivMod(const IUQ& other, IUQ& remainder) {
  if (other.IsZero()) {
    remainder = *this;
    *this = IUQ();
    return *this;
  }
  if (CompareTo(other) < 0) {
    remainder = *this;
    *this = IUQ();
    return *this;
  }

  ISW n = length_;
  ISW m = other.length_;
  const IUD* d = DigitsRaw();
  const IUD* od = other.DigitsRaw();

  // Temporary working arrays (naked new for scratch, freed before return).
  IUD* w = new IUD[n + 2];
  for (ISW i = 0; i < n; ++i) w[i] = d[i];
  for (ISW i = n; i < n + 2; ++i) w[i] = 0;

  IUD* div = new IUD[m];
  for (ISW i = 0; i < m; ++i) div[i] = od[i];

  IUD base = Base();
  ISW q_len = n - m + 1;
  IUD* q = new IUD[q_len];
  for (ISW i = 0; i < q_len; ++i) q[i] = 0;

  for (ISW pos = q_len - 1; pos >= 0; --pos) {
    IUD w_hi = (pos + m < n + 1) ? w[pos + m] : 0;
    IUD w_mid = w[pos + m - 1];
    IUD qdig = 0;

    if (div[m - 1] > 0) {
      if (w_hi >= div[m - 1]) {
        qdig = base - 1;
      } else {
        IUD q_est = (w_hi * (base / div[m - 1])) +
                    ((w_hi * (base % div[m - 1]) + w_mid) / div[m - 1]);
        qdig = (q_est >= base) ? (base - 1) : q_est;
      }
    }
    if (qdig == 0) continue;

    IUD* prod = new IUD[m + 1];
    for (ISW i = 0; i <= m; ++i) prod[i] = 0;
    IUD carry = 0;
    for (ISW j = 0; j < m; ++j) {
      IUD p = div[j] * qdig + carry;
      prod[j] = p % base;
      carry = p / base;
    }
    prod[m] = carry;

    IUD borrow = 0;
    for (ISW j = 0; j <= m; ++j) {
      IUD sub = prod[j] + borrow;
      if (w[pos + j] >= sub) {
        w[pos + j] -= sub;
        borrow = 0;
      } else {
        w[pos + j] += base - (sub % base);
        borrow = sub / base + 1;
      }
    }

    if (borrow > 0) {
      --qdig;
      carry = 0;
      for (ISW j = 0; j < m; ++j) {
        IUD sum = w[pos + j] + div[j] + carry;
        w[pos + j] = sum % base;
        carry = sum / base;
      }
      if (carry > 0 && pos + m < n + 1) {
        w[pos + m] += carry;
        if (w[pos + m] >= base) {
          w[pos + m] -= base;
          if (pos + m + 1 < n + 1) w[pos + m + 1] += 1;
        }
      }
    }

    delete[] prod;
    q[pos] = qdig;
  }

  while (q_len > 1 && q[q_len - 1] == 0) --q_len;

  // Replace *this with quotient.
  aobj_.origin = aobj_.ram(aobj_.origin, RAMFactoryDelete);
  aobj_.origin = AujAlloc(aobj_.ram, q_len);
  length_ = q_len;
  if (!IsError()) {
    IUD* rd = DigitsRaw();
    for (ISW i = 0; i < q_len; ++i) rd[i] = q[i];
  }

  ISW r_len = m;
  while (r_len > 1 && w[r_len - 1] == 0) --r_len;

  remainder.aobj_.origin = remainder.aobj_.ram(remainder.aobj_.origin, RAMFactoryDelete);
  remainder.aobj_.origin = AujAlloc(remainder.aobj_.ram, r_len);
  remainder.length_ = r_len;
  if (!remainder.IsError()) {
    IUD* rd = remainder.DigitsRaw();
    for (ISW i = 0; i < r_len; ++i) rd[i] = w[i];
  }

  delete[] w;
  delete[] div;
  delete[] q;
  return *this;
}

IUQ& IUQ::operator/(const IUQ& other) {
  IUQ rem;
  DivMod(other, rem);
  return *this;
}

IUQ IUQ::operator%(const IUQ& other) const {
  IUQ copy = *this;
  IUQ rem;
  copy.DivMod(other, rem);
  return rem;
}

ISW IUQ::StrMaxChars(IUB digit_count) {
  return static_cast<ISW>(digit_count) * 10 + 2;
}

CHA* IUQ::ToStr(CHA* buf, ISW buf_size) const {
  if (!buf || buf_size <= 0) return buf;
  if (IsZero()) { buf[0] = '0'; buf[1] = '\0'; return buf; }

  ISW tlen = length_;
  IUD* tdigits = new IUD[tlen];
  const IUD* d = DigitsRaw();
  for (ISW i = 0; i < tlen; ++i) tdigits[i] = d[i];

  IUD base = Base();
  IUD ten = 10;
  ISW pos = 0;

  while (tlen > 0) {
    IUD rem = 0;
    for (ISW i = tlen - 1; i >= 0; --i) {
      IUD cur = rem * base + tdigits[i];
      tdigits[i] = cur / ten;
      rem = cur % ten;
    }
    if (pos < buf_size - 1) {
      buf[pos] = static_cast<CHA>('0' + rem);
    }
    ++pos;
    while (tlen > 0 && tdigits[tlen - 1] == 0) --tlen;
  }
  if (pos < buf_size) buf[pos] = '\0';

  for (ISW i = 0, j = pos - 1; i < j; ++i, --j) {
    CHA tmp = buf[i]; buf[i] = buf[j]; buf[j] = tmp;
  }

  delete[] tdigits;
  return buf;
}

ISW IUQ::DigitCount() const { return length_; }
const IUD* IUQ::Digits() const { return DigitsRaw(); }
IUD* IUQ::DigitsMutable() { return DigitsRaw(); }
ISW IUQ::Capacity() const { return DigitsCapacity(); }

// -- Free functions for IUQ --

IUQ IUQFromStr(const CHA* str) { return IUQ(str); }
CHA* IUQToStr(const IUQ& n, CHA* buf, ISW buf_size) {
  return n.ToStr(buf, buf_size);
}

BOL operator<(const IUQ& a, const IUQ& b) { return a.CompareTo(b) < 0; }
BOL operator<=(const IUQ& a, const IUQ& b) { return a.CompareTo(b) <= 0; }
BOL operator>(const IUQ& a, const IUQ& b) { return a.CompareTo(b) > 0; }
BOL operator>=(const IUQ& a, const IUQ& b) { return a.CompareTo(b) >= 0; }

// Type code for IUQ — placeholder, see __RefactorPlan.md.
DTW TypeOf(const IUQ&) { return 13; }

// ===========================================================================
// ISQ implementation
// ===========================================================================

ISQ::ISQ() : sign_(false), mag_() {}

ISQ::ISQ(IUA value) : sign_(false), mag_(static_cast<IUD>(value)) {}
ISQ::ISQ(IUB value) : sign_(false), mag_(static_cast<IUD>(value)) {}
ISQ::ISQ(IUC value) : sign_(false), mag_(static_cast<IUD>(value)) {}
ISQ::ISQ(IUD value) : sign_(false), mag_(value) {}

ISQ::ISQ(ISA value)
    : sign_(value < 0),
      mag_(static_cast<IUD>((value < 0) ? -value : value)) {}
ISQ::ISQ(ISB value)
    : sign_(value < 0),
      mag_(static_cast<IUD>((value < 0) ? -value : value)) {}
ISQ::ISQ(ISC value)
    : sign_(value < 0),
      mag_(static_cast<IUD>((value < 0) ? -value : value)) {}
ISQ::ISQ(ISD value)
    : sign_(value < 0),
      mag_(static_cast<IUD>((value < 0) ? -value : value)) {}

ISQ::ISQ(const CHA* str) : sign_(false), mag_() {
  if (!str) return;
  const CHA* p = str;
  while (*p == ' ' || *p == '\t') ++p;
  if (*p == '-') { sign_ = true; ++p; }
  else if (*p == '+') { ++p; }
  mag_ = IUQ(p);
  if (mag_.IsZero()) sign_ = false;
}

ISQ::ISQ(const ISQ& other) : sign_(other.sign_), mag_(other.mag_) {}

ISQ& ISQ::operator=(const ISQ& other) {
  if (this == &other) return *this;
  sign_ = other.sign_;
  mag_ = other.mag_;
  return *this;
}

ISQ::~ISQ() {}

BOL ISQ::IsError() const { return mag_.IsError(); }

BOL ISQ::IsZero() const { return mag_.IsZero(); }
BOL ISQ::IsNegative() const { return sign_ && !mag_.IsZero(); }
BOL ISQ::IsPositive() const { return !sign_ && !mag_.IsZero(); }

ISQ ISQ::Abs() const {
  ISQ result;
  result.sign_ = false;
  result.mag_ = mag_.Abs();
  return result;
}

ISB ISQ::CompareTo(const ISQ& other) const {
  if (sign_ != other.sign_) {
    if (sign_) return -1;
    return 1;
  }
  ISB cmp = mag_.CompareTo(other.mag_);
  return sign_ ? -cmp : cmp;
}

BOL ISQ::operator==(const ISQ& other) const { return CompareTo(other) == 0; }
BOL ISQ::operator!=(const ISQ& other) const { return CompareTo(other) != 0; }

ISQ ISQ::operator+(const ISQ& other) const {
  if (sign_ == other.sign_) {
    ISQ result;
    result.sign_ = sign_;
    result.mag_ = mag_ + other.mag_;
    if (result.mag_.IsZero()) result.sign_ = false;
    return result;
  }
  ISB cmp = mag_.CompareTo(other.mag_);
  if (cmp == 0) return ISQ();
  ISQ result;
  if (cmp > 0) { result.mag_ = mag_ - other.mag_; result.sign_ = sign_; }
  else { result.mag_ = other.mag_ - mag_; result.sign_ = other.sign_; }
  if (result.mag_.IsZero()) result.sign_ = false;
  return result;
}

ISQ& ISQ::operator+=(const ISQ& other) { *this = *this + other; return *this; }

ISQ ISQ::operator-(const ISQ& other) const {
  ISQ neg_other;
  neg_other.sign_ = !other.sign_;
  neg_other.mag_ = other.mag_;
  if (other.mag_.IsZero()) neg_other.sign_ = false;
  return *this + neg_other;
}

ISQ& ISQ::operator-=(const ISQ& other) { *this = *this - other; return *this; }

ISQ ISQ::operator*(const ISQ& other) const {
  ISQ result;
  result.mag_ = mag_ * other.mag_;
  result.sign_ = (mag_.IsZero() || other.mag_.IsZero()) ? false :
                 (sign_ != other.sign_);
  return result;
}

ISQ& ISQ::operator*=(const ISQ& other) { *this = *this * other; return *this; }

ISQ ISQ::DivMod(const ISQ& other, ISQ& remainder) {
  if (other.IsZero()) {
    remainder = *this;
    *this = ISQ();
    return *this;
  }
  IUQ mag_copy = mag_;
  IUQ rem_mag;
  IUQ q_mag = mag_copy.DivMod(other.mag_, rem_mag);

  ISQ quotient;
  quotient.mag_ = q_mag;
  quotient.sign_ = (sign_ != other.sign_) && !q_mag.IsZero();

  ISQ rem;
  rem.mag_ = rem_mag;
  // C++ convention: remainder has the sign of the dividend.
  rem.sign_ = sign_ && !rem_mag.IsZero();

  *this = quotient;
  remainder = rem;
  return *this;
}

ISQ& ISQ::operator/(const ISQ& other) {
  ISQ rem;
  DivMod(other, rem);
  return *this;
}

ISQ ISQ::operator%(const ISQ& other) const {
  ISQ copy = *this;
  ISQ rem;
  copy.DivMod(other, rem);
  return rem;
}

ISQ ISQ::operator-() const {
  ISQ result;
  result.sign_ = !sign_;
  result.mag_ = mag_;
  if (mag_.IsZero()) result.sign_ = false;
  return result;
}

ISW ISQ::StrMaxChars(IUB digit_count) {
  return static_cast<ISW>(digit_count) * 10 + 3;
}

CHA* ISQ::ToStr(CHA* buf, ISW buf_size) const {
  if (!buf || buf_size <= 0) return buf;
  if (IsZero()) { buf[0] = '0'; buf[1] = '\0'; return buf; }
  if (sign_) {
    buf[0] = '-';
    if (buf_size <= 1) return buf;
  }
  CHA* p = (sign_) ? buf + 1 : buf;
  ISW left = buf_size - (p - buf);
  if (left <= 0) return buf;
  mag_.ToStr(p, left);
  return buf;
}

ISW ISQ::DigitCount() const { return mag_.DigitCount(); }
const IUD* ISQ::Digits() const { return mag_.Digits(); }
IUD* ISQ::DigitsMutable() { return mag_.DigitsMutable(); }
ISW ISQ::Capacity() const { return mag_.Capacity(); }
const IUQ& ISQ::Magnitude() const { return mag_; }
IUQ& ISQ::MagnitudeMutable() { return mag_; }

// -- Free functions for ISQ --

ISQ ISQFromStr(const CHA* str) { return ISQ(str); }
CHA* ISQToStr(const ISQ& n, CHA* buf, ISW buf_size) {
  return n.ToStr(buf, buf_size);
}

BOL operator<(const ISQ& a, const ISQ& b) { return a.CompareTo(b) < 0; }
BOL operator<=(const ISQ& a, const ISQ& b) { return a.CompareTo(b) <= 0; }
BOL operator>(const ISQ& a, const ISQ& b) { return a.CompareTo(b) > 0; }
BOL operator>=(const ISQ& a, const ISQ& b) { return a.CompareTo(b) >= 0; }

// Type code for ISQ — placeholder, see __RefactorPlan.md.
DTW TypeOf(const ISQ&) { return 14; }

}  // namespace _
