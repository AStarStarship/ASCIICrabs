// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_CLOCK
#include "../Clock.hpp"
#include "../Uniprinter.hpp"
#if SEAM == CRABS_CLOCK
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif

using namespace ::_;
namespace CRTest {

static const CHA* UUID(const CHA* args) {
#if SEAM >= CRABS_CLOCK

  D_COUT(Headingf("Testing UUID Generation and Conversion"));

  IUD hot_source_id = GenerateHotSourceID();
  D_COUT("Hot UUID Source ID (28-bit): 0x" << Hexf(hot_source_id));
  D_ASSERT(hot_source_id < (IUD(1) << 28));
  
  IUD Evergreen_source_id = GenerateEvergreenSourceID();
  D_COUT("Evergreen UUID Source ID (34-bit): 0x" << Hexf(Evergreen_source_id));
  D_ASSERT(Evergreen_source_id < (IUD(1) << 34));

  ISC seconds = 1234567;
  ISC ticks = 42;
  IUD source_id = GenerateHotSourceID();
  
  IUD hot_uuid = CreateHotUUID(seconds, ticks, source_id);
  D_COUT("Hot UUID: 0x" << Hexf(hot_uuid));
  
  TMD tmd_hot(hot_uuid);
  D_ASSERT(tmd_hot.IsHotUUID());

  IUD Evergreen_uuid = ConvertHotToEvergreenUUID(hot_uuid, Evergreen_source_id);
  D_COUT("Evergreen UUID: 0x" << Hexf(Evergreen_uuid));
  
  TMD tmd_Evergreen(Evergreen_uuid);
  D_ASSERT(tmd_Evergreen.IsEvergreenUUID());
  D_ASSERT(!tmd_Evergreen.IsHotUUID());

  IUD Evergreen_uuid2 = CreateEvergreenUUID(seconds, Evergreen_source_id);
  TMD tmd_Evergreen2(Evergreen_uuid2);
  D_ASSERT(tmd_Evergreen2.IsEvergreenUUID());

  TMD tmd_test(hot_uuid);
  IUD new_source_id = GenerateEvergreenSourceID();
  tmd_test.ConvertToEvergreenUUID(new_source_id);
  D_ASSERT(tmd_test.IsEvergreenUUID());

  D_COUT(Headingf("UUID Tests Complete! :-)\n"));

#endif
  return NILP;
}

static const CHA* Clock(const CHA* args) {
#if SEAM >= CRABS_CLOCK
  A_TEST_BEGIN;

  D_COUT(Headingf("Testing TScanTime..."));

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
    D_COUT(Linef('-') << "\n    " << i << ".) " << Strings[i] << " -> ");
    TMC t = -1;
    result = SScanTime(Strings[i], t);
    A_ASSERT(result);
    // A_ASSERT (!ClockCompare (t, 2018, 8, 9, 0, 0, 0));
  }

  D_COUT("\n\nTesting TestTimes...\n\n");

  enum { Size = 128, TestTimeCount = 3 };
  CHA socket[Size];
  const TMC TestTimes[TestTimeCount][7] = {
    { 2008,  9, 17,  4, 20,  0, 1970 },
    { 2020,  4, 20,  4, 20,  0, 1970 },
    { 2038,  1, 19, 23, 59, 59, 1970 },
  };
  for (ISN i = 0; i < TestTimeCount; ++i) {
    D_COUT("\n\n" << i << ".) Testing: " << TestTimes[i][0] << '-' << TestTimes[i][1] << '-' <<
      TestTimes[i][2] << '@' << TestTimes[i][3] << ':' << TestTimes[i][4] <<
      ':' << TestTimes[i][5] << " epoch:" << TestTimes[i][6]);

    t = ClockTimeTMC(TestTimes[i][0], TestTimes[i][1], TestTimes[i][2], 
      TestTimes[i][3], TestTimes[i][4], TestTimes[i][5], TestTimes[i][6]);
    AClock clock;
    ClockInit(clock, t);
    D_COUT("\nAClock:");
    #if D_THIS
    TClockPrint<::_::COut>(StdOut(), clock);
    #endif
    ClockPrint(socket, socket + Size, t);
    D_COUT("\n\n  Before:\"" << socket << '\"');
    result = SScanTime(socket, t_found);
    D_COUT("\n\nresult:\"" << TClock<ISC>(t_found) << "\"\"");
    A_ASSERT(ClockCompare(t_found, t));
  }

  D_COUT(Headingf("Testing invalid input"));
  TSScan<CHA>("cat", t);

  TSScan<CHA>("2017-30-40", t);

  D_COUT("\nDone testing date parsing utils! :-)\n");

  D_COUT(Headingf("Testing ClockDayToMonth..."));

  TMC year = 2021; //< Not a leap year.
  A_AVOW(ISA(MonthJanuary  ), ClockDayToMonth(DaysJanuaryStart   - 1, year));
  A_AVOW(ISA(MonthJanuary  ), ClockDayToMonth(DaysJanuaryStop    - 1, year));
  A_AVOW(ISA(MonthFebruary ), ClockDayToMonth(DaysFebruaryStart  - 1, year));
  A_AVOW(ISA(MonthFebruary ), ClockDayToMonth(DaysFebruaryStop   - 1, year));
  A_AVOW(ISA(MonthMarch    ), ClockDayToMonth(DaysMarchStart     - 1, year));
  A_AVOW(ISA(MonthMarch    ), ClockDayToMonth(DaysMarchStop      - 1, year));
  A_AVOW(ISA(MonthApril    ), ClockDayToMonth(DaysAprilStart     - 1, year));
  A_AVOW(ISA(MonthApril    ), ClockDayToMonth(DaysAprilStop      - 1, year));
  A_AVOW(ISA(MonthMay      ), ClockDayToMonth(DaysMayStart       - 1, year));
  A_AVOW(ISA(MonthMay      ), ClockDayToMonth(DaysMayStop        - 1, year));
  A_AVOW(ISA(MonthJune     ), ClockDayToMonth(DaysJuneStart      - 1, year));
  A_AVOW(ISA(MonthJune     ), ClockDayToMonth(DaysJuneStop       - 1, year));
  A_AVOW(ISA(MonthJuly     ), ClockDayToMonth(DaysJulyStart      - 1, year));
  A_AVOW(ISA(MonthJuly     ), ClockDayToMonth(DaysJulyStop       - 1, year));
  A_AVOW(ISA(MonthAugust   ), ClockDayToMonth(DaysAugustStart    - 1, year));
  A_AVOW(ISA(MonthAugust   ), ClockDayToMonth(DaysAugustStop     - 1, year));
  A_AVOW(ISA(MonthSeptember), ClockDayToMonth(DaysSeptemberStart - 1, year));
  A_AVOW(ISA(MonthSeptember), ClockDayToMonth(DaysSeptemberStop  - 1, year));
  A_AVOW(ISA(MonthOctober  ), ClockDayToMonth(DaysOctoberStart   - 1, year));
  A_AVOW(ISA(MonthOctober  ), ClockDayToMonth(DaysOctoberStop    - 1, year));
  A_AVOW(ISA(MonthNovember ), ClockDayToMonth(DaysNovemberStop   - 1, year));
  A_AVOW(ISA(MonthNovember ), ClockDayToMonth(DaysNovemberStop   - 1, year));
  A_AVOW(ISA(MonthDecember ), ClockDayToMonth(DaysDecemberStart  - 1, year));
  A_AVOW(ISA(MonthDecember ), ClockDayToMonth(DaysDecemberStop   - 1, year));

  A_AVOW(ISA(-1), ClockDayToMonth(-1, year));
  A_AVOW(ISA(-1), ClockDayToMonth(366, year));
  D_COUT("\nDone testing ClockDayToMonth! :-)\n");
#endif
  return NILP;
}
}  //< namespace CRTest
