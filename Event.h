// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsEventH
#define _CrabsEventH
#include <_Config.h>
#if _Seam >= _CrabsDictionary
namespace _ {

/* Interface for an abstract event.
 */
struct Event {
  /* Virtual destructor. */
  virtual ~Event() = 0;

  /* Triggers the event. */
  virtual void Trigger() = 0;
};

}  //< namespace _
#endif
#endif  //< _CrabsEventH
