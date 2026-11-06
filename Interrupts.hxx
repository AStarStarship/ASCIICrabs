// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsInterruptsHXX
#define _CrabsInterruptsHXX
#include "Interrupts.h"
#if _Seam >= _CrabsOp && USING_CRABS_INTERRUPTS
#include <cassert>
namespace _ {

#if _CrabsUsingText
const CHA* RoomCrashException::what() const throw() { return "External crash"; }
#endif

}  //< namespace _
#endif  ///< #if _Seam >= _CrabsOp

#endif  //< _CrabsInterruptsHXX
