// Copyright AStarship <https://astarship.net>.
#include "Clock.hpp"
#if SEAM >= CRABS_CLOCK
#include <ctime>
#include "Uniprinter.hpp"
#include "Random.h"
#include "Crabs.h"
#if SEAM == CRABS_CLOCK
#include "_Debug.h"
#else
#include "_Release.h"
#endif
using namespace ::_;
namespace _ {

ISC ClockEpoch() {
#if SEAM < CRABS_OP
  return AClockEpochYearInit;
#else
  //@todo fix me!
  return AClockEpochYearInit;
#endif
}

inline BOL ClockIsNotLeapYear(ISC year) {
  return year & 0x3;
}

inline BOL ClockIsLeapYear(ISC year) {
  return !(year & 0x3);
}

inline ISB ClockYear(TMC timestamp, TMC epoch) {
  return TClockYear<TMC>(timestamp, epoch);
}

inline ISA ClockSecond(TMC timestamp, TMC epoch) {
    return TClockSeconds<TMC>(timestamp);
}

inline ISA ClockMinute(TMC timestamp, TMC epoch) {
    return TClockMinutes<TMC>(timestamp);
}

inline ISA ClockHour(TMC timestamp, TMC epoch) {
    return TClockHours<TMC>(timestamp);
}

inline ISA ClockDay(TMC timestamp, TMC epoch) {
    return TClockDayOfYear<TMC>(timestamp);
}

ISA ClockDayToMonth(TMC day_of_year, TMC year) {
  if (!TClockIsLeap<TMC>(year, day_of_year)) 
    ++day_of_year;
  if (day_of_year <= 0 || day_of_year > DaysYear) {
    return -1;
  }
  if (day_of_year < DaysSeptemberStart) {
    if (day_of_year < DaysMayStart) {
      if (day_of_year < DaysMarchStart) {
        if (day_of_year < DaysFebruaryStart) {
          return MonthJanuary;
        }
        else {
          return MonthFebruary;
        }
      }
      else {
        if (day_of_year < DaysAprilStart) {
          return MonthMarch;
        }
        else {
          return MonthApril;
        }
      }
    }
    else {
      if (day_of_year < DaysJulyStart) {
        if (day_of_year < DaysJuneStart) {
          return MonthMay;
        }
        else {
          return MonthJune;
        }
      }
      else {
        if (day_of_year < DaysAugustStart) {
          return MonthJuly;
        }
        else {
          return MonthAugust;
        }
      }
    }
  }
  if (day_of_year < DaysNovemberStart) {
    if (day_of_year < DaysOctoberStart){
      return MonthSeptember;
    }
    return MonthOctober;
  }
  if (day_of_year < DaysDecemberStart) {
    return MonthNovember;
  }
  return MonthDecember;
}

inline ISA ClockMonth(TMC timestamp, TMC epoch) {
    return TClockMonth<TMC>(timestamp);
}

inline ISA ClockWeekday(TMC timestamp, TMC epoch) {
  return 0;// return TClockYear<TMC>(timestamp, epoch);
}

TMT::TMT(ISC seconds, IUC ticks) :
    ticks(ticks), seconds(seconds) {}

// Bit-level constructors: a 64-bit word unpacks into {seconds, ticks} with
// the low 32 bits as seconds and the high 32 bits as ticks. (A plain
// "*this = value" would recurse into this constructor forever, since the
// assignment needs the same implicit conversion that defines it.)
TMT::TMT(ISD value) :
    seconds(ISC(value)), ticks(IUC(value >> 32)) {}

TMT::TMT(IUD value) :
    seconds(ISC(ISD(value))), ticks(IUC(ISD(value) >> 32)) {}

SSD::SSD(ISC seconds, ISC ticker, ISC source) :
  value(ISD(seconds) << SSDHotSecondsBit0 | ISD(ticker) << SSDHotTickerBit0 | ISD(source))
{}

SSD::SSD(IUD value) :
  value(ISD(value)) {}

SSD::SSD(ISD value) :
  value(value) {}

ISC SSD::Seconds() {
  ISD mask = (1 << (SSDHotSecondsBitCount - 1)) - 1;
  return (this->value >> SSDHotSecondsBit0) & mask;
}

ISC SSD::Ticker() { 
  return (this->value >> SSDHotTickerBit0) & ((1 << SSDHotTickerBitCount) - 1);
}

ISC SSD::Source() { return this->value & ((1 << SSDHotSourceBitCount) - 1); }

/* Checks if the TMD is a Hot UUID.
Hot UUID have MSb = 1 and bits 35:34 = 0b00 (ticks < 192). */
BOL SSD::IsHot() {
  ISD ssd = this->value;
  return ssd < 0 && (((ssd >> SSDEvergreenBit0) & SSDEvergreenBitMaskLSB) == 0);
}

/* Checks if the TMD is a Cold UUID.
Cold UUID have MSb = 0. */
BOL SSD::IsCold() { return this->value >= 0; }

/* Checks if the TMD is an Evergreen UUID.
Evergreen UUID have MSb = 1 and bits 35:34 = 1. */
BOL SSD::IsEvergreen() {
	ISD ssd = this->value;
  return ssd < 0 && (((ssd >> SSDEvergreenBit0) & SSDEvergreenBitMaskLSB) ==
    SSDEvergreenBitMaskLSB);
}

/* Converts a Hot UUID to a Cold UUID.
Cold UUIDs carry the same field layout as Hot UUIDs with MSb = 0, so the
conversion clears the sign bit and leaves seconds, ticker, and source
untouched. The ticker parameter is reserved for future Cold layouts.
@return The Cold UUID as ISD. */
ISD SSD::ToCold(ISC ticker) const {
  (void)ticker;
  return this->value & ~(ISD(1) << 63);
}

/* Converts Hot UUID to Evergreen UUID.
Hot UUID: MSb=1, [27-bit seconds][8-bit ticks][28-bit id]
Evergreen UUID: MSb=1, bit35=1, bit34=1, [27-bit seconds][34-bit id]
@param hot_uuid The Hot UUID 64-bit value
@param source_id The random source ID (will be masked to 34-bit)
@return Evergreen UUID as IUD */
IUD ClockHotToEvergreen(IUD hot_uuid, IUD source_id) {
  ISC seconds = ISC((hot_uuid >> SSDHotSecondsBit0) & ((ISD(1) << 27) - 1));
  ISC ticks = ISC((hot_uuid >> SSDHotTickerBit0) & (SSDHotTickerBitCount - 1));
  // Build Evergreen UUID:
  // b63 (MSb): 1
  // b62:36: 27-bit seconds
  // b35:34  0b11
  // b33:0: 34-bit source
  
  IUD Evergreen = 0;
  Evergreen |= (IUD(1) << 63) | (IUD(3) << 34) |
               (IUD(seconds & ((ISD(1) << 27) - 1)) << SSDEvergreenSecondsBit0);
  
  // Set 34-bit source_id at bits 33:0
  Evergreen |= (source_id & ((IUD(1) << 34) - 1));
  
  return Evergreen;
}

/* Generates a random 28-bit source ID for Hot UUID using RNG module.
@return Random 28-bit source ID. */
IUD GenerateHotSourceID() {
  // Generate 64-bit random number and mask to 28 bits
  IUD random_high = IUD(IUCRandom()) << 32;
  IUD random_low = IUD(IUCRandom());
  IUD random_value = random_high | random_low;
  
  // Mask to 28 bits (keep only lower 28 bits)
  return random_value & ((IUD(1) << 28) - 1);
}

/* Generates a random 34-bit source ID for Evergreen UUID using RNG module.
@return Random 34-bit source ID. */
IUD GenerateEvergreenSourceID() {
  // Generate 64-bit random number and mask to 34 bits
  IUD random_high = IUD(IUCRandom()) << 32;
  IUD random_low = IUD(IUCRandom());
  IUD random_value = random_high | random_low;
  
  // Mask to 34 bits (keep only lower 34 bits)
  return random_value & ((IUD(1) << 34) - 1);
}

/* Creates a Hot UUID from seconds, ticks, and source_id.
@param seconds The 27-bit seconds timestamp
@param ticks The 8-bit subsecond ticker (max 191)
@param source_id The 28-bit random source ID
@return Hot UUID as IUD */
IUD CreateHotUUID(ISC seconds, ISC ticks, IUD source_id) {
  // Validate inputs
  D_ASSERT(seconds >= 0 && seconds < (ISD(1) << 27));
  D_ASSERT(ticks >= 0 && ticks < 192);  // Max 192 ticks per second
  D_ASSERT(source_id < (IUD(1) << 28));
  
  IUD hot_uuid = 0;
  
  // Set MSb = 1 (Hot UUID indicator)
  hot_uuid |= (IUD(1) << 63);
  
  // Set 27-bit seconds
  hot_uuid |= IUD(seconds & ((ISD(1) << 27) - 1)) << SSDHotSecondsBit0;
  
  // Set 8-bit ticks
  hot_uuid |= IUD(ticks & ((1 << SSDHotTickerBitCount) - 1)) << SSDHotTickerBit0;
  
  // Set 28-bit source_id
  hot_uuid |= source_id & ((IUD(1) << 28) - 1);
  
  return hot_uuid;
}

/* Creates an Evergreen UUID from seconds and source_id.
@param seconds The 27-bit seconds timestamp  
@param source_id The 34-bit random source ID
@return Evergreen UUID as IUD */
IUD CreateEvergreenUUID(ISC seconds, IUD source_id) {
  // Validate inputs
  D_ASSERT(seconds >= 0 && seconds < (ISD(1) << 27));
  D_ASSERT(source_id < (IUD(1) << 34));
  
  IUD Evergreen = 0;
  
  // Set MSb = 1 (Evergreen UUID indicator)
  Evergreen |= IUD(1) << 63;
  
  // Set bit 35 = 1
  Evergreen |= IUD(1) << 35;
  
  // Set bit 34 = 1
  Evergreen |= IUD(1) << 34;
  
  // Set 27-bit seconds at bits 62:36
  Evergreen |= IUD(seconds & ((ISD(1) << 27) - 1)) << 36;
  
  // Set 34-bit source_id at bits 33:0
  Evergreen |= source_id & ((IUD(1) << 34) - 1);
  
  return Evergreen;
}

SSE::SSE(ISD seconds, ISD ticks, ISD source_msb, ISD source_lsb) :
  msb(seconds << SSESecondsMSBit0 | ticks << SSETickerMSBit0 | source_msb),
  lsb(source_lsb)
{}

ISC SSE::Seconds() { return ISC(this->msb >> SSESecondsMSBit0); }

ISC SSE::Ticks() { 
  return ISC((this->msb >> SSETickerMSBit0) & (SSETickerBitCount - 1));
}

ISC SSE::IdMSB() { return ISC(this->msb & (SSESourceBitCount - 1)); }

ISD SSE::IdLSB() { return this->lsb; }

const ISB* ClockLastDayOfMonth() {
  static const ISB LastDayOfMonth[12] = {31,  59,  90,  120, 151, 181,
                                         212, 243, 273, 304, 334, 365};
  return LastDayOfMonth;
}

const ISB* ClockLastDayOfMonthLeapYear() {
  static const ISB LastDayOfMonthLeapYear[12] = {31,  60,  91,  121, 152, 182,
                                                 213, 244, 274, 305, 335, 366};
  return LastDayOfMonthLeapYear;
}

ISN MonthByDay(ISN day, ISN year) {
  const ISB *cursor, *stop;
  if (day < 1) return 0;
  if (year & 3) {  // 3 = 0b'11 which is a much faster way to do modulo 4.
    cursor = ClockLastDayOfMonthLeapYear();
  } else {
    cursor = ClockLastDayOfMonth();
  }
  stop = cursor + 11;
  while (cursor <= stop)
    if (day < *cursor++) return (ISN)(stop - cursor);
  return 0;
}

AClock* ClockInit(AClock& clock, TMC t, TMC epoch) {
  return TClockInit<TMC>(clock, t, epoch);
}

AClock* ClockInit(AClock& clock, TMD t, TMC epoch) {
  return TClockInit<TMD>(clock, t, epoch);
}

TMT& ClockInit(TMT& timestamp, ISC t, IUC ticks) {
  timestamp.seconds = t;
  timestamp.ticks = ticks;
  return timestamp;
}

AClock* ClockInit(AClock& clock) {
  time_t t;
  time(&t);
  return TClockInit<time_t>(clock, t);
}

inline void ClockEpochUpdate() {
  // RoomLock();
  // ClockEpochInit += AClockEpoch;
  // RoomUnlock();
}

TMD ClockNow() {
  time_t t;
  time(&t);
  // 64-bit seconds timestamp has no epoch
  //if (t > ClockEpoch()) ClockEpochUpdate();
  return TMD(t);
}

TMC ClockToTimestamp(AClock& clock) {
  return (clock.year - ClockEpoch()) * SecondsPerYear +
         (clock.day - 1) * SecondsPerDay + clock.hour * SecondsPerHour +
         clock.minute * SecondsPerMinute + clock.second;
}

//ISC ClockISC(AClock& clock) { return ClockSeconds(clock); }
//
//ISD ClockISD(AClock& clock) { return ISD(ClockSeconds(clock)); }

ISN ClockMonthDayCount(ISC t) {
  TClock<ISC> date(t);
  static const CHA days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31};
  AClock& clock = date.clock;
  if ((clock.year & 3) && (clock.month == 4)) {
    // Then it's a leap year and April:
    return 29;
  }
  return days_per_month[clock.month];
}

ISN ClockMonthDayCount(ISN month, ISN year) {
  if (month < 1) return 0;
  if (month > 12) return 0;
  static const CHA days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31};
  if ((year & 0xC) && (month == 4))  // Then it's a leap year and April:
    return 29;
  return days_per_month[month];
}

const CHA* ClockWeekDay(ISN day_number) {
  static const CHA* days[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                              "Thursday", "Friday", "Saturday"};
  static const CHA cInvalidText[] = "Invalid\0";
  if (day_number < 0) {
    return cInvalidText;
  }
  if (day_number >= 7) {
    return cInvalidText;
  }
  return days[day_number];
}

CHA ClockDayOfWeekInitial(ISN day_number) {
  static const CHA days[] = "SMTWRFSU";
  if (day_number < 0) return 'I';
  if (day_number >= 7) return 'I';
  return days[day_number];
}

ISN ClockCompare(const AClock& date_a, const AClock& date_b) {
  D_COUT("\n    Expecting Date:" << date_a << " and found:" << date_b);

  if (date_a.year != date_b.year) {
    D_COUT("\n    year.a:" << date_a.year + ClockEpoch()
                           << " != year.b:" << date_b.year + ClockEpoch());
    return 1;
  }
  if (date_a.month != date_b.month) {
    D_COUT("\n    month.a:" << date_a.month
                            << " != month.b:" << date_b.month + 1 << " ");
    return 2;
  }
  if (date_a.day != date_b.day) {
    D_COUT("\n    day.a:" << date_a.day << " != day.b:" << date_b.day << " ");
    return 3;
  }
  if (date_a.hour != date_b.hour) {
    D_COUT("\n    hour.a:" << date_a.hour << " != hour.b:" << date_b.hour
                           << " ");
    return 4;
  }
  if (date_a.minute != date_b.minute) {
    D_COUT("\n    minute.a:" << date_a.minute
                             << " != minute.b:" << date_b.minute);
    return 5;
  }
  if (date_a.second != date_b.second) {
    D_COUT("\n    second.a:" << date_a.second
                             << " != second.b:" << date_b.second);
    return 6;
  }
  return 0;
}

ISN ClockCompare(ISC time_a, ISC time_b) {
  AClock a, b;
  ClockInit(a, time_a);
  ClockInit(b, time_b);
  ISN result = ClockCompare(a, b);
  D_COUT("\n  Comparing time_a:" << time_a << " to time_b:" << time_b
                                 << " with result:" << result);
  return result;
}

ISN ClockCompare(const AClock& clock, ISN year, ISN month, ISN day,
                 ISN hour = 0, ISN minute = 0, ISN second = 0) {
  D_COUT("\n    Expecting " << year << '/"' << month << '/"' << day << '@'
                            << hour << ':' << minute << ':' << second
                            << " and found " << clock);

  if (year - ClockEpoch() != clock.year) {
    D_COUT("\n    Expecting year:" << year << " but found:"
                                   << clock.year + ClockEpoch() << '.');
    return 1;
  }
  if (month != clock.month) {
    D_COUT("\n    Expecting month:" << month << " but found:" << clock.month
                                    << '.');
    return 2;
  }
  if (day != clock.day) {
    D_COUT("\n    Expecting day:" << day << " but found:" << clock.day << '.');
    return 3;
  }
  if (hour != clock.hour) {
    D_COUT("\n    Expecting hour:" << hour << " but found:" << clock.hour
                                   << '.');
    return 4;
  }
  if (minute != clock.minute) {
    D_COUT("\n    Expecting minute:" << minute << " but found:" << clock.minute
                                     << '.');
    return 5;
  }
  if (second != clock.second) {
    D_COUT("\n    Expecting second:" << second << " but found:" << clock.second
                                     << '.');
    return 6;
  }
  return 0;
}

inline void ClockZeroTime(AClock& local_time) {
  local_time.second = 0;
  local_time.minute = 0;
  local_time.hour   = 0;
  local_time.day    = 0;
  local_time.month  = 0;
  local_time.year   = 0;
}

inline TMC TimeMake(AClock& time) { 
  return TMC(mktime(TPtr<tm>(&time)));
}

inline const ISA* ClockDaysMonth() {
  static const ISA DaysMonthArray[12] = {31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31};
  return DaysMonthArray;
}

inline ISN ClockDaysMonth(ISN month, ISN year) {
  if ((year & 3) == 0) {
    if (month == 4) return 29;
  }
  if (month < 1 || month > 12) return 0;
  return ClockDaysMonth()[month - 1];
}

inline ISN ClockDayOfYear(ISN year, ISN month, ISN day) {
  if (day < 1 || day > ClockDaysMonth(month, year) || month < 1 || month > 12)
    return 0;
  if (month == 1) {
    return day;
  }
  if (year & 3) return ClockLastDayOfMonth()[month - 2] + day;
  return ClockLastDayOfMonthLeapYear()[month - 2] + day;
}

inline TMC ClockTimeTMC(ISN year, ISN month, ISN day, ISN hour, ISN minute,
                        ISN second, TMC epoch) {
  return TClockTimestamp<ISC>(year, month, day, hour, minute, second);
}

inline TMD ClockTimeTMD(ISN year, ISN month, ISN day, ISN hour, ISN minute,
                        ISN second, TMC epoch) {
  return TClockTimestamp<ISC>(year, month, day, hour, minute, second);
}

#if USING_STA == YES_0
inline CHA* SPrint(CHA* origin, CHA* stop, const AClock& clock) {
  return TSPrint<CHA>(origin, stop, clock);
}

inline CHA* SPrint(CHA* origin, CHA* stop, const TMT& t) {
  return TSPrint<CHA>(origin, stop, t);
}

inline CHA* ClockPrint(CHA* origin, CHA* stop, ISC t) {
  AClock clock;
  ClockInit(clock, t);
  return TSPrint<CHA>(origin, stop, clock);
}

inline CHA* ClockPrint(CHA* origin, CHA* stop, ISD t) {
  AClock clock;
  ClockInit(clock, t);
  return TSPrint<CHA>(origin, stop, clock);
}

inline const CHA* ScanTime(const CHA* string, ISN& hour, ISN& minute, ISN& second) {
  return TSScan<CHA>(string, hour, minute, second);
}

inline const CHA* SScan(const CHA* string, AClock& clock) {
  return TSScan<CHA>(string, clock);
}

inline const CHA* SScan(const CHA* string, TMT& t) { 
  return TSScan<CHA>(string, t); 
}

inline const CHA* SScanTime(const CHA* string, TMC& t) {
  return TSScan<CHA>(string, t);
}

inline const CHA* SScanTime(const CHA* string, TMD& t) {
  return TSScan<CHA, CHC, TMD>(string, t);
}

#endif
#if USING_STB == YES_0

CHB* SPrint(CHB* origin, CHB* stop, const AClock& clock) {
  return TSPrint<CHB>(origin, stop, clock);
}

CHB* SPrint(CHB* origin, CHB* stop, const TMT& t) {
  return TSPrint<CHB>(origin, stop, t);
}

CHB* ClockPrint(CHB* origin, CHB* stop, TMC t) {
  return TClockSPrint<CHB, TMC>(origin, stop, t);
}

CHB* ClockPrint(CHB* origin, CHB* stop, TMD t) {
  return TClockSPrint<CHB, TMD>(origin, stop, t);
}

CHC* SPrint(CHC* origin, CHC* stop, const AClock& clock) {
  return TSPrint<CHC>(origin, stop, clock);
}

CHC* SPrint(CHC* origin, CHC* stop, const TMT& t) {
  return TSPrint<CHC>(origin, stop, t);
}

CHC* ClockPrint(CHC* origin, CHC* stop, ISC t) {
  return TClockSPrint<CHC, ISC>(origin, stop, t);
}

CHC* ClockPrint(CHC* origin, CHC* stop, ISD t) {
  return TClockSPrint<CHC, ISD>(origin, stop, t);
}

inline const CHB* SScan(const CHB* string, AClock& clock) {
  return TSScan<CHB>(string, clock);
}

inline const CHB* SScan(const CHB* string, SSD& result) {
  return NILP;// TSScan<CHB>(string, result);
}

inline const CHB* SScan(const CHB* string, SSE& result) {
  return NILP;// TSScan<CHB>(string, result);
}

inline const CHB* SScan(const CHB* string, TMT& result) {
  return TSScan<CHB>(string, result);
}

inline const CHB* ScanTime(const CHB* string, ISN& hour, ISN& minute, ISN& second) {
  return TSScan<CHB>(string, hour, minute, second);
}

inline const CHB* ScanTime(const CHB* string, ISC& result) {
  return TSScan<CHB, CHC, ISC>(string, result);
}

inline const CHB* ScanTime(const CHB* string, ISD& result) {
  return TSScan<CHB, CHC, ISD>(string, result);
}
#endif

#if USING_STC == YES_0

inline CHC* SPrint(CHC* origin, CHC* stop, AClock& clock) {
  return TSPrint<CHC>(origin, stop, clock);
}

inline CHC* SPrint(CHC* origin, CHC* stop, ISC& t) {
  return TSPrint<CHC>(origin, stop, t);
}

inline const CHC* ScanTime(const CHC* string, ISN& hour, ISN& minute, ISN& second) {
  return TSScan<CHC>(string, hour, minute, second);
}

inline const CHC* SScan(const CHC* string, AClock& time) {
  return TSScan<CHC>(string, time);
}

inline const CHC* SScan(const CHC* string, TMT& result) {
  return TSScan<CHC>(string, result);
}

inline const CHC* ScanTime(const CHC* string, ISC& result) {
  return TSScan<CHC, CHC, ISC>(string, result);
}

inline const CHC* ScanTime(const CHC* string, ISD& result) {
  return TSScan<CHC, CHC, ISD>(string, result);
}

#endif

}  //< namespace _

#endif  //< #if SEAM >= CRABS_CLOCK
