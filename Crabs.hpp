// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsCrabsHPP
#define _CrabsCrabsHPP
#include "Crabs.h"
#if _Seam >= _CrabsOp
#if _Seam == _CrabsOp
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

template<typename Printer>
Printer& TCrabsPrintStack(Printer & o, Crabs* crabs) {
  _AAssert(crabs);

  ISC i, stack_count;
  const Op* op;
  Operand* operand;
  Operand** stack = CrabsStack(crabs);

  stack_count = crabs->stack_count;
  o << "\nOperand stack_count:" << stack_count;

  if (stack_count == 1) {
    return o << "\nStack Item 1: " << OperandName(crabs->root);
  }
  for (i = 0; i < stack_count - 1; ++i) {
    o << "\nStack Item " << i + 1 << ":\"";
    operand = stack[i];
    op = operand->Star('?', _NilP);
    o << op->name << '\"';
  }
  op = crabs->operand->Star('?', _NilP);
  return o << "\nStack Item " << i + 1 << ":\"" << op->name << "\"";
}

template<typename Printer>
Printer& TCrabsPrint(Printer& o, Crabs* crabs) {
  if (IsError(crabs)) 
    return o;
  o << Linef('~', 80) << "\nStack:    " << Hexf(crabs) << '\n'
    << Linef('_', 80) << "\nbytes_left : " << crabs->bytes_left
    << "\nheader_size: " << crabs->header_bytes
    << "\nstack_count: " << crabs->stack_count
    << "\nstack_size : " << crabs->stack_total
    << "\nbin_state  : " << BInStates(crabs->bin_state)
    << "\nbout_state : " << BOutStates(crabs->bout_state)
    << "\nnum_states : " << crabs->num_states
    << "\nheader_size: " << crabs->header_bytes << Linef('-', 80)
    << Hexf(crabs->operand) << "\nheader     : ";
  TBSeqPrint<Printer>(o, crabs->header_start);
  o << Linef('-', 80);
  TCrabsPrintStack<Printer>(o, crabs);
  o << Linef('-', 80);
  TBInPrintVars<Printer>(o, CrabsBIn(crabs));
  TBOutPrintVars<Printer>(o, CrabsBOut(crabs));
  return o << Linef('-', 80) << '\n';
}

}  // namespace _
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsCrabsHPP
