// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsLanguageH
#define _CrabsLanguageH
#include <_Config.h>
#if _Seam >= _CrabsDictionary
namespace _ {
namespace language {

enum {
  EnUppercaseBegin = 'A',
  EnUppercaseEnd   = 'Z',
  EnLowercaseBegin = 'a',
  EnLowercaseEnd   = 'z',
};
}  //< namespace language

/* A spoken language Unicode character range.

*/
struct Language {
  CHC uppercase_begin,  //< First uppercase Unicode CHA index.
      uppercase_end,    //< Last uppercase Unicode CHA index.
      lowercase_begin,  //< First lowercase Unicode CHA index.
      lowercase_end;    //< Last lowercase Unicode CHA index.
};

}  //< namespace _
#endif
#endif  //< _CrabsLanguageH
