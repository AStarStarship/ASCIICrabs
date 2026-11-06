// Copyright AStarship <https://astarship.net>.
//
// Homotuples.h — ASCIICrabs VHT (Vector of Homotuples) POD types.
//
// 19 ASCII POD types x 3 arities (2, 3, 4) = 57 homotuples.
// Per _Spec/Data/Types.md: VT=0, SW encodes element count, POD is the
// element type. The generic THomoT<T,N> template lives in Homotuples.hpp.
//
// No <cmath>, <vector>, <string>. Pure POD. Render is a separate layer.
#pragma once
#include <_ConfigHeader.h>

#ifndef _CrabsHomotuplesH
#define _CrabsHomotuplesH

namespace _ {

/* --- ISC2: 2-tuple of ISC (int32). VHT<2, ISC>. */
struct ISC2 {
  ISC x;
  ISC y;
  ISC2() : x(0), y(0) {}
  ISC2(ISC x_, ISC y_) : x(x_), y(y_) {}
  ISC2 operator+(const ISC2& o) const { return ISC2(x + o.x, y + o.y); }
  ISC2 operator-(const ISC2& o) const { return ISC2(x - o.x, y - o.y); }
  BOL operator==(const ISC2& o) const { return x == o.x && y == o.y; }
  BOL operator!=(const ISC2& o) const { return !(*this == o); }
};

/* --- FPC2: 2-tuple of FPC (float32). VHT<2, FPC>. The spec-conformant name
   for a 2D float point/vector. Replaces sf::Vector2f. */
struct FPC2 {
  FPC x;
  FPC y;
  FPC2() : x(0.f), y(0.f) {}
  FPC2(FPC x_, FPC y_) : x(x_), y(y_) {}
  FPC2 operator+(const FPC2& o) const { return FPC2(x + o.x, y + o.y); }
  FPC2 operator-(const FPC2& o) const { return FPC2(x - o.x, y - o.y); }
  FPC2 operator*(FPC s) const { return FPC2(x * s, y * s); }
  FPC2 operator/(FPC s) const { return FPC2(x / s, y / s); }
  FPC2& operator+=(const FPC2& o) { x += o.x; y += o.y; return *this; }
  FPC2& operator-=(const FPC2& o) { x -= o.x; y -= o.y; return *this; }
  FPC2& operator*=(FPC s) { x *= s; y *= s; return *this; }
  BOL operator==(const FPC2& o) const { return x == o.x && y == o.y; }
  BOL operator!=(const FPC2& o) const { return !(*this == o); }
  /* squared distance (no sqrt, no <cmath>); compare squared for nearest. */
  FPC Dist2(const FPC2& o) const {
    FPC dx = x - o.x;
    FPC dy = y - o.y;
    return dx * dx + dy * dy;
  }
};

/* --- ISC4: 4-tuple of ISC (int32). VHT<4, ISC>. Axis-aligned integer rect
   (texture sub-rect, grid bounds). Replaces sf::IntRect. Layout (left, top,
   w, h) — 1:1 SFML port. */
struct ISC4 {
  ISC x;  // left
  ISC y;  // top
  ISC w;  // width
  ISC h;  // height
  ISC4() : x(0), y(0), w(0), h(0) {}
  ISC4(ISC x_, ISC y_, ISC w_, ISC h_) : x(x_), y(y_), w(w_), h(h_) {}
  ISC Left() const { return x; }
  ISC Top() const { return y; }
  ISC Right() const { return x + w; }
  ISC Bottom() const { return y + h; }
  BOL Intersects(const ISC4& o) const {
    return x < o.Right() && Right() > o.x && y < o.Bottom() && Bottom() > o.y;
  }
  BOL Contains(const ISC2& p) const {
    return p.x >= x && p.x < Right() && p.y >= y && p.y < Bottom();
  }
};

/* --- FPC4: 4-tuple of FPC (float32). VHT<4, FPC>. Axis-aligned float rect.
   Replaces sf::FloatRect. Layout (left, top, w, h) — 1:1 SFML port. */
struct FPC4 {
  FPC x;  // left
  FPC y;  // top
  FPC w;  // width
  FPC h;  // height
  FPC4() : x(0.f), y(0.f), w(0.f), h(0.f) {}
  FPC4(FPC x_, FPC y_, FPC w_, FPC h_) : x(x_), y(y_), w(w_), h(h_) {}
  FPC Left() const { return x; }
  FPC Top() const { return y; }
  FPC Right() const { return x + w; }
  FPC Bottom() const { return y + h; }
  FPC2 Center() const { return FPC2(x + w * 0.5f, y + h * 0.5f); }
  /* AABbox intersection (SFML FloatRect::intersects parity). */
  BOL Intersects(const FPC4& o) const {
    return x < o.Right() && Right() > o.x && y < o.Bottom() && Bottom() > o.y;
  }
  BOL Contains(const FPC2& p) const {
    return p.x >= x && p.x < Right() && p.y >= y && p.y < Bottom();
  }
  /* Inflate in place by (dx, dy) on each edge. */
  FPC4& Inflate(FPC dx, FPC dy) {
    x -= dx;
    y -= dy;
    w += dx * 2.f;
    h += dy * 2.f;
    return *this;
  }
};

/* --- TVec2I/TVec2F/TIntRect/TFloatRect deprecated aliases (see below). --- */

/* --- TVideoMode: a headless video mode (width, height, bpp). VHT-ish
   (3-tuple of ISC, but with named fields for clarity). Replaces
   sf::VideoMode. The T-prefix follows Chimera+ for POD structs. */
struct TVideoMode {
  ISC width;
  ISC height;
  ISC bits_per_pixel;
  TVideoMode() : width(0), height(0), bits_per_pixel(0) {}
  TVideoMode(ISC w, ISC h, ISC bpp) : width(w), height(h), bits_per_pixel(bpp) {}
};

/* --- Deprecated aliases: the old TVec2I/TVec2F/TIntRect/TFloatRect names
   kept so existing TileWorld/CarWorld/BikeWorld code compiles unchanged.
   New code should use the spec-conformant ISC2/FPC2/ISC4/FPC4 names. */
using TVec2I = ISC2;
using TVec2F = FPC2;
using TIntRect = ISC4;
using TFloatRect = FPC4;

/* --- Full VHT homotuple set: 19 POD types x 3 arities (2, 3, 4). -------
   Per _Spec/Data/Types.md: VT=0, SW encodes element count, POD is the
   element type. The generic THomoT<T,N> is the storage form; the named
   types below are the common instantiations with vector math.
   Types: ISA IUA ISB IUB ISC IUC ISD IUD ISE IUE
          CHA CHB CHC CHS BOL FPC FPD FPE FPG
   Arity 2 (vector), 3 (point/normal/rgb), 4 (quaternion/rgba/rect). */

/* --- 8-bit integer --- */
struct ISA2 { ISA x, y;
  ISA2() : x(0), y(0) {} ISA2(ISA a, ISA b) : x(a), y(b) {}
  ISA2 operator+(const ISA2& o) const { return ISA2(x+o.x, y+o.y); }
  ISA2 operator-(const ISA2& o) const { return ISA2(x-o.x, y-o.y); } };
struct ISA3 { ISA x, y, z;
  ISA3() : x(0), y(0), z(0) {} ISA3(ISA a, ISA b, ISA c) : x(a), y(b), z(c) {}
  ISA3 operator+(const ISA3& o) const { return ISA3(x+o.x, y+o.y, z+o.z); }
  ISA3 operator-(const ISA3& o) const { return ISA3(x-o.x, y-o.y, z-o.z); } };
struct ISA4 { ISA x, y, z, w;
  ISA4() : x(0), y(0), z(0), w(0) {}
  ISA4(ISA a, ISA b, ISA c, ISA d) : x(a), y(b), z(c), w(d) {}
  ISA4 operator+(const ISA4& o) const { return ISA4(x+o.x, y+o.y, z+o.z, w+o.w); }
  ISA4 operator-(const ISA4& o) const { return ISA4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- 8-bit unsigned --- */
struct IUA2 { IUA x, y;
  IUA2() : x(0), y(0) {} IUA2(IUA a, IUA b) : x(a), y(b) {}
  IUA2 operator+(const IUA2& o) const { return IUA2((IUA)(x+o.x), (IUA)(y+o.y)); }
  IUA2 operator-(const IUA2& o) const { return IUA2((IUA)(x-o.x), (IUA)(y-o.y)); } };
struct IUA3 { IUA x, y, z;
  IUA3() : x(0), y(0), z(0) {} IUA3(IUA a, IUA b, IUA c) : x(a), y(b), z(c) {}
  IUA3 operator+(const IUA3& o) const { return IUA3((IUA)(x+o.x), (IUA)(y+o.y), (IUA)(z+o.z)); }
  IUA3 operator-(const IUA3& o) const { return IUA3((IUA)(x-o.x), (IUA)(y-o.y), (IUA)(z-o.z)); } };
struct IUA4 { IUA x, y, z, w;
  IUA4() : x(0), y(0), z(0), w(0) {}
  IUA4(IUA a, IUA b, IUA c, IUA d) : x(a), y(b), z(c), w(d) {}
  IUA4 operator+(const IUA4& o) const { return IUA4((IUA)(x+o.x), (IUA)(y+o.y), (IUA)(z+o.z), (IUA)(w+o.w)); }
  IUA4 operator-(const IUA4& o) const { return IUA4((IUA)(x-o.x), (IUA)(y-o.y), (IUA)(z-o.z), (IUA)(w-o.w)); } };

/* --- 16-bit integer --- */
struct ISB2 { ISB x, y;
  ISB2() : x(0), y(0) {} ISB2(ISB a, ISB b) : x(a), y(b) {}
  ISB2 operator+(const ISB2& o) const { return ISB2(x+o.x, y+o.y); }
  ISB2 operator-(const ISB2& o) const { return ISB2(x-o.x, y-o.y); } };
struct ISB3 { ISB x, y, z;
  ISB3() : x(0), y(0), z(0) {} ISB3(ISB a, ISB b, ISB c) : x(a), y(b), z(c) {}
  ISB3 operator+(const ISB3& o) const { return ISB3(x+o.x, y+o.y, z+o.z); }
  ISB3 operator-(const ISB3& o) const { return ISB3(x-o.x, y-o.y, z-o.z); } };
struct ISB4 { ISB x, y, z, w;
  ISB4() : x(0), y(0), z(0), w(0) {}
  ISB4(ISB a, ISB b, ISB c, ISB d) : x(a), y(b), z(c), w(d) {}
  ISB4 operator+(const ISB4& o) const { return ISB4(x+o.x, y+o.y, z+o.z, w+o.w); }
  ISB4 operator-(const ISB4& o) const { return ISB4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- 16-bit unsigned --- */
struct IUB2 { IUB x, y;
  IUB2() : x(0), y(0) {} IUB2(IUB a, IUB b) : x(a), y(b) {}
  IUB2 operator+(const IUB2& o) const { return IUB2((IUB)(x+o.x), (IUB)(y+o.y)); }
  IUB2 operator-(const IUB2& o) const { return IUB2((IUB)(x-o.x), (IUB)(y-o.y)); } };
struct IUB3 { IUB x, y, z;
  IUB3() : x(0), y(0), z(0) {} IUB3(IUB a, IUB b, IUB c) : x(a), y(b), z(c) {}
  IUB3 operator+(const IUB3& o) const { return IUB3((IUB)(x+o.x), (IUB)(y+o.y), (IUB)(z+o.z)); }
  IUB3 operator-(const IUB3& o) const { return IUB3((IUB)(x-o.x), (IUB)(y-o.y), (IUB)(z-o.z)); } };
struct IUB4 { IUB x, y, z, w;
  IUB4() : x(0), y(0), z(0), w(0) {}
  IUB4(IUB a, IUB b, IUB c, IUB d) : x(a), y(b), z(c), w(d) {}
  IUB4 operator+(const IUB4& o) const { return IUB4((IUB)(x+o.x), (IUB)(y+o.y), (IUB)(z+o.z), (IUB)(w+o.w)); }
  IUB4 operator-(const IUB4& o) const { return IUB4((IUB)(x-o.x), (IUB)(y-o.y), (IUB)(z-o.z), (IUB)(w-o.w)); } };

/* --- ISC2 and ISC4 already defined above. Adding ISC3. --- */
struct ISC3 { ISC x, y, z;
  ISC3() : x(0), y(0), z(0) {} ISC3(ISC a, ISC b, ISC c) : x(a), y(b), z(c) {}
  ISC3 operator+(const ISC3& o) const { return ISC3(x+o.x, y+o.y, z+o.z); }
  ISC3 operator-(const ISC3& o) const { return ISC3(x-o.x, y-o.y, z-o.z); } };

/* --- 32-bit unsigned --- */
struct IUC2 { IUC x, y;
  IUC2() : x(0), y(0) {} IUC2(IUC a, IUC b) : x(a), y(b) {}
  IUC2 operator+(const IUC2& o) const { return IUC2(x+o.x, y+o.y); }
  IUC2 operator-(const IUC2& o) const { return IUC2(x-o.x, y-o.y); } };
struct IUC3 { IUC x, y, z;
  IUC3() : x(0), y(0), z(0) {} IUC3(IUC a, IUC b, IUC c) : x(a), y(b), z(c) {}
  IUC3 operator+(const IUC3& o) const { return IUC3(x+o.x, y+o.y, z+o.z); }
  IUC3 operator-(const IUC3& o) const { return IUC3(x-o.x, y-o.y, z-o.z); } };
struct IUC4 { IUC x, y, z, w;
  IUC4() : x(0), y(0), z(0), w(0) {}
  IUC4(IUC a, IUC b, IUC c, IUC d) : x(a), y(b), z(c), w(d) {}
  IUC4 operator+(const IUC4& o) const { return IUC4(x+o.x, y+o.y, z+o.z, w+o.w); }
  IUC4 operator-(const IUC4& o) const { return IUC4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- 64-bit integer --- */
struct ISD2 { ISD x, y;
  ISD2() : x(0), y(0) {} ISD2(ISD a, ISD b) : x(a), y(b) {}
  ISD2 operator+(const ISD2& o) const { return ISD2(x+o.x, y+o.y); }
  ISD2 operator-(const ISD2& o) const { return ISD2(x-o.x, y-o.y); } };
struct ISD3 { ISD x, y, z;
  ISD3() : x(0), y(0), z(0) {} ISD3(ISD a, ISD b, ISD c) : x(a), y(b), z(c) {}
  ISD3 operator+(const ISD3& o) const { return ISD3(x+o.x, y+o.y, z+o.z); }
  ISD3 operator-(const ISD3& o) const { return ISD3(x-o.x, y-o.y, z-o.z); } };
struct ISD4 { ISD x, y, z, w;
  ISD4() : x(0), y(0), z(0), w(0) {}
  ISD4(ISD a, ISD b, ISD c, ISD d) : x(a), y(b), z(c), w(d) {}
  ISD4 operator+(const ISD4& o) const { return ISD4(x+o.x, y+o.y, z+o.z, w+o.w); }
  ISD4 operator-(const ISD4& o) const { return ISD4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- 64-bit unsigned --- */
struct IUD2 { IUD x, y;
  IUD2() : x(0), y(0) {} IUD2(IUD a, IUD b) : x(a), y(b) {}
  IUD2 operator+(const IUD2& o) const { return IUD2(x+o.x, y+o.y); }
  IUD2 operator-(const IUD2& o) const { return IUD2(x-o.x, y-o.y); } };
struct IUD3 { IUD x, y, z;
  IUD3() : x(0), y(0), z(0) {} IUD3(IUD a, IUD b, IUD c) : x(a), y(b), z(c) {}
  IUD3 operator+(const IUD3& o) const { return IUD3(x+o.x, y+o.y, z+o.z); }
  IUD3 operator-(const IUD3& o) const { return IUD3(x-o.x, y-o.y, z-o.z); } };
struct IUD4 { IUD x, y, z, w;
  IUD4() : x(0), y(0), z(0), w(0) {}
  IUD4(IUD a, IUD b, IUD c, IUD d) : x(a), y(b), z(c), w(d) {}
  IUD4 operator+(const IUD4& o) const { return IUD4(x+o.x, y+o.y, z+o.z, w+o.w); }
  IUD4 operator-(const IUD4& o) const { return IUD4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- ISE (64-bit signed, platform-dependent; 128-bit on some platforms
   where operator+/- are not defined, so no vector arithmetic). --- */
struct ISE2 { ISE x, y;
  ISE2() : x(0), y(0) {} ISE2(ISE a, ISE b) : x(a), y(b) {} };
struct ISE3 { ISE x, y, z;
  ISE3() : x(0), y(0), z(0) {} ISE3(ISE a, ISE b, ISE c) : x(a), y(b), z(c) {} };
struct ISE4 { ISE x, y, z, w;
  ISE4() : x(0), y(0), z(0), w(0) {}
  ISE4(ISE a, ISE b, ISE c, ISE d) : x(a), y(b), z(c), w(d) {} };

/* --- IUE (64/128-bit unsigned, platform-dependent; 128-bit on some
   platforms where operator+/- are not defined, so no vector arithmetic). --- */
struct IUE2 { IUE x, y;
  IUE2() : x(0), y(0) {} IUE2(IUE a, IUE b) : x(a), y(b) {} };
struct IUE3 { IUE x, y, z;
  IUE3() : x(0), y(0), z(0) {} IUE3(IUE a, IUE b, IUE c) : x(a), y(b), z(c) {} };
struct IUE4 { IUE x, y, z, w;
  IUE4() : x(0), y(0), z(0), w(0) {}
  IUE4(IUE a, IUE b, IUE c, IUE d) : x(a), y(b), z(c), w(d) {} };

/* --- Character types --- */
struct CHA2 { CHA x, y;
  CHA2() : x(0), y(0) {} CHA2(CHA a, CHA b) : x(a), y(b) {} };
struct CHA3 { CHA x, y, z;
  CHA3() : x(0), y(0), z(0) {} CHA3(CHA a, CHA b, CHA c) : x(a), y(b), z(c) {} };
struct CHA4 { CHA x, y, z, w;
  CHA4() : x(0), y(0), z(0), w(0) {}
  CHA4(CHA a, CHA b, CHA c, CHA d) : x(a), y(b), z(c), w(d) {} };

struct CHB2 { CHB x, y;
  CHB2() : x(0), y(0) {} CHB2(CHB a, CHB b) : x(a), y(b) {} };
struct CHB3 { CHB x, y, z;
  CHB3() : x(0), y(0), z(0) {} CHB3(CHB a, CHB b, CHB c) : x(a), y(b), z(c) {} };
struct CHB4 { CHB x, y, z, w;
  CHB4() : x(0), y(0), z(0), w(0) {}
  CHB4(CHB a, CHB b, CHB c, CHB d) : x(a), y(b), z(c), w(d) {} };

struct CHC2 { CHC x, y;
  CHC2() : x(0), y(0) {} CHC2(CHC a, CHC b) : x(a), y(b) {} };
struct CHC3 { CHC x, y, z;
  CHC3() : x(0), y(0), z(0) {} CHC3(CHC a, CHC b, CHC c) : x(a), y(b), z(c) {} };
struct CHC4 { CHC x, y, z, w;
  CHC4() : x(0), y(0), z(0), w(0) {}
  CHC4(CHC a, CHC b, CHC c, CHC d) : x(a), y(b), z(c), w(d) {} };

struct CHS2 { CHS x, y;
  CHS2() : x(0), y(0) {} CHS2(CHS a, CHS b) : x(a), y(b) {} };
struct CHS3 { CHS x, y, z;
  CHS3() : x(0), y(0), z(0) {} CHS3(CHS a, CHS b, CHS c) : x(a), y(b), z(c) {} };
struct CHS4 { CHS x, y, z, w;
  CHS4() : x(0), y(0), z(0), w(0) {}
  CHS4(CHS a, CHS b, CHS c, CHS d) : x(a), y(b), z(c), w(d) {} };

/* --- Boolean --- */
struct BOL2 { BOL x, y;
  BOL2() : x(false), y(false) {} BOL2(BOL a, BOL b) : x(a), y(b) {} };
struct BOL3 { BOL x, y, z;
  BOL3() : x(false), y(false), z(false) {} BOL3(BOL a, BOL b, BOL c) : x(a), y(b), z(c) {} };
struct BOL4 { BOL x, y, z, w;
  BOL4() : x(false), y(false), z(false), w(false) {}
  BOL4(BOL a, BOL b, BOL c, BOL d) : x(a), y(b), z(c), w(d) {} };

/* --- FPC2 and FPC4 already defined above. Adding FPC3. --- */
struct FPC3 {
  FPC x, y, z;
  FPC3() : x(0.f), y(0.f), z(0.f) {}
  FPC3(FPC a, FPC b, FPC c) : x(a), y(b), z(c) {}
  FPC3 operator+(const FPC3& o) const { return FPC3(x+o.x, y+o.y, z+o.z); }
  FPC3 operator-(const FPC3& o) const { return FPC3(x-o.x, y-o.y, z-o.z); }
  FPC3 operator*(FPC s) const { return FPC3(x*s, y*s, z*s); }
  FPC3 operator/(FPC s) const { return FPC3(x/s, y/s, z/s); }
  FPC3& operator+=(const FPC3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
  FPC3& operator-=(const FPC3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
  FPC3& operator*=(FPC s) { x*=s; y*=s; z*=s; return *this; }
  BOL operator==(const FPC3& o) const { return x==o.x && y==o.y && z==o.z; }
  BOL operator!=(const FPC3& o) const { return !(*this == o); }
  FPC Dist2(const FPC3& o) const {
    FPC dx=x-o.x, dy=y-o.y, dz=z-o.z;
    return dx*dx + dy*dy + dz*dz;
  }
  FPC Dot(const FPC3& o) const { return x*o.x + y*o.y + z*o.z; }
};

/* --- Double precision (STEM workhorse) --- */
struct FPD2 {
  FPD x, y;
  FPD2() : x(0.0), y(0.0) {}
  FPD2(FPD a, FPD b) : x(a), y(b) {}
  FPD2 operator+(const FPD2& o) const { return FPD2(x+o.x, y+o.y); }
  FPD2 operator-(const FPD2& o) const { return FPD2(x-o.x, y-o.y); }
  FPD2 operator*(FPD s) const { return FPD2(x*s, y*s); }
  FPD2 operator/(FPD s) const { return FPD2(x/s, y/s); }
  FPD2& operator+=(const FPD2& o) { x+=o.x; y+=o.y; return *this; }
  FPD2& operator-=(const FPD2& o) { x-=o.x; y-=o.y; return *this; }
  FPD2& operator*=(FPD s) { x*=s; y*=s; return *this; }
  BOL operator==(const FPD2& o) const { return x==o.x && y==o.y; }
  BOL operator!=(const FPD2& o) const { return !(*this == o); }
  FPD Dist2(const FPD2& o) const { FPD dx=x-o.x, dy=y-o.y; return dx*dx+dy*dy; }
  FPD Dot(const FPD2& o) const { return x*o.x + y*o.y; }
};

struct FPD3 {
  FPD x, y, z;
  FPD3() : x(0.0), y(0.0), z(0.0) {}
  FPD3(FPD a, FPD b, FPD c) : x(a), y(b), z(c) {}
  FPD3 operator+(const FPD3& o) const { return FPD3(x+o.x, y+o.y, z+o.z); }
  FPD3 operator-(const FPD3& o) const { return FPD3(x-o.x, y-o.y, z-o.z); }
  FPD3 operator*(FPD s) const { return FPD3(x*s, y*s, z*s); }
  FPD3 operator/(FPD s) const { return FPD3(x/s, y/s, z/s); }
  FPD3& operator+=(const FPD3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
  FPD3& operator-=(const FPD3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
  FPD3& operator*=(FPD s) { x*=s; y*=s; z*=s; return *this; }
  BOL operator==(const FPD3& o) const { return x==o.x && y==o.y && z==o.z; }
  BOL operator!=(const FPD3& o) const { return !(*this == o); }
  FPD Dist2(const FPD3& o) const {
    FPD dx=x-o.x, dy=y-o.y, dz=z-o.z;
    return dx*dx + dy*dy + dz*dz;
  }
  FPD Dot(const FPD3& o) const { return x*o.x + y*o.y + z*o.z; }
  FPD3 Cross(const FPD3& o) const {
    return FPD3(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x);
  }
};

struct FPD4 {
  FPD x, y, z, w;
  FPD4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
  FPD4(FPD a, FPD b, FPD c, FPD d) : x(a), y(b), z(c), w(d) {}
  FPD4 operator+(const FPD4& o) const { return FPD4(x+o.x, y+o.y, z+o.z, w+o.w); }
  FPD4 operator-(const FPD4& o) const { return FPD4(x-o.x, y-o.y, z-o.z, w-o.w); }
  FPD4 operator*(FPD s) const { return FPD4(x*s, y*s, z*s, w*s); }
  FPD4 operator/(FPD s) const { return FPD4(x/s, y/s, z/s, w/s); }
  FPD4& operator+=(const FPD4& o) { x+=o.x; y+=o.y; z+=o.z; w+=o.w; return *this; }
  FPD4& operator-=(const FPD4& o) { x-=o.x; y-=o.y; z-=o.z; w-=o.w; return *this; }
  FPD4& operator*=(FPD s) { x*=s; y*=s; z*=s; w*=s; return *this; }
  BOL operator==(const FPD4& o) const { return x==o.x && y==o.y && z==o.z && w==o.w; }
  BOL operator!=(const FPD4& o) const { return !(*this == o); }
  FPD Dist2(const FPD4& o) const {
    FPD dx=x-o.x, dy=y-o.y, dz=z-o.z, dw=w-o.w;
    return dx*dx + dy*dy + dz*dz + dw*dw;
  }
  FPD Dot(const FPD4& o) const { return x*o.x + y*o.y + z*o.z + w*o.w; }
};

/* --- FPE (double-extended, platform-dependent; operator+/- not
   guaranteed in all include contexts, so no vector arithmetic). --- */
struct FPE2 { FPE x, y;
  FPE2() : x(0.0), y(0.0) {} FPE2(FPE a, FPE b) : x(a), y(b) {} };
struct FPE3 { FPE x, y, z;
  FPE3() : x(0.0), y(0.0), z(0.0) {} FPE3(FPE a, FPE b, FPE c) : x(a), y(b), z(c) {} };
struct FPE4 { FPE x, y, z, w;
  FPE4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
  FPE4(FPE a, FPE b, FPE c, FPE d) : x(a), y(b), z(c), w(d) {} };

/* --- FPG (half-precision float, platform-dependent) --- */
struct FPG2 { FPG x, y;
  FPG2() : x(0.f), y(0.f) {} FPG2(FPG a, FPG b) : x(a), y(b) {}
  FPG2 operator+(const FPG2& o) const { return FPG2(x+o.x, y+o.y); }
  FPG2 operator-(const FPG2& o) const { return FPG2(x-o.x, y-o.y); } };
struct FPG3 { FPG x, y, z;
  FPG3() : x(0.f), y(0.f), z(0.f) {} FPG3(FPG a, FPG b, FPG c) : x(a), y(b), z(c) {}
  FPG3 operator+(const FPG3& o) const { return FPG3(x+o.x, y+o.y, z+o.z); }
  FPG3 operator-(const FPG3& o) const { return FPG3(x-o.x, y-o.y, z-o.z); } };
struct FPG4 { FPG x, y, z, w;
  FPG4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
  FPG4(FPG a, FPG b, FPG c, FPG d) : x(a), y(b), z(c), w(d) {}
  FPG4 operator+(const FPG4& o) const { return FPG4(x+o.x, y+o.y, z+o.z, w+o.w); }
  FPG4 operator-(const FPG4& o) const { return FPG4(x-o.x, y-o.y, z-o.z, w-o.w); } };

/* --- The old TVec2F/TVec2I/TFloatRect/TIntRect struct bodies are removed;
   they are now the `using` aliases above to the spec-conformant
   ISC2/FPC2/ISC4/FPC4 types (identical members + methods). Kept as aliases
   so existing TileWorld/CarWorld/BikeWorld code compiles unchanged. --- */

}  // namespace _

#endif  //< _CrabsHomotuplesH
