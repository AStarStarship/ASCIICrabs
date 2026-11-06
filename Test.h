// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef _CrabsTestH
#define _CrabsTestH

/* Function pointer prototype for a test case with command line argument.
@return Pointer to a  to utf to the stdout.
@param seam_log Beginning of the seam log socket.
@param seam_end End of the seam log socket.
@param args     Command line argument . */
typedef const CHA* (*TestCase)(const CHA* args);

namespace _ {

// Triggers a software-define break point that outputs an optional message.
BOL BRK(BOL value, const CHR* message = _NilP);
BOL* BRK(BOL* value, const CHR* message = _NilP);
const BOL* BRK(const BOL* value, const CHR* message = _NilP);

IUA BRK(IUA value, const CHR* message = _NilP);
IUA* BRK(IUA* value, const CHR* message = _NilP);
const IUA* BRK(const IUA* value, const CHR* message = _NilP);

ISA BRK(ISA value, const CHR* message = _NilP);
ISA* BRK(ISA* value, const CHR* message = _NilP);
const ISA* BRK(const ISA* value, const CHR* message = _NilP);

IUB BRK(IUB value, const CHR* message = _NilP);
IUB* BRK(IUB* value, const CHR* message = _NilP);
const IUB* BRK(const IUB* value, const CHR* message = _NilP);

ISB BRK(ISB value, const CHR* message = _NilP);
ISB* BRK(ISB* value, const CHR* message = _NilP);
const ISB* BRK(const ISB* value, const CHR* message = _NilP);

IUC BRK(IUC value, const CHR* message = _NilP);
IUC* BRK(IUC* value, const CHR* message = _NilP);
const IUC* BRK(const IUC* value, const CHR* message = _NilP);

ISC BRK(ISC value, const CHR* message = _NilP);
ISC* BRK(ISC* value, const CHR* message = _NilP);
const ISC* BRK(const ISC* value, const CHR* message = _NilP);

IUD BRK(IUD value, const CHR* message = _NilP);
IUD* BRK(IUD* value, const CHR* message = _NilP);
const IUD* BRK(const IUD* value, const CHR* message = _NilP);

ISD BRK(ISD value, const CHR* message = _NilP);
ISD* BRK(ISD* value, const CHR* message = _NilP);
const ISD* BRK(const ISD* value, const CHR* message = _NilP);

FPC BRK(FPC value, const CHR* message = _NilP);
FPC* BRK(FPC* value, const CHR* message = _NilP);
const FPC* BRK(const FPC* value, const CHR* message = _NilP);

FPD BRK(FPD value, const CHR* message = _NilP);
FPD* BRK(FPD* value, const CHR* message = _NilP);
const FPD* BRK(const FPD* value, const CHR* message = _NilP);

//IUE BreakPoint(IUE value, const CHR* message = _NilP);
//IUE* BRK(IUE* value, const CHR* message = _NilP);
//const IUE* BRK(const IUE* value, const CHR* message = _NilP);
// 
//ISE BreakPoint(ISE value, const CHR* message = _NilP);
//ISE* BRK(ISE* value, const CHR* message = _NilP);
//const ISE* BRK(const ISE* value, const CHR* message = _NilP);

CHA BRK(CHA value, const CHR* message = _NilP);
CHA* BRK(CHA* value, const CHR* message = _NilP);
const CHA* BRK(const CHA* value, const CHR* message = _NilP);

CHB BRK(CHB value, const CHR* message = _NilP);
CHB* BRK(CHB* value, const CHR* message = _NilP);
const CHB* BRK(const CHB* value, const CHR* message = _NilP);

CHC BRK(CHC value, const CHR* message = _NilP);
CHC* BRK(CHC* value, const CHR* message = _NilP);
const CHC* BRK(const CHC* value, const CHR* message = _NilP);

/* Called when a test fails.
@return True upon failure.
@param line The line the program failed at.
@param file The file the error occurred at.
@param message An optional message to utf. */
BOL TestFail(ISN line, const CHA* funciton, const CHA* file);

/* Handles an _DAssert.
@return True upon failure.
@param line The line the program failed at.
@param file The file the error occurred at.
@param message An optional message to utf. */
void TestFunctionLine(ISN line, const CHA* funciton, const CHA* file);

/* Handles an _DAssert.
@return True upon failure.
@param line The line the program failed at.
@param file The file the error occurred at.
@param message An optional message to utf. */
BOL TestWarn(ISN line, const CHA* funciton, const CHA* file);

/* Converts ISC main(ISC,CHA**) arguments back into a string.
@return nil if there are no arguments or the args string upon success.
@param arg_count The number_ of arguments.
@param args      The arguments. */
const CHA* ArgsToString(ISN arg_count, CHA** args);

/* Converts the result not being null into the proper OS return value for int
main (). */
ISN SeamResult(const CHA* result);

/* Tests an array of TestCase(s).
@return 0 upon success or an app exit code upon failure. */
ISN SeamTreeTest(ISN arg_count, CHA** args, TestCase* tests, ISN test_count);

/* Tests an array of TestCase(s).
@return Nil upon success or an error  upon failure. */
const CHA* TestTree(const CHA* args, TestCase* seams, ISN node_count);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param condition The condition to check. */
BOL Test(BOL condition);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param value The value to test. */
BOL Test(ISA value);
BOL Test(IUA value);
BOL Test(ISB value);
BOL Test(IUB value);
BOL Test(ISC value);
BOL Test(IUC value);
BOL Test(ISD value);
BOL Test(IUD value);
BOL Test(FPC value);
BOL Test(FPD value);

/* Tests if the pointer is valid.
@return false if the condition is false.
@param value The value to test.  */
BOL Test(const void* value);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(const CHA* a, const CHA* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(CHA a, CHA b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(CHB a, CHB b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(CHC a, CHC b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(const CHB* a, const CHB* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(const CHC* a, const CHC* b);

/* Tests if the two pointers are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(const void* a, const void* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(IUA a, IUA b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(ISA a, ISA b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(IUA a, IUA b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(ISB a, ISB b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(IUB a, IUB b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(ISC a, ISC b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(IUC a, IUC b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(ISD a, ISD b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(IUD a, IUD b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(FPC a, FPC b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param a Parameter a.
@param b Parameter b. */
BOL TestEq(FPD a, FPD b);

}  //< namespace _

#endif  //< _CrabsTestH
