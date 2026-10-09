// Copyright AStarship <https://astarship.net>.
// Unit test: TOcclusionMap DDA raycast + per-observer visibility.
// Hand-verified 8x5 grid with a vertical wall column; observer on the left.
//   . = empty (see-through)    # = occluder (wall)
//   O = observer at (1,2)
// Grid (x=col 0..7, y=row 0..4), wall at x=4 (all y):
//   x: 0 1 2 3 4 5 6 7
//   y0: . . . . # . . .
//   y1: . . . . # . . .
//   y2: . O . . # . . .
//   y3: . . . . # . . .
//   y4: . . . . # . . .
// Observer at cell (1,2) center (1.5, 2.5).
// Expectations (line-of-sight to cell center):
//   - same cell (1,2): visible
//   - (0,2) left: visible (no occluder between)
//   - (3,2) just left of wall: visible
//   - (5,2) just right of wall, same row: NOT visible (wall at (4,2) blocks)
//   - (7,0) far top-right: NOT visible (wall column blocks the straight shot)
//   - (0,0) top-left: visible
// No <cmath>/<vector>/<string>. Exits 0 iff all pass.
#include <_ConfigHeader.h>
#include "TVec.h"
#include "TOcclusion.h"
using namespace _;
extern "C" int write(int, const void*, unsigned long);
static void Say(const char* s) {
  unsigned long n = 0;
  while (s[n]) ++n;
  write(1, s, n);
}

int main() {
  int fails = 0;
  TOcclusionMap m;
  if (!m.Init(8, 5)) {
    Say("FAIL Init\n");
    return 1;
  }
  // Build the wall at x=4, all y.
  for (ISC y = 0; y < 5; ++y) m.SetOccluded(4, y, true);

  // One observer at cell (1,2) center.
  ISC o = m.AddObserver(1.5f, 2.5f);
  m.Recompute();

  struct Case {
    ISC x;
    ISC y;
    BOL expect;
    const char* name;
  };
  Case cases[] = {
      {1, 2, true, "same cell (1,2)"},
      {0, 2, true, "left (0,2)"},
      {3, 2, true, "just-left-of-wall (3,2)"},
      {5, 2, false, "just-right-of-wall (5,2) blocked"},
      {7, 0, false, "far top-right (7,0) blocked"},
      {0, 0, true, "top-left (0,0)"},
      {2, 0, true, "above observer (2,0)"},
      {6, 2, false, "right (6,2) blocked"},
  };
  int n = sizeof(cases) / sizeof(cases[0]);
  for (int i = 0; i < n; ++i) {
    BOL got = m.ObserverSees(o, cases[i].x, cases[i].y);
    if (got == cases[i].expect) {
      Say("PASS ");
      Say(cases[i].name);
      Say("\n");
    } else {
      Say("FAIL ");
      Say(cases[i].name);
      Say(" (expected ");
      Say(cases[i].expect ? "visible" : "hidden");
      Say(", got ");
      Say(got ? "visible" : "hidden");
      Say(")\n");
      ++fails;
    }
  }

  // Layer B check: a dynamic occluder (sprite) at (3,2) now blocks (5,2)...
  // actually (3,2) is left of the wall; add an occluder between observer and
  // a previously-visible cell to prove dynamic occluders are honored.
  // Occlude (0,2): now (0,2) should become hidden? No — the DEST cell is not
  // treated as blocking, so occluding the destination doesn't hide it. Instead
  // occlude (1,2)? That's the observer cell (never blocks). Occlude (0,0)
  // path: put occluder at (0,1) which lies between (1.5,2.5)->(0.5,0.5)? The
  // segment from (1.5,2.5) to (0.5,0.5) passes through cells (1,2),(1,1),(0,1),
  // (0,0). Occlude (1,1) -> (0,0) should become hidden.
  m.SetOccluded(1, 1, true);
  m.Recompute();
  {
    BOL sees_tl = m.ObserverSees(o, 0, 0);
    if (sees_tl == false) {
      Say("PASS dynamic-occluder (1,1) hides (0,0)\n");
    } else {
      Say("FAIL dynamic-occluder: (0,0) still visible after occluding (1,1)\n");
      ++fails;
    }
  }

  // Print the visibility field for the original (pre-dynamic-occluder) state
  // for the record — recompute on a fresh map without the dynamic occluder.
  {
    TOcclusionMap m2;
    m2.Init(8, 5);
    for (ISC y = 0; y < 5; ++y) m2.SetOccluded(4, y, true);
    ISC o2 = m2.AddObserver(1.5f, 2.5f);
    m2.Recompute();
    Say("visibility grid (O at (1,2), wall at x=4), # = occluder, . = hidden, * = seen:\n");
    for (ISC y = 4; y >= 0; --y) {
      for (ISC x = 0; x < 8; ++x) {
        CHA c = m2.Occluded(x, y) ? '#' : (m2.ObserverSees(o2, x, y) ? '*' : '.');
        write(1, &c, 1);
      }
      write(1, "\n", 1);
    }
  }

  Say(fails == 0 ? "OCCLUSION_TEST_PASS\n" : "OCCLUSION_TEST_FAIL\n");
  return fails == 0 ? 0 : 1;
}
