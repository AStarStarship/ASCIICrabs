// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_DIC
#include "../Dic.hpp"
#if SEAM == CRABS_DIC
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
using namespace ::_;
namespace CRTest {

template<typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, 
  typename ISY = ISQ, typename DT = DTB, typename HSH = IUN>
static void TestDic() {
  enum {
    Size = 512 * sizeof(ISZ),
    TotalInit = 8, //< Must be a multiple of 8.
  };
  D_COUT(Linef("\n\n---\n\nTesting ADic<IS") << CSizeCodef<ISZ>() << 
         ",CH" << CSizeCodef<CHS>() << ",IU" << CSizeCodef<ISZ>() << 
         ",IS" << CSizeCodef<ISY>() <<  " DT" << CSizeCodef<DT>() << 
         "> with Size:" << Size << " and Total:" << TotalInit);

  ADic<DIC_P, Size> dic(TotalInit);

  D_COUT("\n\nAfter init total:" << dic.Total() << " bytes:" << dic.CBytes() << 
         " size_words:" << dic.BytesWords() <<
         "\nPrinting empty dictionary:\n");
  D_COUT_DIC(dic.This());
  A_AVOW(ISY(0), dic.Find(TSEmpty<CHS>()));
  D_COUT("\nPopulating " << TotalInit << " test words...");

  const CHS* words = TTestWords<CHS>::Words(),
           * key   = words;
  ISY i = 0;
  A_AVOW(ISY(++i), dic.Insert(key += 16, ISA('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, IUA('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, ISB('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, IUB('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, ISC('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, IUC('!' + i)));
  A_AVOW(ISY(++i), dic.Insert(key += 16, ISD('!' + i)));

  D_COUT_DIC(dic.This());
  //D_COUT_TABLE(dic.Keys());
  D_COUT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nTesting Grow...\n");
  A_AVOW(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));

  while(i < TTestWords<CHS>::Total - 8) {
    A_AVOW(ISY(++i), dic.Insert(key += 16, ISA('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, IUA('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, ISB('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, IUB('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, ISC('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, IUC('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, ISD('!' + i)));
    A_AVOW(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));
    //@todo Change order above to test memory alignment.
    if (i != dic.Total())
      A_AVOW(ISY(++i), dic.Insert(key += 16, IUD('!' + i)));
  }

  D_COUT_DIC(dic.This());
}
}  //< namespace CRTest
#endif

namespace CRTest {
const CHA* Dic(const CHA* args) {
#if SEAM >= CRABS_DIC
  A_TEST_BEGIN;
//#if USING_STA == YES_0
  TestDic<CHA, CHC, ISC, ISB, DTB, IUC>();
//  TestDic<CHA, CHC, ISD, ISC, DTB, IUD>();
//#endif
//#if USING_STB == YES_0
//  TestDic<CHB, CHC, ISC, ISB, DTB, IUC>();
//  TestDic<CHB, CHC, ISD, ISC, DTB, IUD>();
//#endif
//#if USING_STC == YES_0
//  TestDic<CHC, CHC, ISC, ISB, DTB, IUC>();
//  TestDic<CHC, CHC, ISD, ISC, DTB, IUD>();
//#endif
#endif
  return NILP;
}
}  //< namespace CRTest
