// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBOutHPP
#define _CrabsBOutHPP
#include "BOut.h"
#if _Seam >= _CrabsOp
#if _Seam == _CrabsOp
#include "_Debug.h"
#else
#include "_Release.h"
#include "AType.hpp"
#endif
namespace _ {

template<typename Printer>
Printer& TBOutPrint(Printer& o, const TBOut<ISC>* bout) {
  _AAssert(bout);
  ISN size = bout->bytes;
  o << Linef("\n___") << "\nBOut: size:" << size
    << " origin:" << bout->origin << " stop:" << bout->stop
    << " read:" << bout->read << Charsf(BOutBegin(bout), size - 1);
  return o;
}

template<typename Printer>
Printer& TBOutPrintVars(Printer& printer, TBOut<ISC>* bout) {
  _AAssert(bout);
  ISN size = bout->bytes;
  return printer << Linef('_', 80) << "\nBOut: size:" << size
    << " start:" << bout->origin << " stop:" << bout->stop
    << " read:" << bout->read;
}

template<typename Printer>
Printer& TBOutPrint(Printer& printer, TBOut<ISC>* bout) {
  _AAssert(bout);
  ISN size = bout->bytes;
  return printer << Linef('_', 80) << "\nBOut: size:" << size
          << " start:" << bout->origin << " stop:" << bout->stop
          << " read:" << bout->read << Charsf(BOutBuffer(bout), size - 1);
}
}  //< namespace _
#endif
#endif
