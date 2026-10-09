// Copyright AStarship <https://astarship.net>.
//
// TOcclusion.h — headless occlusion / line-of-sight map for TileWorld.
//
// This is the "headless with sprites that have an occlusion map" primitive.
// It makes the world *spatially aware* without any renderer:
//
//   Occluders  = static collision tiles          (layer A)
//             ∪ dynamic sprites with occlude_=1  (layer B)
//   Visibility = DDA raycast from each observer  (the engine)
//             → per-observer, per-cell grid       (layer C output)
//
// All three "levels" (static / dynamic / per-observer field) are the SAME
// system: A and B are two sources of occluders feeding one DDA raycast, and
// C is the output format (a per-observer visibility grid). No SFML, no
// <cmath>, no <vector>, no heap beyond the caller-provided grid buffers.
//
// The DDA (Amanatides & Woo grid traversal) is pure FPC/ISC math.

#pragma once
#include <_ConfigHeader.h>
#include "TVec.h"

#ifndef CRABS_TOCCLUSION_H
#define CRABS_TOCCLUSION_H

namespace _ {

/* Max observers per occlusion map (player + enemies). Fixed so the
   visibility storage is a flat caller buffer of ObsMax * W * H bytes. */
static const ISC TOcclusionObsMax = 8;

// Local abs for the DDA (avoid <cmath>); input is any FPC.
inline FPC fabs_step(FPC v) { return v < 0 ? -v : v; }

struct TOcclusionMap {
  ISC width;    // grid cells in x
  ISC height;   // grid cells in y
  ISC obs_count;

  // Occluder grid: 1 = this cell blocks sight (static collision tile OR a
  // dynamic sprite with occlude_=1 covering it). Rebuilt by RebuildOccluders.
  IUA* occluded;  // [width * height], caller owns/frees

  // Observer positions in grid-cell space (float, sub-cell precision).
  TVec2F obs_pos[TOcclusionObsMax];

  // Per-observer visibility: vis[o][y * width + x] = 1 iff observer o has
  // line-of-sight to cell (x,y). Rebuilt by Recompute.
  IUA* vis;  // [obs_count * width * height], caller owns/frees

  TOcclusionMap()
      : width(0), height(0), obs_count(0), occluded(NILP), vis(NILP) {}

  ~TOcclusionMap() {
    if (occluded != NILP) delete[] occluded;
    if (vis != NILP) delete[] vis;
  }

  /* Allocate the grids for a width x height map. */
  BOL Init(ISC w, ISC h) {
    if (w <= 0 || h <= 0) return false;
    width = w;
    height = h;
    if (occluded != NILP) delete[] occluded;
    if (vis != NILP) delete[] vis;
    occluded = new IUA[IUD(w) * IUD(h)]();
    // vis is sized for the max observer count so Recompute can fill any
    // obs_count <= TOcclusionObsMax without reallocation.
    vis = new IUA[IUD(TOcclusionObsMax) * IUD(w) * IUD(h)]();
    return true;
  }

  ISC CellIndex(ISC x, ISC y) const { return IUD(y) * IUD(width) + IUD(x); }
  BOL InBounds(ISC x, ISC y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
  }

  /* --- Layer A+B: mark a cell as an occluder / clear it. --- */
  void SetOccluded(ISC x, ISC y, BOL on) {
    if (InBounds(x, y)) occluded[CellIndex(x, y)] = on ? 1 : 0;
  }
  BOL Occluded(ISC x, ISC y) const {
    if (!InBounds(x, y)) return true;  // out of bounds = solid (can't see)
    return occluded[CellIndex(x, y)] != 0;
  }

  /* --- Observers (layer B participants). --- */
  void ClearObservers() { obs_count = 0; }
  /* Add an observer at a sub-cell position. Returns its index, or -1. */
  ISC AddObserver(FPC px, FPC py) {
    if (obs_count >= TOcclusionObsMax) return -1;
    obs_pos[obs_count] = TVec2F(px, py);
    return obs_count++;
  }

  /* Cast line-of-sight from (x0,y0) to (x1,y1) in cell space via DDA
     (Amanatides & Woo grid traversal). Returns true iff the segment is NOT
     blocked by any occluded cell it passes through. The destination cell is
     not treated as blocking (you can see the cell an enemy stands in); the
     observer's own starting cell is never blocking. Pure FPC/ISC math, no
     <cmath>: truncation toward zero via (ISC) cast, abs via ternary. */
  BOL LineOfSight(FPC x0, FPC y0, FPC x1, FPC y1) const {
    // Truncate toward zero to get the starting cell (cell space is >=0 in
    // practice, but guard negative anyway).
    ISC x = (x0 >= 0) ? (ISC)x0 : ((ISC)x0 - 1);
    ISC y = (y0 >= 0) ? (ISC)y0 : ((ISC)y0 - 1);
    ISC x_end = (x1 >= 0) ? (ISC)x1 : ((ISC)x1 - 1);
    ISC y_end = (y1 >= 0) ? (ISC)y1 : ((ISC)y1 - 1);
    FPC dx = x1 - x0;
    FPC dy = y1 - y0;
    ISC step_x = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    ISC step_y = dy > 0 ? 1 : (dy < 0 ? -1 : 0);
    // Distance (in parameter t) from the current cell edge to the next cell
    // boundary, and the per-step increment. Guard division by zero.
    FPC adx = fabs_step(dx);
    FPC ady = fabs_step(dy);
    FPC tdelta_x = adx > 0 ? (FPC)1.f / adx : 1e30f;
    FPC tdelta_y = ady > 0 ? (FPC)1.f / ady : 1e30f;
    // t distance to the first vertical / horizontal boundary ahead.
    FPC tmax_x = (step_x > 0) ? ((FPC)(x + 1) - x0) * tdelta_x
                              : (step_x < 0) ? (x0 - (FPC)x) * tdelta_x
                                             : 1e30f;
    FPC tmax_y = (step_y > 0) ? ((FPC)(y + 1) - y0) * tdelta_y
                              : (step_y < 0) ? (y0 - (FPC)y) * tdelta_y
                                             : 1e30f;

    ISC guard = 0;
    ISC max_steps = width + height + 2;
    while (guard++ < max_steps) {
      if (x == x_end && y == y_end) return true;  // reached dest, unblocked
      // The observer's own starting cell never blocks sight out of itself.
      if (!(x == (x0 >= 0 ? (ISC)x0 : (ISC)x0 - 1) &&
            y == (y0 >= 0 ? (ISC)y0 : (ISC)y0 - 1))) {
        if (Occluded(x, y)) return false;
      }
      if (tmax_x < tmax_y) {
        x += step_x;
        tmax_x += tdelta_x;
      } else {
        y += step_y;
        tmax_y += tdelta_y;
      }
      if (!InBounds(x, y)) return false;  // ray left the map
    }
    return false;
  }

  /* --- Layer C: recompute the per-observer visibility grids. ---
     For each observer, every cell with clear line-of-sight is marked 1.
     O(obs_count * width * height * avg_path_len). */
  void Recompute() {
    if (vis == NILP || occluded == NILP) return;
    for (ISC o = 0; o < obs_count; ++o) {
      IUA* row = &vis[IUD(o) * IUD(width) * IUD(height)];
      FPC ox = obs_pos[o].x;
      FPC oy = obs_pos[o].y;
      for (ISC cy = 0; cy < height; ++cy) {
        for (ISC cx = 0; cx < width; ++cx) {
          row[CellIndex(cx, cy)] =
              LineOfSight(ox, oy, (FPC)cx + 0.5f, (FPC)cy + 0.5f) ? 1 : 0;
        }
      }
    }
  }

  /* Can observer o see cell (cx,cy)? (O(1) after Recompute.) */
  BOL ObserverSees(ISC o, ISC cx, ISC cy) const {
    if (o < 0 || o >= obs_count) return false;
    if (!InBounds(cx, cy)) return false;
    return vis[IUD(o) * IUD(width) * IUD(height) + CellIndex(cx, cy)] != 0;
  }
};

}  // namespace _

#endif  // CRABS_TOCCLUSION_H
