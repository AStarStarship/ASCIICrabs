// Copyright AStarship <https://astarship.net>.
#include "pch.h"
#define _CrabsCore          1
#define _CrabsIToS          2
#define _CrabsFToS          3
#define _CrabsUniprinter    4
#define _CrabsClock         5
#define _CrabsArray         6
#define _CrabsStack         7
#define _CrabsString        8
#define _CrabsRegEx         9
#define _CrabsLoom          10
#define _CrabsList          11
#define _CrabsBook          12
#define _CrabsMap           13
#define _CrabsTable         14
#define _CrabsDictionary    15
#define _CrabsOp            16
#define _CrabsRoom          17
#define _CrabsMathCore      18
#define _CrabsRandom        19
#define _CrabsFile          20
#define _CrabsPromise       21
#define _CrabsScript        22
#define _CrabsPassword      23
#define _CrabsRelease       24
#define _SeamN _CrabsRelease
#include <_Config.h>
#include <_Package.hxx>
#include "01.Core.hxx"
#include "02.ItoS.hxx"
#include "03.FtoS.hxx"
#include "04.Uniprinter.hxx"
#include "05.Clock.hxx"
#include "06.Array.hxx"
#include "07.Stack.hxx"
#include "08.String.hxx"
#include "09.Regex.hxx"
#include "10.Loom.hxx"
#include "11.List.hxx"
#include "12.Book.hxx"
#include "13.Map.hxx"
#include "14.Table.hxx"
#include "15.Dictionary.hxx"
#include "16.CrabsOp.hxx"
#include "17.Room.hxx"
#include "18.MathCore.hxx"
#include "19.Random.hxx"
#include "20.File.hxx"
#include "21.Promise.hxx"
#include "22.Script.hxx"
#include "23.Password.hxx"
#include "24.Release.hxx"

using namespace ::_;
using namespace CRTest;

ISN main(ISN arg_count, CHA** args) {
  const CHA* argss = ArgsToString(arg_count, args);
#if _Seam == _CrabsRelease
  return SeamResult(Release(argss));
#else
  return TTestTree<Core, ItoS, FtoS, Uniprinter, Clock, Array, Stack, String,
    Regex, Loom,  List, Book, Map, Table, Dictionary, CrabsOp, Room, MathCore, Random,
    File, Promise, CrabsScript, Password
  >(args);
#endif // _Seam == _CrabsRelease
}
