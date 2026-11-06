// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsClock
#include "../Clock.hpp"
#include "../Uniprinter.hpp"
#if _Seam == _CrabsClock
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

static const CHA* UUID(const CHA* args) {
#if _Seam >= _CrabsClock

  _DROut(Headingf("Testing UUID Generation and Conversion"));

  IUD hot_source_id = GenerateHotSourceID();
  _DROut("Hot UUID Source ID (28-bit): 0x" << Hexf(hot_source_id));
  _AAssert(hot_source_id < (IUD(1) << 28));

  IUD Evergreen_source_id = GenerateEvergreenSourceID();
  _DROut("Evergreen UUID Source ID (34-bit): 0x" << Hexf(Evergreen_source_id));
  _AAssert(Evergreen_source_id < (IUD(1) << 34));

  ISC seconds = 1234567;
  ISC ticks = 42;
  IUD source_id = GenerateHotSourceID();

  IUD hot_uuid = CreateHotUUID(seconds, ticks, source_id);
  _DROut("Hot UUID: 0x" << Hexf(hot_uuid));
  {
    SSD s_hot(hot_uuid);
    _AAssert(s_hot.IsHot());
    _AAssert(!s_hot.IsEvergreen());
    _AAssert(s_hot.Source() == source_id);
    _AAssert(s_hot.Ticker() == ticks);
  }

  IUD Evergreen_uuid = ClockHotToEvergreen(hot_uuid, Evergreen_source_id);
  _DROut("Evergreen UUID: 0x" << Hexf(Evergreen_uuid));
  {
    SSD s_ever(Evergreen_uuid);
    _AAssert(s_ever.IsEvergreen());
    _AAssert(!s_ever.IsHot());
  }

  IUD Evergreen_uuid2 = CreateEvergreenUUID(seconds, Evergreen_source_id);
  {
    SSD s_ever2(Evergreen_uuid2);
    _AAssert(s_ever2.IsEvergreen());
    _AAssert(!s_ever2.IsHot());
  }

  // Converting an existing Hot TMD to Evergreen must flip the marker bits.
  TMD tmd_test = hot_uuid;
  IUD new_source_id = GenerateEvergreenSourceID();
  IUD converted = ClockHotToEvergreen(tmd_test, new_source_id);
  {
    SSD s_converted(converted);
    _AAssert(s_converted.IsEvergreen());
    _AAssert(!s_converted.IsHot());
  }

  // A Hot UUID converted to Cold keeps its fields and only clears the MSb.
  {
    SSD s_hot2(hot_uuid);
    IUD cold_uuid = s_hot2.ToCold(0);
    SSD s_cold(cold_uuid);
    _AAssert(s_cold.IsCold());
    _AAssert(!s_cold.IsHot());
    _AAssert(!s_cold.IsEvergreen());
    _AAssert(s_cold.Ticker() == ticks);
    _AAssert(s_cold.Source() == source_id);
  }

  _DROut(Headingf("UUID Tests Complete! :-)"));

#endif
  return _NilP;
}

static const CHA* Clock(const CHA* args) {
#if _Seam >= _CrabsClock
  _ATestBegin;

  UUID(args);

  _DROut(Headingf("Testing TScanTime..."));

  ISC t, t_found;
  const CHA* result;

  // @note The following dates must be the current day to work right in order
  // to auto-detect the year.
  const CHA* Strings[] = {
    "1/19",
    "01/19",
    "1/9/19",
    "1/19/19",
    "1/9/2019",
    "1/19/2019",
    "1/19/2019",
    "01/19/2019",
    "1/19/2019@00",
    "1.19.2019@00AM",
    "1/19/2019@00:00",
    "1/19/19@00:0AM",
    "1/19/2019@00:00:00",
    "1/19/2019@00:00:00AM",
    "2019-01-19@00:00:00AM",
    "2019-01-19@00:00:00am",
    "2019-01-19@00:00:00A",
    "2019-01-19@00:00:00a ",
  };

  for (TMC i = 0; i < 18; ++i) {
    _DROut(Linef('-') << "\n    " << i << ".) " << Strings[i] << " -> ");
    TMC t = -1;
    result = SScanTime(Strings[i], t);
    _AAssert(result);
    // _AAssert (!ClockCompare (t, 2018, 8, 9, 0, 0, 0));
  }

  _DROut("\n\nTesting TestTimes...\n\n");

  enum { Size = 128, TestTimeCount = 3 };
  CHA socket[Size];
  const TMC TestTimes[TestTimeCount][7] = {
    { 2008,  9, 17,  4, 20,  0, 1970 },
    { 2020,  4, 20,  4, 20,  0, 1970 },
    { 2038,  1, 19, 23, 59, 59, 1970 },
  };
  for (ISN i = 0; i < TestTimeCount; ++i) {
    _DROut("\n\n" << i << ".) Testing: " << TestTimes[i][0] << '-' << TestTimes[i][1] << '-' <<
      TestTimes[i][2] << '@' << TestTimes[i][3] << ':' << TestTimes[i][4] <<
      ':' << TestTimes[i][5] << " epoch:" << TestTimes[i][6]);

    t = ClockTimeTMC(TestTimes[i][0], TestTimes[i][1], TestTimes[i][2], 
      TestTimes[i][3], TestTimes[i][4], TestTimes[i][5], TestTimes[i][6]);
    AClock clock;
    ClockInit(clock, t);
    _DROut("\nAClock:");
    #if _DThis
    TClockPrint<::_::ROut>(RefOut(), clock);
    #endif
    ClockPrint(socket, socket + Size, t);
    _DROut("\n\n  Before:\"" << socket << '\"');
    result = SScanTime(socket, t_found);
    _DROut("\n\nresult:\"" << TClock<ISC>(t_found) << "\"\"");
    _AAssert(ClockCompare(t_found, t));
  }

  _DROut(Headingf("Testing invalid input"));
  TSScan<CHA>("cat", t);

  TSScan<CHA>("2017-30-40", t);

  _DROut("\nDone testing date parsing utils! :-)\n");

  _DROut(Headingf("Testing ClockDayToMonth..."));

  TMC year = 2021; //< Not a leap year.
  _AAvow(ISA(MonthJanuary  ), ClockDayToMonth(DaysJanuaryStart   - 1, year));
  _AAvow(ISA(MonthJanuary  ), ClockDayToMonth(DaysJanuaryStop    - 1, year));
  _AAvow(ISA(MonthFebruary ), ClockDayToMonth(DaysFebruaryStart  - 1, year));
  _AAvow(ISA(MonthFebruary ), ClockDayToMonth(DaysFebruaryStop   - 1, year));
  _AAvow(ISA(MonthMarch    ), ClockDayToMonth(DaysMarchStart     - 1, year));
  _AAvow(ISA(MonthMarch    ), ClockDayToMonth(DaysMarchStop      - 1, year));
  _AAvow(ISA(MonthApril    ), ClockDayToMonth(DaysAprilStart     - 1, year));
  _AAvow(ISA(MonthApril    ), ClockDayToMonth(DaysAprilStop      - 1, year));
  _AAvow(ISA(MonthMay      ), ClockDayToMonth(DaysMayStart       - 1, year));
  _AAvow(ISA(MonthMay      ), ClockDayToMonth(DaysMayStop        - 1, year));
  _AAvow(ISA(MonthJune     ), ClockDayToMonth(DaysJuneStart      - 1, year));
  _AAvow(ISA(MonthJune     ), ClockDayToMonth(DaysJuneStop       - 1, year));
  _AAvow(ISA(MonthJuly     ), ClockDayToMonth(DaysJulyStart      - 1, year));
  _AAvow(ISA(MonthJuly     ), ClockDayToMonth(DaysJulyStop       - 1, year));
  _AAvow(ISA(MonthAugust   ), ClockDayToMonth(DaysAugustStart    - 1, year));
  _AAvow(ISA(MonthAugust   ), ClockDayToMonth(DaysAugustStop     - 1, year));
  _AAvow(ISA(MonthSeptember), ClockDayToMonth(DaysSeptemberStart - 1, year));
  _AAvow(ISA(MonthSeptember), ClockDayToMonth(DaysSeptemberStop  - 1, year));
  _AAvow(ISA(MonthOctober  ), ClockDayToMonth(DaysOctoberStart   - 1, year));
  _AAvow(ISA(MonthOctober  ), ClockDayToMonth(DaysOctoberStop    - 1, year));
  _AAvow(ISA(MonthNovember ), ClockDayToMonth(DaysNovemberStop   - 1, year));
  _AAvow(ISA(MonthNovember ), ClockDayToMonth(DaysNovemberStop   - 1, year));
  _AAvow(ISA(MonthDecember ), ClockDayToMonth(DaysDecemberStart  - 1, year));
  _AAvow(ISA(MonthDecember ), ClockDayToMonth(DaysDecemberStop   - 1, year));

  _AAvow(ISA(-1), ClockDayToMonth(-1, year));
  _AAvow(ISA(-1), ClockDayToMonth(366, year));
  _DROut("\nDone testing ClockDayToMonth! :-)\n");
#endif
  return _NilP;
}
}  //< namespace CRTest
