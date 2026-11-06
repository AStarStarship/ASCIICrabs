// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsDictionary
#include "../Dic.hpp"
#if _Seam == _CrabsDictionary
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
namespace CRTest {
using namespace ::_;

template<typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, 
  typename ISY = ISQ, typename DT = DTB, typename HSH = IUN>
static void TestDictionary() {
  enum {
    Size = 512 * sizeof(ISZ),
    TotalInit = 8, //< Must be a multiple of 8.
  };
  _DROut(Linef("\n\n---\n\nTesting ADic<IS") << CSizeCodef<ISZ>() << 
         ",CH" << CSizeCodef<CHS>() << ",IU" << CSizeCodef<ISZ>() << 
         ",IS" << CSizeCodef<ISY>() <<  " DT" << CSizeCodef<DT>() << 
         "> with Size:" << Size << " and Total:" << TotalInit);

  ADic<DIC_P, Size> dic(TotalInit);

  _DROut("\n\nAfter init total:" << dic.Total() << " bytes:" << dic.CBytes() << 
         " size_words:" << dic.BytesWords() <<
         "\nPrinting empty dictionary:\n");
  _DROutDic(dic.This());
  _AAvow(ISY(0), dic.Find(TSEmpty<CHS>()));
  _DROut("\nPopulating " << TotalInit << " test words...");

  const CHS* words = TTestWords<CHS>::Words(),
           * key   = words;
  ISY i = 0;
  _AAvow(ISY(++i), dic.Insert(key += 16, ISA('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, IUA('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, ISB('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, IUB('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, ISC('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, IUC('!' + i)));
  _AAvow(ISY(++i), dic.Insert(key += 16, ISD('!' + i)));

  _DROutDic(dic.This());
  //_DROutTable(dic.Keys());
  _DROut("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nTesting Grow...\n");
  _AAvow(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));

  while(i < TTestWords<CHS>::Total - 8) {
    _AAvow(ISY(++i), dic.Insert(key += 16, ISA('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, IUA('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, ISB('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, IUB('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, ISC('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, IUC('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, ISD('!' + i)));
    _AAvow(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));
    //@todo Change order above to test memory alignment.
    if (i != dic.Total())
      _AAvow(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));
  }

  _DROutDic(dic.This());
}
}  //< namespace CRTest
#endif

namespace CRTest {
const CHA* Dictionary(const CHA* args) {
#if _Seam >= _CrabsDictionary
  _ATestBegin;
  TestDictionary<CHA, CHC, ISC, ISB, DTB, IUC>();
//  TestDictionary<CHA, CHC, ISD, ISC, DTB, IUD>();
//
//  TestDictionary<CHB, CHC, ISC, ISB, DTB, IUC>();
//  TestDictionary<CHB, CHC, ISD, ISC, DTB, IUD>();
//
//  TestDictionary<CHC, CHC, ISC, ISB, DTB, IUC>();
//  TestDictionary<CHC, CHC, ISD, ISC, DTB, IUD>();

#endif
  return _NilP;
}
}  //< namespace CRTest
