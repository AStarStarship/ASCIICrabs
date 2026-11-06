// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBInHPP
#define _CrabsBInHPP
#include "BIn.h"
#include "AType.hpp"
#if _Seam >= _CrabsOp
namespace _ {

inline const IUA* BInOrigin(const TBIn<ISC>* bin) { 
  return TRef<IUA>(bin, sizeof(TBIn<ISC>));
}

inline IUA* BInOrigin(TBIn<ISC>* bin) {
    return TRef<IUA>(bin, sizeof(TBIn<ISC>));
}

template<typename Printer>
Printer& TBInPrintVars(Printer& printer, const TBIn<ISC>* bin) {
  ISW size = bin->bytes;
  return printer << Linef('_', 80) << "\nBIn: size:" << bin->bytes
    << " start:" << bin->origin << " stop:" << bin->stop
    << " read:" << bin->read;
}

/* Print the TBIn to the printer. */
template<typename Printer>
Printer& TBInPrint(Printer& o, const TBIn<ISC>* bin) {
  ISW size = bin->bytes;
  return o << Linef('_', 80) << "\nBIn: size:" << bin->bytes
           << " start:" << bin->origin << " stop:" << bin->stop
           << " read:" << bin->read << Hexf(BInOrigin(bin), size + sizeof(TBIn<ISC>));
}

template<typename CHS=CHR>
const CHS* TBInStates() {
  static const CHS StateStrings[BInStateInvalid + 1][16] = {
    /*00*/{'A','d','d','r','e','s','s'},
    /*01*/{'P','a','c','k','e','d',' ','A','r','g','s'},
    /*02*/{'P','a','c','k','e','d',' ','U','T','F','8'},
    /*03*/{'P','a','c','k','e','d',' ','U','T','F','1','6'},
    /*04*/{'P','a','c','k','e','d',' ','U','T','F','3','2'},
    /*05*/{'P','a','c','k','e','d',' ','V','a','r','i','n','t'},
    /*06*/{'P','a','c','k','e','d',' ',' ','O','b','j'},
    /*07*/{'V','e','r','i','f','y','i','n','g',' ','H','a','s','h'},
    /*08*/{'H','a','n','d','l','i','n','g',' ','E','r','r','o','r'},
    /*09*/{'D','i','s','c','o','n','n','e','c','t','e','d'},
    /*10*/{'A','c','k'},
    /*11*/{'L','o','c','k','e','d'},
    /*12*/{'P','a','c','k','e','d',' ','P','O','D'},
    /*13*/{'I','n','v','a','l','i','d' }
  };
  return &StateStrings[0][0];
}
template<typename CHS=CHR, typename IS=ISW>
inline const CHS* TBInStates(IS state) {
  if (state < 0 || state >= BInStateInvalid) state = BInStateInvalid;
  return &TBInStates<CHS>()[BInStateInvalid * 32];
}

}  //< namespace _

#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsBInHPP
