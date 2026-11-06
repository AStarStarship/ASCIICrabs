// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsList
#include "../List.hpp"
#include "../Random.h"
//#include "../Uniprinter.hpp"
#if _Seam == _CrabsList
#include "../_Debug.h"
#define TEST_MIN 0
#define TEST_MAX 0xffff
#else
#include "../_Release.h"
#define TEST_MIN 0
#define TEST_MAX 0xffff
#endif
namespace CRTest {
using namespace ::_;

template<LST_A>
static void TestList() {
  _DROut(Linef("\n\n\n\n+---\n Testing AList<IS")
         << CSizeCodef<ISZ>() << Linef(">\n+---"));
  AList<LST_P, 256> list(8);
  _DROut("\n\nlist.Size():" << list.Total()
                            << " list.Bytes():" << list.CBytes()
                            << " list.SizeWords():" << list.BytesWords());
  _DROutOBJ(list);
  _DROut("\n\nStep 1: Fill up the list...");
  auto i = list.Count();
  _AAvow(i, list.Insert(ISA('!' + i)));
  auto tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_ISA), tv.type);
  _AAvow(ISA('!' + i++), *TRef<ISA>(tv.value));

  _AAvow(i, list.Insert(CHA('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_CHA), tv.type);
  _AAvow(IUA('!' + i++), *TRef<IUA>(tv.value));

  _AAvow(i, list.Insert(ISB('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_ISB), tv.type);
  _AAvow(ISB('!' + i++), *TRef<ISB>(tv.value));

  _AAvow(i, list.Insert(IUB('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_IUB), tv.type);
  _AAvow(IUB('!' + i++), *TRef<IUB>(tv.value));

  _AAvow(i, list.Insert(ISC('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_ISC), tv.type);
  _AAvow(ISC('!' + i++), *TRef<ISC>(tv.value));

  _AAvow(i, list.Insert(CHC('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_CHC), tv.type);
  _AAvow(IUC('!' + i++), *TRef<IUC>(tv.value));

  _AAvow(i, list.Insert(ISD('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_ISD), tv.type);
  _AAvow(ISD('!' + i++), *TRef<ISD>(tv.value));

  _AAvow(i, list.Insert(IUD('!' + i)));
  tv = TListTypeValue<LST_P>(list.This(), i);
  _AAvow(DT(_IUD), tv.type);
  _AAvow(IUD('!' + i++), *TRef<IUD>(tv.value));

  _DROutOBJ(list);

  _DROut("\n\nStep 2b: Testing auto-grow up to 16...");
  for (; i < 16;) {
    _AAvow(i, list.Insert(ISA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISA), tv.type);
    _AAvow(ISA('!' + i++), *TRef<ISA>(tv.value));

    _AAvow(i, list.Insert(IUA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUA), tv.type);
    _AAvow(IUA('!' + i++), *TRef<IUA>(tv.value));

    _AAvow(i, list.Insert(ISB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISB), tv.type);
    _AAvow(ISB('!' + i++), *TRef<ISB>(tv.value));

    _AAvow(i, list.Insert(IUB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUB), tv.type);
    _AAvow(IUB('!' + i++), *TRef<IUB>(tv.value));

    _AAvow(i, list.Insert(ISC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISC), tv.type);
    _AAvow(ISC('!' + i++), *TRef<ISC>(tv.value));

    _AAvow(i, list.Insert(IUC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUC), tv.type);
    _AAvow(IUC('!' + i++), *TRef<IUC>(tv.value));

    _AAvow(i, list.Insert(ISD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISD), tv.type);
    _AAvow(ISD('!' + i++), *TRef<ISD>(tv.value));

    _AAvow(i, list.Insert(IUD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUD), tv.type);
    _AAvow(IUD('!' + i++), *TRef<IUD>(tv.value));
  }
  _DROutOBJ(list);
  _DROut("\n\nStep 2c: Testing auto-grow up to 32...");
  for (; i < 32;) {
    _AAvow(i, list.Insert(ISA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISA), tv.type);
    _AAvow(ISA('!' + i++), *TRef<ISA>(tv.value));

    _AAvow(i, list.Insert(IUA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUA), tv.type);
    _AAvow(IUA('!' + i++), *TRef<IUA>(tv.value));

    _AAvow(i, list.Insert(ISB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISB), tv.type);
    _AAvow(ISB('!' + i++), *TRef<ISB>(tv.value));

    _AAvow(i, list.Insert(IUB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUB), tv.type);
    _AAvow(IUB('!' + i++), *TRef<IUB>(tv.value));

    _AAvow(i, list.Insert(ISC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISC), tv.type);
    _AAvow(ISC('!' + i++), *TRef<ISC>(tv.value));

    _AAvow(i, list.Insert(IUC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUC), tv.type);
    _AAvow(IUC('!' + i++), *TRef<IUC>(tv.value));

    _AAvow(i, list.Insert(ISD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISD), tv.type);
    _AAvow(ISD('!' + i++), *TRef<ISD>(tv.value));

    _AAvow(i, list.Insert(IUD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUD), tv.type);
    _AAvow(IUD('!' + i++), *TRef<IUD>(tv.value));
  }
  _DROutOBJ(list);
  _DROut("\n\nStep 2d: Testing auto-grow up to 64...");
  for (; i < 64;) {
    _AAvow(i, list.Insert(ISA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISA), tv.type);
    _AAvow(ISA('!' + i++), *TRef<ISA>(tv.value));

    _AAvow(i, list.Insert(IUA('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUA), tv.type);
    _AAvow(IUA('!' + i++), *TRef<IUA>(tv.value));

    _AAvow(i, list.Insert(ISB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISB), tv.type);
    _AAvow(ISB('!' + i++), *TRef<ISB>(tv.value));

    _AAvow(i, list.Insert(IUB('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUB), tv.type);
    _AAvow(IUB('!' + i++), *TRef<IUB>(tv.value));

    _AAvow(i, list.Insert(ISC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISC), tv.type);
    _AAvow(ISC('!' + i++), *TRef<ISC>(tv.value));

    _AAvow(i, list.Insert(IUC('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUC), tv.type);
    _AAvow(IUC('!' + i++), *TRef<IUC>(tv.value));

    _AAvow(i, list.Insert(ISD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_ISD), tv.type);
    _AAvow(ISD('!' + i++), *TRef<ISD>(tv.value));

    _AAvow(i, list.Insert(IUD('!' + i)));
    tv = TListTypeValue<LST_P>(list.This(), i);
    _AAvow(DT(_IUD), tv.type);
    _AAvow(IUD('!' + i++), *TRef<IUD>(tv.value));
  }
  _AAssert(list.Count() == 64);

  _DROut("\n\nStep 2: Testing pop...");
  for (ISY i = list.Total() - 1; i >= 0; --i)
    list.Pop();

  list.Clear();
  i = 0;
  _DROut("\n\nStep 3: Testing SCKPack...");
  //for (IS j = 0; j < 3; ++j)
  //  _AAvow(count++, list.Insert(test_cha[j]));
  //_AAvow(IS(3), list.Insert(test_isc[0]));
  //_DROutOBJ(list);
  //_AAvow(IS(3), list.Insert(test_cha[3], SCKPack));
  // @todo Add more SCKPack test cases.
}
}  //< namespace CRTest
#endif

namespace CRTest {
static const CHA* List(const CHA* args) {
#if _Seam >= _CrabsList
  _ATestBegin;
  TestList<ISB, ISA, DTB>();
  TestList<ISC, ISB, DTB>();
  TestList<ISD, ISC, DTB>();
  TestList<ISD, ISD, DTB>();
#endif
  return _NilP;
}
}  //< namespace CRTest
#undef TEST_MIN
#undef TEST_MAX
