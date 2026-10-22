// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
//
#include "../_Package.hxx"
//
#include "01.COut.hxx"
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
#include "15.Dic.hxx"
#include "16.CrabsOp.hxx"
#include "17.Room.hxx"
#include "18.MathCore.hxx"
#include "19.Random.hxx"
#include "20.File.hxx"
#include "21.Promise.hxx"
#include "22.Script.hxx"
#include "23.Release.hxx"
//
#include "../Test.hpp"
using namespace ::_;

inline const CHA* CrabsTests(const CHA* args) {
  return TTestTree<CRTest::Core, CRTest::ItoS, CRTest::FtoS, 
    CRTest::Uniprinter, CRTest::Clock, CRTest::Stack, CRTest::String, 
    CRTest::Loom,  CRTest::List, CRTest::Book, CRTest::Map, CRTest::Table,
    CRTest::Dic, CRTest::File, CRTest::CrabsOp, CRTest::Room, CRTest::MathCore, 
    CRTest::RNG, CRTest::Promise, CRTest::CrabsScript, CRTest::Regex>(args);
}
