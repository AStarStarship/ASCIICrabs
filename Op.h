// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsOpH
#define _CrabsOpH
#include <_Config.h>
#if _Seam >= _CrabsOp
#include "AType.h"
#include "AType.hpp"
namespace _ {


//enum {
//  CROpPush          = 0, //< Operation Type 0: Stack push .
//  CROpOperation     = 1, //< Operation Type 1: Abstract Operation.
//  CROpOperationPush = 2, //< Operation Type 2: Operation with stack push.
//};

/* An expression Operation with name key, multiple input params,
result, and optional description of a data set.
@code
static const Op This = { "Key",
    Params<1, 2>::Header, Params<1, 2>::Header,
    "Description", '}', ';', ' ', _NilP, "-", _NilP };

static const Op OpExample =   { "Key2",
    NumOps (0), FirstOp ('A'),
    "Description", '}', ';', ' ', _NilP, "-", _NilP };
@endcode */

const Op* OpError(ERC error);

}  //< namespace _

#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsOpH
