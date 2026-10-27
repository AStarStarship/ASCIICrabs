// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_CLOCK_DECL
#define CRABS_CLOCK_DECL
#include <_Config.h>
#if SEAM >= CRABS_CLOCK

namespace _ {

/* @defgroup CLOCK Clock
@brief A set of functions and data structures for working with time and clocks.
@details The CLOCK module provides a set of functions and data structures for working with time and clocks
@see ./_Spec/Data/Clock.md
*/

enum ClockConstants {
  SecondsPerMinute = 60,                    //< Number of seconds in an minute.
  SecondsPerHour = 60 * SecondsPerMinute,   //< Number of seconds in an hour.
  SecondsPerDay = 24 * SecondsPerHour,      //< Number of seconds in an day.
  SecondsPerYear = SecondsPerDay * 365,     //< Number of seconds in an year.
  DayLeap = 58,                             //< February 28th.
  DaysYear = 365,                           //< Number of days in a year.
  DaysJanuary = 31,                         //< Number of days in January.
  DaysFebruary = 28,                        //< Number of days in February.
  DaysMarch = 31,                           //< Number of days in March.
  DaysApril = 30,                           //< Number of days in April.
  DaysMay = 31,                             //< Number of days in May.
  DaysJune = 30,                            //< Number of days in June.
  DaysJuly = 31,                            //< Number of days in July.
  DaysAugust = 31,                          //< Number of days in August.
  DaysSeptember = 30,                       //< Number of days in September.
  DaysOctober = 31,                         //< Number of days in October.
  DaysNovember = 30,                        //< Number of days in November.
  DaysDecember = 31,                        //< Number of days in December.

  // @warning the Days Start and Stop values are 1-indexed non-leap year.
  DaysJanuaryStart = 1,                     //< First day of year in January.
  DaysFebruaryStart = 32,                   //< First day of year in February.
  DaysMarchStart = 60,                      //< First day of year in March.
  DaysAprilStart = 91,                      //< First day of year in April.
  DaysMayStart = 121,                       //< First day of year in May.
  DaysJuneStart = 152,                      //< First day of year in June.
  DaysJulyStart = 182,                      //< First day of year in July.
  DaysAugustStart = 213,                    //< First day of year in August.
  DaysSeptemberStart = 244,                 //< First day of year in September.
  DaysOctoberStart = 274,                   //< First day of year in October.
  DaysNovemberStart = 305,                  //< First day of year in November.
  DaysDecemberStart = 335,                  //< First day of year in December.

  DaysJanuaryStop = 31,                    //< Last day of year in January.
  DaysFebruaryStop = 59,                   //< Last day of year in February.
  DaysMarchStop = 90,                      //< Last day of year in March.
  DaysAprilStop = 120,                     //< Last day of year in April.
  DaysMayStop = 151,                       //< Last day of year in May.
  DaysJuneStop = 181,                      //< Last day of year in June.
  DaysJulyStop = 212,                      //< Last day of year in July.
  DaysAugustStop = 243,                    //< Last day of year in August.
  DaysSeptemberStop = 273,                 //< Last day of year in September.
  DaysOctoberStop = 304,                   //< Last day of year in October.
  DaysNovemberStop = 334,                  //< Last day of year in November.
  DaysDecemberStop = 365,                  //< Last day of year in December.

  MonthJanuary = 0,                         //< Index of January.
  MonthFebruary = 1,                        //< Index of February.
  MonthMarch = 2,                           //< Index of March.
  MonthApril = 3,                           //< Index of April.
  MonthMay = 4,                             //< Index of May.
  MonthJune = 5,                            //< Index of June.
  MonthJuly = 6,                            //< Index of July.
  MonthAugust = 7,                          //< Index of August.
  MonthSeptember = 8,                       //< Index of September.
  MonthOctober = 9,                         //< Index of October.
  MonthNovember = 10,                       //< Index of November.
  MonthDecember = 11,                       //< Index of December.

  SSDHotSecondsBitCount = 28,  //< SSD Hot Seconds bit count.
  SSDHotTickerBitCount  = 8,   //< SSD Hot Subsecond Ticker bit count.
  SSDHotSourceBitCount  = 28,  //< SSD Hot Source Id bit count.
  // SSD Hot Seconds bit 0.
  SSDHotSecondsBit0 = SSDHotSourceBitCount + SSDHotTickerBitCount,
  SSDHotTickerBit0  = SSDHotSourceBitCount, //< SSD Hot Subsecond Ticker bit 0.

  SSDSecondsMaskLSB = SSDHotSecondsBitCount - 1, //< Mask for SSD seconds.
  SSDTickerMaskLSB  = SSDHotTickerBitCount - 1,  //< Mask for SSD ticks.
  SSDSourceMaskLSB  = SSDHotSourceBitCount - 1,  //< Mask for SSD source.

  SSDEpochSeconds = 1 << (SSDHotSecondsBitCount - 1), 

  SSDEvergreenSourceBitCount = 34, //< Evergreen UUID source id bit count.
  SSDEvergreenBit0 = 34,           //< Bit 0 of the Evergreen marker bits.
  SSDEvergreenSecondsBit0 = 36,    //< Bit 0 of the Evergreen seconds field.
  SSDEvergreenBitMaskLSB = 3,      //< Mask for bits 35:34 in LSB (0b11).

  SSDColdSecondsBitCount = 36,  //< Cold UUID seconds timestamp bit count.
  SSDColdTickerBitCount  = 16,  //< Cold UUID subsecond ticker bit count.
  SSDColdSourceBitCount  = 76,  //< Cold UUID Source bit count.

  SSESecondsBitCount = 36,      //< Cold UUID seconds timestamp bit count.
  SSETickerBitCount  = 16,      //< Cold UUID subsecond ticker bit count.
  SSESourceBitCount  = 76,      //< Cold UUID Source bit count.

  // SSE UUID Subsecond Ticker bit 0.
  SSESecondsBit0 = SSESourceBitCount + SSETickerBitCount,
  SSETickerBit0 = SSESourceBitCount, //< SSE Subsecond Ticker bit 0.

  // Bit 0 of the MSB of the SSE seconds timestamp.
  SSESecondsMSBit0 = SSESourceBitCount - 64,
	// Bit 0 of the MSB of the SSE ticker timestamp.
  SSETickerMSBit0  = 64 - SSESecondsBitCount,
	// Number of bits in the MSB of the SSE ticker timestamp.
	SSETickerMSBBitCount = 64 - SSESecondsBitCount,

  SSESecondsMaskLSB = SSESecondsBitCount - 1, //< Mask for SSE seconds in LSB.
  SSETickerMaskLSB = SSETickerBitCount - 1,   //< Mask for SSE ticker in LSB.
  SSESourceMaskLSB = SSESourceBitCount - 1,   //< Mask for SSE source in LSB.
};

/* A time in seconds and optional microseconds format that is compatible with the C++ standard library.
Data structure is identical to std::tm with the execution that it has an
additional microseconds from origin of second variable. */
struct AClock {
  ISB year;    //< Number of years since epoch [-1902, 1970] U [1970, 2038]
  ISA second,  //< Second of the minute [0, 59].
      minute,  //< Minute of the hour [0, 59].
      hour,    //< Hour of the day [0, 23].
      day,     //< Day of the month [1, 31].
      month,   //< Months since December [0, 11].
      weekday; //< Days since Sunday [0, 6].
};

/* 64-bit ASCII Time Ticker.
@see ~/_Spec/Data/Types/Clock.md */
struct TMT {
  ISC seconds;  //< Unix seconds timestamp.
  IUC ticks;    //< Subsecond spin ticker.

  // Creates a TMD from the given seconds, ticks, and id
  TMT(ISC seconds, IUC tick = 0);

  // Creates a TMD from the given word.
  TMT(IUD value);

  // Creates a TMD from the given word.
  TMT(ISD value);
};

/* 64-bit ASCII Subsecond Id.
@see ~/_Spec/Data/Types/Clock.md */
struct SSD {
  ISD value;  //< Seconds, Ticker, and Id word.

  // Creates a TMD from the given seconds, ticks, and id
  SSD(ISC seconds, ISC ticker = 0, ISC source = 0);

  // Creates a TMD from the given word.
  SSD(IUD value);

  // Creates a TMD from the given word.
  SSD(ISD value);

  // Gets the seconds bits of the TMD.
  ISC Seconds();

  // Gets the ticks bits of the TMD.
  ISC Ticker();

  // Gets the id portion of the TMD.
  ISC Source();

  // Gets the bit pattern type (Hot, Cold, or Evergreen)
  BOL IsHot();
  BOL IsCold();
  BOL IsEvergreen();
  
  // Converts Hot to Cold UUID.
  ISD ToCold(ISC ticker) const;
};

/* A 128-bit ASCII Subsecond Id.
@see ~/_Spec/Data/Clock.md */
struct SSE {
  ISD lsb,    //< LSB.
      msb;    //< MSB.

  /* Creates a SSE from the given seconds, ticks, and id */
  SSE(ISD seconds, ISD tick = 0, ISD id_msb = 0, ISD id_lsb = 0);

  // Gets the seconds bits of the SSE.
  ISC Seconds();

  // Gets the ticks bits of the SSE.
  ISC Ticks();

  // Gets the id LSB portion of the SSE.
  ISD IdLSB();

  // Gets the id MSB portion of the SSE.
  ISC IdMSB();
};

/* Gets the 32-bit ISC clock epoch. */
TMC ClockEpoch();

/* Returns true if the year is a leap year. */
BOL ClockIsLeapYear(ISC year);
BOL ClockIsNotLeapYear(ISC year);
ISB ClockYear(TMC timestamp, TMC epoch);
ISA ClockMonth(TMC timestamp, TMC epoch);
ISA ClockDay(TMC timestamp, TMC epoch);

// Converts the 0-indexed day_of_year to the month index.
ISA ClockDayToMonth(TMC day_of_year, TMC year);
ISA ClockHour(TMC timestamp, TMC epoch);
ISA ClockMinute(TMC timestamp, TMC epoch);
ISA ClockSecond(TMC timestamp, TMC epoch);
ISA ClockWeekday(TMC timestamp, TMC epoch);

/* Lookup table for converting from day-of-year to month. */
const ISB* ClockLastDayOfMonth();

/* Lookup table for converting from day-of-year to month. */
const ISB* ClockLastDayOfMonthLeapYear();

/* Returns which month the given day is in based on the year. */
ISN MonthByDay(ISN day, ISN year);

/* Initializes the clock from the given timestamp. */
AClock* ClockInit(AClock& clock, TMC time, TMC epoch = ClockEpoch());

/* Initializes the clock from the given timestamp. */
AClock* ClockInit(AClock& clock, TMD time, TMC epoch = ClockEpoch());

/* Initializes the clock from the given 64-bit microsecond timestamp. */
TMT& ClockInit(TMT& clock, ISC t, IUC ticks);

/* Initializes the clock from the given timestamp. */
AClock* ClockInit(AClock& clock);

/* Gets the current 64-bit timestamp. */
TMD ClockNow();

/* Creates a timestamp from the given seconds Clock. */
TMC ClockToTimestamp(AClock& clock);

/* Calculates the seconds from epoch from the clock and stores it to the result.
 */
//ISC ClockISC(AClock& clock);

/* Calculates the seconds from epoch from the clock and stores it to the result.
*/
//ISD ClockISD(AClock& clock);

/* Gets the number_ of days in a months.
    @todo Maybe get some open-source date utility? */
ISN ClockMonthDayCount(ISC t);

/* Gets the number_ of days in a months.
@param month The month index 0-11.
@param year   */
ISN ClockMonthDayCount(ISN month, ISN year);

/* Converts Hot UUID to Evergreen UUID.
@param hot_uuid The Hot UUID TMD value
@param source_id The random source ID.
@return Evergreen UUID as IUD */
IUD ClockHotToEvergreen(IUD hot_uuid, IUD source_id);

/* Generates a random 28-bit source ID for Hot UUID using RNG module.
@return Random 28-bit source ID. */
IUD GenerateHotSourceID();

/* Generates a random 34-bit source ID for Evergreen UUID using RNG module.
@return Random 34-bit source ID. */
IUD GenerateEvergreenSourceID();

/* Creates a Hot UUID from seconds, ticks, and source_id.
@param seconds The 27-bit seconds timestamp
@param ticks The 8-bit subsecond ticker (max 191)
@param source_id The 28-bit random source ID
@return Hot UUID as IUD */
IUD CreateHotUUID(ISC seconds, ISC ticks, IUD source_id);

/* Creates an Evergreen UUID from seconds and source_id.
@param seconds The 27-bit seconds timestamp  
@param source_id The 34-bit random source ID
@return Evergreen UUID as IUD */
IUD CreateEvergreenUUID(ISC seconds, IUD source_id);

/* Gets the abbreviated day of the week CHA of the given day number_ 1-7. */
const CHA* ClockWeekDay(ISN day_number);

/* Gets the abbreviated day of the week CHA of the given day number_ 1-7. */
CHA ClockDayOfWeekInitial(ISN day_number);

/* Compares the two the time and prints the results. */
ISN ClockCompare(ISC a, ISC b);

/* Compares the two the time and prints the results. */
ISN ClockCompare(const AClock& clock, const AClock& other);

/* Compares the given ISC to the time and prints the results. */
ISN ClockCompare(const AClock& clock, ISN year, ISN month, ISN day,
                            ISN hour, ISN minute, ISN second);

/* Zeros out the struct values.
@param calendar_time A calendar time struct to zero out. */
void ClockZeroTime(AClock& seconds);


TMC TimeMake(AClock& time);

/* Gets the array of days in each month. */
const ISA* ClockDaysMonth();

/* Converts the month and year into days in the month. */
ISN ClockDaysMonth(ISN month, ISN year);

/* Converts the year, month, and day to day of the year 1-365. */
ISN ClockDayOfYear(ISN year, ISN month, ISN day);

/* Creates a 32-bit seconds timestamp.  */
TMC ClockTimeTMC(ISN year, ISN month, ISN day, ISN hour = 0,
                            ISN minute = 0, ISN second = 0, TMC epoch = ClockEpoch());

/* Creates a 64-bit seconds timestamp.  */
TMD ClockTimeTMD(ISN year, ISN month, ISN day, ISN hour = 0,
                 ISN minute = 0, ISN second = 0, TMC epoch = ClockEpoch());

#if USING_STA == YES_0
/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHA* SPrint(CHA* origin, CHA* stop, const AClock& clock);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param stop   The stop of the write socket.
@param t      The 64-bit Time Ticker. */
CHA* SPrint(CHA* origin, CHA* stop, const TMT& t);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHA* ClockPrint(CHA* origin, CHA* stop, ISC time);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHA* ClockPrint(CHA* origin, CHA* stop, ISD time);

/* Reads a time or time delta from a a CHA starting with an '@' sign.
@brief
@code
@4        (This is 4AM)
@4PM      (No space required)
@4:20P    (Or M)
@4:20 PM
@16:20
@4:20 am
@4:20a.m.
@4:20:00
@4:20:00AM
@16:20:00
@endcode

@param string A nil-terminated string.
@param hour   The location to write the number_ of hours to.
@param minute The location to write the number_ of minutes to.
@param Second The location to write the number_ of seconds to. */
const CHA* ScanTime(const CHA* string, ISN& hour, ISN& minute,
                               ISN& second);

/* Converts a keyboard input to CHA and deletes the CHA.
@return Nil upon socket failure or CHA directly after the stop of the
timestamp upon success.
*/
const CHA* SScan(const CHA*, AClock& clock);

/* Converts a keyboard input to a SSE. */
const CHA* SScan(const CHA*, TMT& result);

/* Converts a keyboard input to a ISC. */
const CHA* SScanTime(const CHA*, TMC& result);

/* Converts a keyboard input to a ISD. */
const CHA* SScanTime(const CHA*, TMD& result);

#endif  //< #if USING_STA == YES_0

#if USING_STB == YES_0

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHB* SPrint(CHB* origin, CHB* stop, const AClock& clock);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHB* SPrint(CHB* origin, CHB* stop, const TMT& t);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHB* ClockPrint(CHB* origin, CHB* stop, TMC time);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHB* ClockPrint(CHB* origin, CHB* stop, TMD time);

/* Reads a time or time delta from a a CHB starting with an '@' sign.

# Legal Time Formats

@code
@4        (This is 4AM)
@4PM      (No space required)
@4:20P    (Or M)
@4:20 PM
@16:20
@4:20 am
@4:20a.m.
@4:20:00
@4:20:00AM
@16:20:00
@endcode

@param input  The CHB to parse.
@param hour   The location to write the number_ of hours to.
@param minute The location to write the number_ of minutes to.
@param Second The location to write the number_ of seconds to. */
const CHB* ScanTime(const CHB*, ISN& hour, ISN& minute, ISN& second);

/* Converts a keyboard input to CHB and deletes the CHB.
@return Nil upon socket failure or CHB directly after the stop of the
timestamp upon success.
*/
const CHB* SScan(const CHB*, AClock& result);

/* Converts a keyboard input to a SSD. */
const CHB* SScan(const CHB*, SSD& result);

/* Converts a keyboard input to a SSD. */
const CHB* SScan(const CHB*, SSE& result);

/* Converts a keyboard input to a SSD. */
const CHB* SScan(const CHB*, TMT& result);

/* Converts a keyboard input to a ISC. */
const CHB* ScanTime(const CHB*, ISC& result);

/* Converts a keyboard input to a TMD. */
const CHB* ScanTime(const CHB*, ISD& result);

#endif  //< #if USING_STB == YES_0
#if USING_STC == YES_0
/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHC* SPrint(CHC* origin, CHC* stop, const AClock& clock);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHC* SPrint(CHC* origin, CHC* stop, const TMT& time);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHC* ClockPrint(CHC* origin, CHC* stop, ISC time);

/* Writes the given time to the text socket.
@return Null upon failure or a pointer to the IUA after the last
IUA written.
@param origin The beginning of the write socket.
@param time  The time to utf.
@param stop   The stop of the write socket. */
CHC* ClockPrint(CHC* origin, CHC* stop, ISD time);

/* Reads a time or time delta from a a CHA starting with an '@' sign..
@param input  The CHA to parse.
@param hour   The location to write the number_ of hours to.
@param minute The location to write the number_ of minutes to.
@param Second The location to write the number_ of seconds to.
@return The offset The stop of where the parser exited successfully at.

@code
@4        (This is 4AM)
@4PM      (No space required)
@4:20P    (Or M)
@4:20 PM
@16:20
@4:20 am
@4:20a.m.
@4:20:00
@4:20:00AM
@16:20:00
@endcode
*/
const CHC* ScanTime(const CHC* input, ISN& hour, ISN& minute,
                               ISN& second);

/* Converts a keyboard input to CHA and deletes the CHA. */
const CHC* SScan(const CHC* input, AClock& time);

/* Converts a keyboard input to a signed integer. */
const CHC* SScan(const CHC* input, TMT& result);

/* Converts a keyboard input to a signed integer. */
const CHC* ScanTime(const CHC* input, ISC& result);

/* Converts a keyboard input to a signed integer. */
const CHC* ScanTime(const CHC* input, ISD& result);

#endif  //< #if USING_STC == YES_0
}  //< namespace _

#endif
#endif
