// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsOpHPP
#define _CrabsOpHPP
#include "Op.h"
#include "BSeq.hpp"
#if _Seam >= _CrabsOp
#include "String.hpp"
namespace _ {

template<typename Printer>
Printer& TOpPrint(Printer& o, const Op* op) {
  if (IsError(op)) {
    return o << "\nOp: nil";
  }
  return o << "\nOp      :" << op->name 
           << "\nBSQ     :" << TBSeq<ISC, DTB>(op->in)
           << "\nResult: :" << TBSeq<ISC, DTB>(op->out)
           << "\nMetadata:" << op->description;
}

/* Converts the given value to a pointer. */
inline ISW OpCount(const Op& op) { return op.out - op.in; }

/* Converts the given value to a pointer. */
inline const DTB* OpFirst(CHC index) { return TRef<const DTB>(index); }

/* Converts the given value to a pointer. */
inline CHC OpFirst(const Op* op) {
  return CHC(IUW(op->in));
}
/* Converts the given value to a pointer. */
inline const DTB* OpLast(CHC index) { return TRef<const DTB>(index); }

/* Converts the given value to a pointer. */
inline CHC OpLast(const Op* op) {
  return CHC(IUW(op->out));
}

}  //< namespace _

template<typename CHS, typename CHT, typename ISZ>
inline ::_::TSPrinter<CHS, CHT, ISZ>& operator<<(::_::TSPrinter<CHS, CHT, ISZ>& utf,
                                            const ::_::Op* op) {
  return ::_::TOpPrint<::_::TSPrinter<CHS, CHT, ISZ>>(utf, op);
}
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsOpHPP
