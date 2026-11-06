// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBufferH
#define _CrabsBufferH
#include <_Config.h>
#if _Seam >= _CrabsRoom
namespace _ {

/* Creates/Gets a static socket of the specified size. */
template<ISW BufferSize = BufferSizeDefault>
inline CHA* Buffer() {
  enum {
    BufferSizeWords =
        (BufferSize / sizeof(IUW)) + BufferSize % sizeof(IUW) ? 1 : 0
  };
  static IUW socket[BufferSizeWords];
  return TRef<CHA>(socket);
}

}  //< namespace _
#endif
#endif  //< _CrabsBufferH
