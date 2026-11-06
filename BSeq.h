// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBSeqH
#define _CrabsBSeqH
#define CRABS_BSEQ_DECL
#include "AType.h"
#if _Seam >= _CrabsOp
namespace _ {

/*< Returns the requested parameter number_. */
IUC BSeqParamNumber(const DTB* params, ISN param_number);

}  //< namespace _

#endif
#endif  //< _CrabsBSeqH
