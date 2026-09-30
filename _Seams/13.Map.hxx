// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_MAP
#include "../Map.hpp"
//
#include "../Random.h"
#if SEAM == CRABS_MAP
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
using namespace ::_;
namespace CRTest {

template<typename D, D DomainMin_, D DomainMax_, typename ISZ,
         ISZ CodomainMin_, ISZ CodomainMax_, ISZ Size_>
void TestMap() {
  D_COUT(Linef("\n\n\n\n\n\n+---\nTesting AMap<D")
         << sizeof(D) << ",ISZ" << CHA('0' + sizeof(ISZ))
         << Linef(">\n+---\n\n"));

  AMap<MAP_P, Size_> map;
  D_COUT_OBJ(map);

  D_COUT("map_size:" << map.Size() << " map_bytes:" << map.CBytes()
                     << " map_size_words:" << map.SizeWords()
                     << " sizeof(TMapBuf<D,ISZ>):" << sizeof(TMapBuf<D, ISZ>)
                     << "\n domain:" << TDelta<>(map.This(), map.Domain())
                     << " codomain:" << TDelta<>(map.This(), map.Codomain()));
  D domain[Size_] = { 0 };

  D_COUT(Headingf("Generating random domain values..."));

  for (ISZ i = 0; i < Size_; ++i) {
    D d = 0;
    Random(d);
    domain[i] = d;
    D_COUT("\n" << i << ".) " << d);
  }

  for (ISZ i = 0; i < Size_; ++i)
    map.Add(domain[i]);

  D_COUT_MAP(map.This());

  D_COUT(Headingf("Searching the domain for each domain[i]..."));
  for (ISZ i = 0; i < Size_; ++i) 
    A_AVOW_INDEX(i, map.Find(domain[i]), i);

  D_COUT(Headingf("Remapping the codomain to random numbers..."));

  ISZ codomain[Size_] = { 0 };

  for (ISZ i = 0; i < Size_; ++i) {
    ISZ c = Random(CodomainMin_, CodomainMax_);
    codomain[i] = c;
    map.RemapCodomain(i, c);
  }
  D_COUT_OBJ(map);
  D_COUT(Headingf("Searching for the remapping..."));

  for (ISZ i = 0; i < Size_; ++i)
    A_AVOW_INDEX(i, map.FindCodomain(codomain[i]), i);
  D_COUT_OBJ(map);
}
}  //< namespace CRTest
#endif

namespace CRTest {
const CHA* Map(const CHA* args) {
#if SEAM >= CRABS_MAP
  A_TEST_BEGIN;

  TestMap<ISB, CISMin<ISB>(), CISMax<ISB>(), ISB, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISB, CISMin<ISB>(), CISMax<ISB>(), ISC, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISB, CISMin<ISB>(), CISMax<ISB>(), ISD, CISMin<ISB>(),
          CISMax<ISB>(), 8>();

  TestMap<ISC, CISMin<ISB>(), CISMax<ISB>(), ISB, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISC, CISMin<ISB>(), CISMax<ISB>(), ISC, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISC, CISMin<ISB>(), CISMax<ISB>(), ISD, CISMin<ISB>(),
          CISMax<ISB>(), 8>();

  TestMap<ISD, CISMin<ISB>(), CISMax<ISB>(), ISB, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISD, CISMin<ISB>(), CISMax<ISB>(), ISC, CISMin<ISB>(),
          CISMax<ISB>(), 8>();
  TestMap<ISD, CISMin<ISB>(), CISMax<ISB>(), ISD, CISMin<ISB>(),
          CISMax<ISB>(), 8>();

#endif
  return NILP;
}
}  //< namespace CRTest
