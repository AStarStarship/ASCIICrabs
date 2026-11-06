// Copyright AStarship <https://astarship.net>.
// TPassword seam test: FIPS SHA-256 vectors + salted password lifecycle.
// Uses the real ASCIICrabs config chain + Random (IUDRandom for salt).

// _Seam setup: " _Config.h " resolves to the ASCIICrabs root config, which now
// defines the CRABS_* seam table (root _Seams.h, restored) and defaults
// _Seam to _CrabsRelease (23) via _ConfigFooter.h. 23 >= CRABSTK_WHO_CORE (19)
// so the Who headers' gate bodies compile. Include order: _Seams.h (Who seam
// numbers) first, so CRABSTK_WHO_CORE is defined before the gate is evaluated.

#if _Seam >= _CrabsPassword
#if _Seam == _CrabsPassword
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

inline const CHA* Password(const CHA* args) {
#if _Seam == _CrabsPassword
  // The release seam is the final sown test pass: it runs the complete
  // tree of every lower seam's checks in one pass.
  // TPassword seam test: FIPS SHA-256 vectors + salted password lifecycle.
  static int failures = 0;
#define CHECK(label, cond) do { \
  BOL ok = (cond); \
  printf("%-44s %s\n", label, ok ? "PASS" : "FAIL"); \
  if (!ok) ++failures; \
} while (0)

// --- FIPS 180-2 vectors (TPasswordSha256 one-shot path) ---
{
  failures = 0;
  // --- sha256("abc") full vector ---
  {
    IUA digest[32];
    IUA prefix[3] = {'a', 'b', 'c'};
    TPasswordSha256(prefix, 3, digest);
    char hex[70] = {0};
    CHA* out = hex;
    for (ISN i = 0; i < 32; ++i) TPasswordHex2(digest[i], &out);
    CHECK("sha256(\"abc\") full vector",
        strcmp(hex, "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD") == 0);
  }
  // --- sha256("") full vector ---
  {
    IUA digest[32];
    IUA prefix[3] = {'a', 'b', 'c'}; // not used for empty
    TPasswordSha256((const IUA*)"", 0, digest);
    char hex[70] = {0};
    CHA* out = hex;
    for (ISN i = 0; i < 32; ++i) TPasswordHex2(digest[i], &out);
    CHECK("sha256(\"\") full vector",
        strcmp(hex, "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855") == 0);
  }
}
// --- TPassword lifecycle (nil authenticator: no validation rules) ---
{
  failures = 0;
  // --- SetKey succeeds, GetKey starts with PBK2$ ---
  {
    TPassword pw(_NilP, "s3cret-pa55");
    CHECK("SetKey succeeds", pw.IsSalted() == true);
    CHECK("GetKey starts with PBK2$", strncmp(pw.GetKey(), "PBK2$", 5) == 0);
    CHECK("stored length is 5+32+1+64", TPasswordLen(pw.GetKey()) == 102);
    CHECK("Verify correct password", pw.Equals("s3cret-pa55") == true);
    CHECK("Reject wrong password", pw.Equals("wrong-password") == false);
    CHECK("Reject empty password", pw.Equals("") == false);
    CHECK("Reject nil password", pw.Equals(_NilP) == false);
  }
  // --- Two passwords of the same value get DIFFERENT salts ---
  {
    TPassword pw2(_NilP, "s3cret-pa55");
    CHECK("same password, different salt",
        strcmp(pw.GetKey(), pw2.GetKey()) != 0);
    CHECK("both verify against their own salt",
        pw.Equals("s3cret-pa55") && pw2.Equals("s3cret-pa55"));
  }
  // --- Salt accessor round-trip ---
  {
    IUA salt_a[PasswordSaltBytes], salt_b[PasswordSaltBytes];
    CHECK("GetSalt(pw)", pw.GetSalt(salt_a) == true);
    CHECK("GetSalt(pw2)", pw2.GetSalt(salt_b) == true);
    CHECK("salts differ", memcmp(salt_a, salt_b, PasswordSaltBytes) != 0);
  }
  // --- Deterministic derivation: same salt + password => same key ---
  {
    IUA key1[PasswordKeyBytes], key2[PasswordKeyBytes];
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "s3cret-pa55", key1);
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "s3cret-pa55", key2);
    CHECK("deterministic derivation", memcmp(key1, key2, PasswordKeyBytes) == 0);
  }
  // --- Different salt => different key (same password) ---
  {
    IUA key3[PasswordKeyBytes];
    TPasswordDeriveKey(salt_b, PasswordSaltBytes, "s3cret-pa55", key3);
    CHECK("different salt changes key", memcmp(key1, key3, PasswordKeyBytes) != 0);
  }
  // --- Different password => different key (same salt) ---
  {
    IUA key4[PasswordKeyBytes];
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "other-password", key4);
    CHECK("different password changes key",
        memcmp(key1, key4, PasswordKeyBytes) == 0 ? false : true);
  }
}
// --- Serialize / Parse round-trip ---
{
  failures = 0;
  // --- Serialize / Parse round-trip ---
  {
    IUA salt[PasswordSaltBytes], key[PasswordKeyBytes];
    for (ISN i = 0; i < PasswordSaltBytes; ++i) salt[i] = (IUA)(i * 7);
    for (ISN i = 0; i < PasswordKeyBytes; ++i) key[i] = (IUA)(0xAB - i);
    CHA stored[PasswordStoredLengthMin] = {0};
    TPasswordSerialize(salt, key, stored);
    IUA salt2[PasswordSaltBytes], key2[PasswordKeyBytes];
    CHECK("Parse round-trip", TPasswordParse(stored, salt2, key2) == true);
    CHECK("salt round-trip", memcmp(salt, salt2, PasswordSaltBytes) == 0);
    CHECK("key round-trip", memcmp(key, key2, PasswordKeyBytes) == 0);
    CHECK("reject malformed (no PBK2$)", TPasswordParse("plaintext", salt2, key2) == false);
    CHECK("reject malformed (short)", TPasswordParse("PBK2$ABCD", salt2, key2) == false);
    CHECK("reject malformed (bad hex)", TPasswordParse("PBK2$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ$AB", salt2, key2) == false);
  }
}
// --- Constant-time comparison ---
{
  failures = 0;
  // --- Constant-time comparison ---
  {
    IUA a[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    IUA b[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    IUA c[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17};
    CHECK("CT equals (match)", TPasswordConstantTimeEquals(a, b, 16) == true);
    CHECK("CT equals (mismatch last byte)",
        TPasswordConstantTimeEquals(a, c, 16) == false);
  }
}
printf("\n%s (%d failures)\n", failures ? "FAIL" : "ALL PASS", failures);
return failures ? 1 : 0;
#endif
  return _NilP;
}

// --- Type definitions: use ASCIICrabs canonical types from _Config.h ---
// CHA, ISA, IUA, ISB, IUB, ISC, IUC, IUD, ISN, BOL are all defined by
// the ASCIICrabs config chain (_ConfigHeader.h / _ConfigFooter.h).

// --- SHA-256 K[64] round constants ---

static const IUC SHA256_K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4d70dd1d, 0x5cb9a195, 0x6b901122,
  0x7763e373, 0x81c2c92e, 0x92722c85, 0xa2bfe8a8,
  0xa83b4066, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3,
  0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
  0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x65000f17,
  0x766a0abb, 0x83ffd41b, 0x905e1116, 0xa4506ceb,
  0xbef9a3f7, 0xc67178f2, 0xd18edf6c, 0xd8a1efd8,
  0xe7d3fbc8, 0x21e1cde6, 0x43260e7c, 0x53380d13,
  0x6ca67092, 0x1102e15e
};

// --- SHA-256 schedule: W[t] for t=0..15 from message block ---

// --- SHA-256 hash state ---

// --- Initialize hash state (H0..H7) ---

// --- SHA-256 transform one 512-bit block ---

// --- SHA-256 helper functions ---

// Ch(x,y,z) = (x AND y) OR ((NOT x) AND z)
// #define Ch(e,f,g)  ((e & f) | ((~e) & g))

// Maj(x,y,z) = (x AND y) OR (x AND z) OR (y AND z)
// #define Maj(a,b,c)  ((a & b) | (a & c) | (b & c))

// --- Rotate right ---

// --- End SHA-256 implementation ---

}  //< namespace CRTest
