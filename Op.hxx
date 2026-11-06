// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsOpHXX
#define _CrabsOpHXX
#include "Op.h"
#if _Seam >= _CrabsOp
#include "BSeq.hpp"
#include "Slot.h"
#include "Test.h"
#if _Seam == _CrabsOp
#include "_Debug.h"
#else
#include "_Release.h"
#include "AType.hpp"
#endif
namespace _ {

inline const Op* OpError(ERC error) { return (const Op*)IUW(error); }
/*
#if _CrabsUsingText
Op OpInit (IUW* socket, ISC buffer_size) {
    TBOut<ISC>* bout = BOutInit (socket, buffer_size);
    Op log;
    log.bout = bout;
    return log;
}

void Print (Op& log) {
  TBIn<ISC>* bin = TRef<TBIn<ISC>> (log.bout);
  void   * args[1];
  IUA     type = 0,
  iua;
  IUB iub;
  IUC iuc;
  IUD iud;
  //ISA   reserved,
  ISA   si1;
  IUB si2;
  IUC si4;
  IUD si8;
  FPC    flt;
  FPD   dbl;
  CHA   index;
  //if (BinReadChar (TRef<TBIn<ISC>> (log.bout), index))
  //    return;
  CHA socket[DBL_MAX_10_EXP + 2];
  while (index ) {
    switch (type) {
      case STR_: {
        if (BinRead (bin, Params<2, _ADR, STR_> (), Args (args, &iua,
                     socket)))
          return;
        Write (socket);
      }
      case _ISA: {
        if (BinRead (bin, Params<2, _ADR, _ISA> (), Args (args, &si1)))
          return;
        Write (si1);
      }
      case _IUA: {
        if (BinRead (bin, Params<2, _ADR, _IUA> (), Args (args, &iua)))
          return;
        Write (si1);
      }
      case _BOL: {
        if (BinRead (bin, Params<2, _ADR, _ISA> (), Args (args, &si1)))
          return;
        Write (si1);
      }
      case _ISB: {
        if (BinRead (bin, Params<2, _ADR, _ISB> (), Args (args, &si2)))
          return;
        Write (si1);
      }
      case _IUB: {
        if (BinRead (bin, Params<2, _ADR, _IUB> (),
                      Args (args, &iub, socket)))
          return;
        Write (si1);
      }
      case _ISC: {
        if (BinRead (bin, Params<2, _ADR, _ISC> (),
                      Args (args, &si4, socket)))
          return;
        Write (si1;
      }
      case _IUC: {
        if (BinRead (bin, Params<2, _ADR, _IUC> (), Args (args, &iuc)))
            return;
        Write (si1;
      }
      case _ISD: {
        if (BinRead (bin, Params<2, _ADR, _ISD> (), Args (args, &si8)))
            return;
        Write (si8;
      }
      case _IUD: {
        if (BinRead (bin, Params<2, _ADR, _IUD> (), Args (args, &iud)))
          return;
        Write (iud;
      }
      case _TMC: {
        if (BinRead (bin, Params<2, _ADR, kTMC> (), Args (args, &iuc)))
          return;
        ClockPrintTime (iuc);
      }
      case _TMD: {
        if (BinRead (bin, Params<2, _ADR, kTMD> (), Args (args, &iud)))
          return;
        ClockPrintTimestamp (iud);
      }
      case _FPC: {
        if (BinRead (bin, Params<2, _ADR, _FPC> (), Args (args, &flt)))
          return;
        Write (si1;
      }
      case _FPD: {
        if (BinRead (bin, Params<2, _ADR, STR_> (), Args (args, &dbl)))
          return;
        Write (si1;
      }
    }
  }
}
#endif */

}  //< namespace _

#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsOpHXX
