// Copyright AStarship <https://astarship.net>.
//
// TVec.h — no-stdlib 2D geometry PODs for the headless TileWorld engine.
//
// These are the in-memory layout of the ASCIICrabs VHT (Vector of
// Homotuples) type descriptors (VT=0, SW encodes element count, POD is the
// element type) per _Spec/Data/Types.md:
//   TVec2F   = 2-tuple of FPC   (sf::Vector2f replacement)
//   TVec2I   = 2-tuple of ISC   (sf::Vector2i replacement)
//   TFloatRect = 4-tuple of FPC (sf::FloatRect replacement)
//   TIntRect   = 4-tuple of ISC (sf::IntRect replacement)
//
// No <cmath>, <vector>, <string>. Pure FPC/ISC math. Render is a separate
// layer; these carry only the spatial data a headless world needs (position,
// bounds, occlusion math). The future SDL3/CrabsTK renderer reads these.

#pragma once
#include <_ConfigHeader.h>

#ifndef CRABS_TVEC_H
#define CRABS_TVEC_H

namespace _ {

/* --- 2-tuple of FPC: a 2D float point/vector. VHT<2, FPC>. ------------- */
struct TVec2F {
  FPC x;
  FPC y;
  TVec2F() : x(0.f), y(0.f) {}
  TVec2F(FPC x_, FPC y_) : x(x_), y(y_) {}
  /* --- arithmetic (SFML Vector2f parity, no <cmath>) --- */
  TVec2F operator+(const TVec2F& o) const { return TVec2F(x + o.x, y + o.y); }
  TVec2F operator-(const TVec2F& o) const { return TVec2F(x - o.x, y - o.y); }
  TVec2F operator*(FPC s) const { return TVec2F(x * s, y * s); }
  TVec2F operator/(FPC s) const { return TVec2F(x / s, y / s); }
  TVec2F& operator+=(const TVec2F& o) { x += o.x; y += o.y; return *this; }
  TVec2F& operator-=(const TVec2F& o) { x -= o.x; y -= o.y; return *this; }
  TVec2F& operator*=(FPC s) { x *= s; y *= s; return *this; }
  BOL operator==(const TVec2F& o) const { return x == o.x && y == o.y; }
  BOL operator!=(const TVec2F& o) const { return !(*this == o); }
  /* squared distance (no sqrt, no <cmath>); compare squared for nearest. */
  FPC Dist2(const TVec2F& o) const {
    FPC dx = x - o.x;
    FPC dy = y - o.y;
    return dx * dx + dy * dy;
  }
};

/* --- 2-tuple of ISC: a 2D integer point (grid coordinates). VHT<2, ISC>. */
struct TVec2I {
  ISC x;
  ISC y;
  TVec2I() : x(0), y(0) {}
  TVec2I(ISC x_, ISC y_) : x(x_), y(y_) {}
  BOL operator==(const TVec2I& o) const { return x == o.x && y == o.y; }
  BOL operator!=(const TVec2I& o) const { return !(*this == o); }
};

/* --- 4-tuple of FPC: an axis-aligned float rect. VHT<4, FPC>. ----------
   SFML sf::FloatRect uses (left, top, width, height). We keep that layout
   (x=left, y=top) so collision math is a 1:1 port. */
struct TFloatRect {
  FPC x;    // left
  FPC y;    // top
  FPC w;    // width
  FPC h;    // height
  TFloatRect() : x(0.f), y(0.f), w(0.f), h(0.f) {}
  TFloatRect(FPC x_, FPC y_, FPC w_, FPC h_) : x(x_), y(y_), w(w_), h(h_) {}
  FPC Left() const { return x; }
  FPC Top() const { return y; }
  FPC Right() const { return x + w; }
  FPC Bottom() const { return y + h; }
  TVec2F Center() const { return TVec2F(x + w * 0.5f, y + h * 0.5f); }
  /* AABbox intersection (SFML FloatRect::intersects parity). */
  BOL Intersects(const TFloatRect& o) const {
    return x < o.Right() && Right() > o.x && y < o.Bottom() && Bottom() > o.y;
  }
  BOL Contains(const TVec2F& p) const {
    return p.x >= x && p.x < Right() && p.y >= y && p.y < Bottom();
  }
  /* Inflate in place by (dx, dy) on each edge. */
  TFloatRect& Inflate(FPC dx, FPC dy) {
    x -= dx;
    y -= dy;
    w += dx * 2.f;
    h += dy * 2.f;
    return *this;
  }
};

/* --- 4-tuple of ISC: an axis-aligned integer rect (texture sub-rect,
   grid bounds). VHT<4, ISC>. SFML sf::IntRect layout (left, top, w, h). */
struct TIntRect {
  ISC x;
  ISC y;
  ISC w;
  ISC h;
  TIntRect() : x(0), y(0), w(0), h(0) {}
  TIntRect(ISC x_, ISC y_, ISC w_, ISC h_) : x(x_), y(y_), w(w_), h(h_) {}
  ISC Left() const { return x; }
  ISC Top() const { return y; }
  ISC Right() const { return x + w; }
  ISC Bottom() const { return y + h; }
  BOL Intersects(const TIntRect& o) const {
    return x < o.Right() && Right() > o.x && y < o.Bottom() && Bottom() > o.y;
  }
  BOL Contains(const TVec2I& p) const {
    return p.x >= x && p.x < Right() && p.y >= y && p.y < Bottom();
  }
};

}  // namespace _

#endif  // CRABS_TVEC_H
