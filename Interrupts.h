// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsInterruptsH
#define _CrabsInterruptsH
#include <_Config.h>
#if _Seam >= _CrabsOp
#ifdef _CrabsUsingInterrupts
namespace _ {

/* Exception thrown to interrupt and crash the program. */
struct RoomCrashException : public std::exception {
  RoomCrashException() {}
};
}  //< namespace _
#endif
#endif
#endif  //< _CrabsInterruptsH
