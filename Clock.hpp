// Copyright AStarship <https://astarship.net>.
#pragma once
#include "Clock.h"
#if SEAM >= CRABS_CLOCK
#ifndef CRABS_CLOCK_HPP
#define CRABS_CLOCK_HPP
#if SEAM == CRABS_COUT
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

// Returns the year of the given timestamp.
template<typename TM = TMC>
inline ISB TClockYear(TM timestamp, TM epoch) {
  return ISB(timestamp / SecondsPerYear - epoch);
}

// Returns the number of seconds since the begining of the year.
template<typename TM = TMC>
inline TM TClockSubyearSeconds(TM timestamp) {
  return timestamp % SecondsPerYear;
}

// Returns true if the year is a leap year.
template<typename TM = TMC>
inline BOL TClockIsLeapYear(TM year) {
  return !(year & 0x3);
}

// Returns true if the day_of_year is after the leap day.
template<typename TM = TMC>
inline BOL TClockIsLeapDay(TM day_of_year) {
  return day_of_year > DayLeap;
}

// Returns true if the day_of_year is after the leap day of a leap year.
template<typename TM = TMC>
inline BOL TClockIsLeap(TM year, TM day_of_year) {
  return TClockIsLeapYear<TM>(year) && TClockIsLeapDay(day_of_year);
}

template<typename TM = TMC>
inline ISA TClockDayOfYearSubyear(TM subyear_seconds) {
  return ISA(subyear_seconds / SecondsPerDay);
}

template<typename TM = TMC>
inline ISA TClockDayOfYear(TM timestamp) {
	return TClockDayOfYearSubyear<TM>(TClockSubyearSeconds<TM>(timestamp));
}

template<typename TM = TMC>
inline ISA TClockSubdaySecondsSubyear(TM subyear_seconds) {
	return subyear_seconds % SecondsPerDay;
}

template<typename TM = TMC>
inline ISA TClockSubdaySeconds(TM timestamp) {
	return TClockSubdaySecondsSubyear<TM>(TClockSubyearSeconds<TM>(timestamp));
}

template<typename TM = TMC>
inline ISA TClockMonth(TM timestamp) {
  TMC year = timestamp / SecondsPerYear,
      subyear_seconds = timestamp - year * SecondsPerYear;
  return ClockDayToMonth(TMC(subyear_seconds / SecondsPerDay), year);
}

template<typename TM = TMC>
inline ISA TClockHourSubyear(TM subyear_seconds) {
  return TClockSubdaySecondsSubyear<TM>(subyear_seconds) / SecondsPerHour;
}

template<typename TM = TMC>
inline ISA TClockHours(TM timestamp) {
  return TClockSubdaySeconds<TM>(timestamp) / SecondsPerHour;
}

template<typename TM = TMC>
inline ISA TClockMinutes(TM timestamp) {
	return TClockSubdaySeconds<TM>(timestamp) % SecondsPerHour;
}

template<typename TM = TMC>
inline ISA TClockSeconds(TM timestamp) {
  return TClockSubdaySeconds<TM>(timestamp) % SecondsPerMinute;
}

template<typename TM = TMC>
ISA TClockWeekday(TMC timestamp, TMC epoch) {
    return 0;
}

/* Initializes the clock from the given timestamp. */
template<typename IS>
AClock* TClockInit(AClock& clock, IS timestamp, ISC epoch = ClockEpoch()) {
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  IS value = timestamp / SecondsPerYear;
  //if (value >= epoch) {
  //  value -= epoch;
  //}
  timestamp -= value * SecondsPerYear;
  ISB year = ISB(value + epoch);
  clock.year = year;
  value = timestamp / SecondsPerDay;
  timestamp -= value * SecondsPerDay;
  clock.day = ISA(value);
  clock.month = ClockDayToMonth(TMC(value), year);
  value = timestamp / SecondsPerHour;
  timestamp -= value * SecondsPerHour;
  clock.hour = ISA(value);
  value = timestamp / SecondsPerMinute;
  clock.minute = ISA(value);
  clock.second = ISA(timestamp - value * SecondsPerMinute);
  clock.weekday = -1;
  return &clock;
}

/* Creates a Unix timestamp with the given epoch.
@note */
template<typename IS>
IS TClockTimestamp(ISN year, ISN month, ISN day, ISN hour, ISN minute, 
    ISN second, ISC epoch = ClockEpoch()) {
  if (month < 0 || month >= 12 || hour < 0 || hour >= 24 || minute < 0 || 
      minute >= 60 || second < 0 || second >= 60)
    return 0;
  return IS((year - epoch) * SecondsPerYear +
            ClockDayOfYear(year, month, day) * SecondsPerDay +
            hour * SecondsPerHour + minute * SecondsPerMinute + second);
}

/*
template<typename CHS = CHR, typename CHT = CHD>
CHS* Print(CHS* cursor, CHS* stop, TME& t) {
  AClock c (t.seconds);
  cursor = SPrint(cursor, stop, c);
  cursor = SPrint(cursor, stop, ':');
  return SPrint(cursor, stop, t.ticks);
}*/

#if USING_STR

template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrint(CHS* cursor, CHS* stop, const AClock& clock, 
    TMC epoch = ClockEpoch()) {
  // The way the utf functions are setup, we return a nil-term CHA so we
  // don't have to check to write a single CHA in this
  A_ASSERT(cursor);
  A_ASSERT(cursor < stop);

  cursor = SPrint(cursor, stop, ISC(clock.year) - epoch);
  if (IsError(cursor)) 
    return NILP;
  *cursor++ = '-';
  cursor = SPrint(cursor, stop, clock.month + 1);
  if (IsError(cursor))
    return NILP;
  *cursor++ = '-';
  cursor = SPrint(cursor, stop, clock.day);
  if (IsError(cursor))
    return NILP;
  *cursor++ = '@';
  cursor = SPrint(cursor, stop, clock.hour);
  if (IsError(cursor))
    return NILP;
  *cursor++ = ':';
  cursor = SPrint(cursor, stop, clock.minute);
  if (IsError(cursor))
    return NILP;
  *cursor++ = ':';
  cursor = SPrint(cursor, stop, clock.second);
  if (IsError(cursor))
    return NILP;
  return cursor;
}

template<typename Printer>
Printer& TClockPrint(Printer& o, const ::_::AClock& clock) {
  return o << clock.year /* + ClockEpoch() */ << '-' << clock.month + 1 << '-'
           << clock.day << '@' << clock.hour << ':' << clock.minute << ':'
           << clock.second;
}

template<typename CHS = CHR, typename CHT = CHD, typename IS = ISD>
CHS* TClockSPrint(CHS* cursor, CHS* stop, IS t) {
  AClock clock = {};
  ClockInit(clock, t);
  return SPrint(cursor, stop, clock);
}

template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrint(CHS* cursor, CHS* stop, const TMT& t) {
  // The way the utf functions are setup, we return a nil-term CHA so we
  // don't have to check to write a single CHA in this
  A_ASSERT(cursor);
  A_ASSERT(cursor < stop);

  AClock clock;
  ClockInit(clock, ISD(t.seconds));
  cursor = SPrint(cursor, stop, clock);
  *cursor++ = ':';
  cursor = SPrint(cursor, stop, t.ticks);
  if (IsError(cursor)) return NILP;
  return cursor;
}

template<typename Printer>
Printer& TSPrint(Printer& o, TMT& t) {
  AClock clock;
  ClockInit(clock, t.seconds);
  return TSPrint<Printer>(o, clock) << ':' << t.ticks;
}

/* Scans a time in seconds from the given string. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* element, ISC& hour, ISC& minute, ISC& second) {
  if (element == NILP)
    return NILP;

  D_COUT("\n\n    Scanning time:\"" << element << '\"');
  ISC h = 0,   //< Hour.
      m = 0,   //< Minute.
      s = 0;   //< Second.
  if (IsError(TSScanSigned<ISC, IUC, CHS, CHT>(++element, h))) {
    D_COUT("\nInvalid hour:" << h);
    return NILP;
  }
  element = TStringSkipNumbers<CHS>(element);
  if (h < 0) {
    D_COUT("\nHours:" << h << " can't be negative.");
    return NILP;
  }
  if (h > 23) {
    D_COUT("\nHours:" << h << " can't be > 23.");
    return NILP;
  }
  D_COUT(h);
  CHS c = *element++;
  if (!c || TIsWhitespace<CHS>(c)) {  // Case @HH
    D_COUT(" HH ");
    // Then it's a single number_, so create a ISC for the current
    // user-time hour..
    hour = h;
    return element;
  }
  c = TToLower<CHS>(c);
  if (c == 'a') {  //
    D_COUT("\nCase @HHAm\n HHam ");
    c = *element++;
    if (TToLower<CHS>(c) == 'm') c = *element++;
    if (c && !TIsWhitespace<CHS>(c)) {
      D_COUT("\nInvalid am format.");
      return NILP;
    }
    D_COUT(" @HHAM ");
    hour = h;
    return element;
  }
  if (c == 'p') {
    D_COUT(" Case @HHpm ");
    c = *element++;
    if (TToLower<CHS>(c) == 'm') c = *element++;
    if (c && !TIsWhitespace<CHS>(c)) {
      D_COUT("\ninvalid pm format.");
      return NILP;
    }
    D_COUT("\nCase @HHPM " << h + 12 << ":00:00");
    hour = h + 12;
    return element;
  }
  if (c != ':') {
    D_COUT("\nExpecting ':'.");
    return NILP;
  }

  D_COUT(
      "\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
      "HH:MM:SSpm");

  if (IsError(TSScanSigned<ISC, IUC, CHS, CHT>(element, m))) return NILP;
  element = TStringSkipNumbers<CHS>(element);
  if (m < 0) {
    D_COUT("\nMinutes:" << m << " can't be negative!");
    return NILP;
  }
  if (m >= 60) {
    D_COUT("\nMinutes:" << m << " can't be >= 60!");
    return NILP;  //< 60 minutes in an hour.
  }
  D_COUT(':' << m);

  element = TStringSkipNumbers<CHS>(element);
  c = *element++;
  if (!c || TIsWhitespace<CHS>(c)) {
    D_COUT(" HH:MM ");
    hour = h;
    minute = m;
    return element;
  }
  c = TToLower<CHS>(c);
  if (c == 'a') {
    D_COUT(" HH:MMam ");
    c = *element++;
    if (TToLower<CHS>(c) == 'm') {  // The 'm' is optional.
      c = *element++;
    }
    if (c && !TIsWhitespace<CHS>(c)) {  // The space is not.
      D_COUT("Invalid am in HH::MM AM");
      return NILP;
    }
    hour = h;
    minute = m;
    return element;
  }
  if (c == 'p') {  //< Case HH:MM PM
    D_COUT(" HH:MMpm ");
    c = *element++;
    if (TToLower<CHS>(c) == 'm') {  //< The 'm' is optional.
      c = *element++;
    }
    if (c && !TIsWhitespace<CHS>(c)) {  //< The space is not.
      D_COUT("Invalid am in HH::MM PM");
      return NILP;
    }
    hour = h + 12;
    minute = m;
    return element;
  }
  if (c != ':') return NILP;

  D_COUT("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm");

  if (IsError(TSScanSigned<ISC, IUC, CHS, CHT>(element, s))) return NILP;
  if (s < 0) {
    D_COUT("\nSeconds:" << s << " can't be negative!");
    return NILP;
  }
  if (s > 60) {
    D_COUT("\nSeconds:" << s << " can't be >= 60!");
    return NILP;
  }
  D_COUT(':' << s);
  element = TStringSkipNumbers<CHS>(element);
  c = TToLower<CHS>(*element);
  if (!c || TIsWhitespace<CHS>(c)) {
    D_COUT(" HH:MM:SS ");
    hour = h;
    minute = m;
    second = s;
    return element;
  }
  if (c == 'a') {
    D_COUT(" HH:MM:SSam ");
    c = *element++;
    if (TToLower<CHS>(c) == 'm') {  //< The 'm' is optional.
      c = *element++;
    }
    if (!c || !TIsWhitespace<CHS>(c)) {  //< The space is not.
      D_COUT("\nInvalid am in HH::MM:SS AM");
      return NILP;
    }
    hour = h;
    minute = m;
    second = s;
    return element;
  }
  if (c != 'p') {
    D_COUT("\nExpecting a PM but found:" << c);
    return NILP;  // Format error!
  }
  D_COUT(" HH:MM:SSpm ");
  c = TToLower<CHS>(*element++);
  if (c == 'm') {  //< The 'm' is optional.
    c = *element++;
  }
  if (!c || !TIsWhitespace<CHS>(c)) {  //< The space is not.
    D_COUT("\nInvalid am in HH::MM:SS PM");
    return NILP;
  }
  hour = h + 12;
  minute = m;
  second = s;
  return element;
}

/* Scans the given string for a timestamp. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* element, AClock& clock) {
  D_ASSERT(element);
  D_COUT("\n    Scanning AClock:\"" << element << "\n    Scanning: ");
  element = TSSkipAll<CHS>(element, '0');
  CHS c = *element;  //< The current CHS.
  const CHS* stop;  //< Might not need
  ISC hour   = 0,
      minute = 0,
      second = 0;

  if (c == '@') {
    element = TSScan<CHS, CHT>(element, hour, minute, second);
    if (IsError(element)) {
      D_COUT("\nCase @ invalid time");
      return NILP;
    }
    clock.hour = hour;
    clock.minute = minute;
    clock.second = second;

    return element + 1;
  }
  if (c == '#') {
    element = TSScan<CHS, CHT>(element, hour, minute, second);
    if (IsError(element)) {
      D_COUT("\nCase @ invalid time");
    }
    clock.hour += hour;
    clock.minute += minute;
    clock.second += second;

    return element + 1;
  }

  ISC value1 = 0,       //< The first date field scanned.
      value2 = 0,       //< The second date field scanned.
      value3 = 0,       //< The third date field scanned.
      is_last_year = 0; //< Flag for if the date was last year or not.

  // SScan value1
  if (IsError(TSScanSigned<ISC, IUC, CHS, CHT>(element, value1))) {
    D_COUT("SScan error at value1");
    return NILP;
  }
  if (value1 < 0) {
    D_COUT("Dates can't be negative.");
    return NILP;
  }
  element = TSDecimalEnd<CHS>(element);
  if (IsError(element)) 
    return NILP;
  CHT delimiter = *element++;
  D_COUT(value1);
  if (delimiter == '@') {
    D_COUT(" HH@ ");

    if (IsError(element = TSScan<CHS, CHT>(element, hour, minute, second))) {
      D_COUT("\nInvalid time DD@");
      return NILP;
    }
    clock.day = value1;

    return element + 1;
  }
  // SScan value2.
  element = TSSkipAll<CHS>(element, '0');
  if (IsError(TSScanSigned<ISC, IUC, CHS, CHT>(element, value2))) {
    D_COUT("\n    Failed scanning value2 of date.");
    return NILP;
  }
  if (value2 < 0) {
    D_COUT("Day can't be negative.");
    return NILP;  //< Invalid month and day.
  }
  D_COUT(value2);
  element = TSDecimalEnd<CHS>(element);
  c = *element;
  if (c != delimiter) {
    D_COUT("\n    Cases MM/DD and MM/YYyy");
    if (c == '@') {
      if (IsError(element = TSScan<CHS, CHT>(element, hour, minute, second))) {
        D_COUT(" invalid time ");
        return NILP;
      }
    }
    if (!c || TIsWhitespace<CHS>(c)) {
      D_COUT("\n    Format is MM/DD and year is " << clock.year + ClockEpoch());
      // is_last_year = ((value1 >= std_tm.mon) &&
      //    (value2 >= std_tm.mday)) ? 0:1;
      clock.year += is_last_year;
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = 0;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;

      return element + 1;
    }
    c = TToLower<CHS>(c);
    if ((value1 < 12) && (value2 > 0) &&
        (value2 <= ClockMonthDayCount(value1))) {
      D_COUT(" MM/DD ");
      if (value1 > 11) {
        D_COUT("\nInvalid MM/DD@ month");
        return NILP;
      }
      // We need to find out what year it is.
      AClock clock;
      ClockInit(clock);

      if (value2 > ClockMonthDayCount(clock.year - ClockEpoch(), value1)) {
        D_COUT("\nInvalid MM/DD@ day");
        return NILP;
      }
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;
      if (IsError(element = TSScan(element, hour, minute, second))) {
        D_COUT("\nInvalid MM/DD@");
        return NILP;
      }

      return element + 1;
    }
    if ((value1 < 12) && (value2 > ClockMonthDayCount(value1))) {
      D_COUT(" MM/YYyy");
      clock.month = value1 - 1;
      clock.year = value2;
      if (IsError(element = TSScan<CHS, CHT>(element, hour, minute, second))) {
        D_COUT("\nInvalid MM / YYYY@ time");
        return NILP;
      }

      return element + 1;
    }
    D_COUT("\nInvalid MM/DD or MM/YYyy format");
    return NILP;
  }

  // Formats MM/DD/YYyy and YYyy/MM/DD
  element = TSSkipAll<CHS>(++element, '0');
  c = *element;
  // Then there are 3 values and 2 delimiters.
  if (!IsDigit(c) || !TSScanSigned<ISC, IUC, CHS, CHT>(element, value3)) {
    D_COUT("\n    SlotRead error reading value3 of date." << CHS(c) << ": ");
    return NILP;  //< Invalid format!
  }
  element = TSDecimalEnd<CHS>(element);
  D_COUT(c << value3);
  // Now we need to check what format it is in.

  c = *element;
  if (c == '@') {
    if (IsError(stop = TSScan<CHS, CHT>(element, hour, minute, second))) {
      D_COUT("Invalid YYyy/MM/DD@ time.");
      return NILP;
    }
  }
  clock.hour = hour;
  clock.minute = minute;
  clock.second = second;
  if (TIsWhitespace<CHS>(*(++element))) {
    D_COUT("No date found.");
    return NILP;
  }
  if (value1 > 11) {  //<
    D_COUT("\n    Case YYyy/MM/DD");
    if (value2 == 0 || value2 > 12) {
      D_COUT("Invalid number of months");
      return NILP;
    }

    if (value2 > ClockMonthDayCount(value2, value1)) {
      D_COUT("Invalid number of days");
      return NILP;
    }  // 17/05/06

    if (value1 < 100) {
      D_COUT("\n    Case YY/MM/DD");
      value1 += 2000 - ClockEpoch();
    } else {
      D_COUT("\n    Case YYYY/MM/DD");
      value1 -= ClockEpoch();
    }

    clock.year  = value1;
    clock.month = value2 - 1;
    clock.day   = value3;

    return element + 1;
  }
  D_COUT("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    D_COUT("\nInvalid month.");
    return NILP;
  }
  if (value2 > ClockMonthDayCount(value1, value3)) {
    D_COUT("\nInvalid day.");
    return NILP;
  }
  clock.month = value1 - 1;
  clock.day = value2;
  if (value3 < 100) {
    D_COUT("\n    Case MM/DD/YY");
    clock.year = value3 + (2000 - ClockEpoch());
  } else {
    D_COUT("\n    Case MM/DD/YYYY");
    clock.year = value3 - ClockEpoch();
  }
  return element + 1;
}

template<typename CHS, typename CHT, typename IS>
const CHS* TSScan(const CHS* origin, IUC& result) {
  AClock clock = {};
  const CHS* stop = TSScan<CHS, CHT>(origin, clock);
  D_ASSERT(stop);
  result = ClockToTimestamp(clock);
  return stop;
}

template<typename CHS, typename CHT, typename IS>
const CHS* TSScan(const CHS* origin, TMC& result) {
  AClock clock = {};
  const CHS* stop = TSScan<CHS, CHT>(origin, clock);
  #ifdef D_THIS
  TClockPrint<COut>(StdOut(), clock);
  #endif
  D_ASSERT(stop);
  result = ClockToTimestamp(clock);
  return stop;
}

template<typename CHS, typename CHT, typename IS>
const CHS* TSScan(const CHS* origin, TMD& result) {
  AClock clock = {};
  const CHS* stop = TSScan<CHS, CHT>(origin, clock);
  D_ASSERT(stop);
  result = TMD(ClockToTimestamp(clock));
  return stop;
}

template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* origin, TMT& result) {
  origin = TSScan<CHS, CHT, ISC>(origin, result.seconds);
  if (IsError(origin)) return NILP;
  if (*origin++ != ':') {
    result.ticks = 0;
    return origin - 1;
  }
  return TSScanUnsigned<IUC, CHS, CHT>(origin, result.ticks);
}
#endif  // #if USING_STR

template<typename IS>
IS TClockSet(AClock* clock, IS t) {
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  IS value = t / SecondsPerYear;
  t -= value * SecondsPerYear;
  clock->year = ISC(value - ClockEpoch());
  value = t / SecondsPerDay;
  t -= value * SecondsPerDay;
  clock->day = ISC(value);
  value = t / SecondsPerHour;
  t -= value * SecondsPerHour;
  clock->hour = ISC(value);
  value = t / SecondsPerMinute;
  clock->minute = ISC(value);
  clock->second = ISC(t - value * SecondsPerMinute);
  return t;
}

/* A time in seconds stored as either a 32-bit or 64-bit IS.
The difference between a TClock and AClock is that that TClock stores the AClock
and the TMD or ISC. */
template<typename IS>
struct TClock {
  AClock clock;  //< A human-readable clock.

  /* Constructs a clock from the given seconds timestamp. */
  TClock(IS t) { ClockInit(clock, t); }

  /* Prints the given */
  template<typename Printer, typename IS_>
  Printer& Print(Printer& o) {
    return o << clock.year << '/' << clock.month + 1 << '/' << clock.day 
             << '@' << clock.hour << ':' << clock.minute << ':' << clock.second;
  }
};

}  //< namespace _
#if USING_CONSOLE == YES_0

inline ::_::COut& operator<<(::_::COut& o, const ::_::AClock& item) {
  return ::_::TClockPrint(o, item);
}

template<typename IS>
::_::COut& operator<<(::_::COut& o, ::_::TClock<IS> item) {
  return o << item.clock;
}
#endif

#endif
#endif
