// Copyright AStarship <https://astarship.net>.
// Password — SHA-256 password hashing and key derivation.
// Provides TPasswordSha256, TPasswordDeriveKey, TPasswordHex2, TPasswordLen,
// TPasswordConstantTimeEquals for the ASCIICrabs crypto subsystem.
#pragma once
#ifndef _CrabsPasswordH
#define _CrabsPasswordH
#include <_Config.h>

// --- Fixed-width types ---
typedef unsigned char IUA;   // 1-byte unsigned integer
typedef signed char   ISA;   // 1-byte signed integer
typedef ISN ISN;             // signed size type (from _Config.h)
typedef BOL BOL;             // boolean (from _Config.h)

// --- SHA-256 round constants ---

// K[64] — round constants (hex)
extern const IUA TPasswordSha256_K[64];

// --- SHA-256: compute hash of buffer ---

// TPasswordSha256: compute SHA-256 of prefix bytes, result in digest[32]
//  prefix: input bytes
//  len:    number of input bytes
//  digest: output buffer (32 bytes), caller-owned
void TPasswordSha256(const IUA* prefix, ISN len, IUA* digest);

// --- SHA-256: convert 32-byte digest to hex string ---

// TPasswordHex2: convert one 32-bit word to 2-char hex at out[2]
//  di:     32-bit digest word
//  out:    output buffer (must have at least 3 bytes: 2 chars + null)
void TPasswordHex2(IUA di, CHA* out);

// --- SHA-256: derive key from salt + password ---

// TPasswordDeriveKey: derive key from salt and password string
//  salt:      salt bytes (PasswordSaltBytes)
//  saltlen:   salt length (PasswordSaltBytes)
//  password:  password string (_NilP-terminated)
//  key:       output key buffer (PasswordKeyBytes)
void TPasswordDeriveKey(const IUA* salt, ISN saltlen, const CHA* password, IUA* key);

// --- SHA-256: constant-time comparison ---

// TPasswordConstantTimeEquals: constant-time compare of two key bundles
//  a:       first key bundle (PasswordKeyBytes)
//  b:       second key bundle (PasswordKeyBytes)
//  len:     length in bytes (PasswordKeyBytes)
//  returns: true if equal, false otherwise
BOL TPasswordConstantTimeEquals(const IUA* a, const IUA* b, ISN len);

// --- SHA-256: password length computation ---

// TPasswordLen: compute stored password length
//  pw:      password key string (PBK2$ prefix expected)
//  returns: stored length (typically 102 = 5 + 32 + 1 + 64 for PBK2$ + salt + $ + key)
ISN TPasswordLen(const CHA* pw);

// --- SHA-256: FIPS 180-2 test vectors (compile-time assert) ---

// sha256("abc") expected: BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD
// sha256("")   expected: E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855

#endif  //< _CrabsPasswordH
