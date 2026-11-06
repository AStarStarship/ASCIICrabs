// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsOperandHXX
#define _CrabsOperandHXX
#include "Operand.h"
#if _Seam >= _CrabsOp
#include "Op.hpp"
#if _Seam == _CrabsOp
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

const CHA* OperandName(Operand* operand) {
  _AAssert(operand);
  const Op* op = operand->Star('?', _NilP);
  _AAssert(op);

  return op->name;
}

IUW OperandCount(Operand* operand) {
  _AAssert(operand);
  const Op* op = operand->Star(0, _NilP);
  return (op == _NilP) ? 0 : IUW(op->in);
}

CHC OperandIndex(Operand* operand, CHA* origin, CHA* stop) {
  _AAssert(operand);
  const Op* op = operand->Star('?', _NilP);
  _AAssert(op);
  CHC index = OpFirst(op),
      last  = OpLast (op);
  _AAssert(index);
  for (; index <= last; ++index) {
    if (TSEquals<CHA>(origin, stop, operand->Star(index, _NilP)->name)) {
      return index;
    }
  }
  return 0;
}

#if _CrabsUsingText
/*
UTF& Print (UTF& utf, const Operand* op) {
    utf << "\n Op:\n" << op->name << "\nparams_in:"
          << BSeq (op->in)
          << "\nparams_out:" << op->out
          << "\npop:" << op->pop  << " close:" << op->close
          << " default_op:"       << op->default_op
          << "\nignore_chars :"   << op->ignore_chars
          << "\nallowed_chars:"   << op->allowed_chars
          << "\n description :\"" << op->description;
    return utf;
}*/

UTF1& PrintOperand(UTF1& utf, Operand* operand) {
  _AAssert(operand);

  const Op* op = operand->Star('?', _NilP);

  _AAssert(op);

  IUW num_ops = IUW(op->in),
      op_num = IUW(op->out), last_op = op_num + num_ops - 1;
  if (num_ops > ParamsTotal) {
    return utf << "\nError: Too many parameters!";
  }
  utf << "\nOperand         :" << op->name << Line('-', 80);
  for (; op_num <= last_op; ++op_num) {
    op = operand->Star((CHC)op_num, _NilP);
    utf << "\nOp \'" << op_num << "\':" << op_num << ' ' << op << Line('-', 80);
  }
  return utf;
}

Slot& OperandQuery(Operand* root, const CHA* address, Slot& slot) {
  _AAssert(address);
  _AAssert(root);

  ISC index = *address++;
  const Op* op = root->Star(index, _NilP);
  CHA socket[1024];
  _DROut(op->name)
  index = *address++;
  while (index) {
    op = root->Star(index, _NilP);
    _AAssert(op);
    _DROut('.' << op->name)
    index = *address++;
  }
  slot.Write(socket);
  return slot;
}

#endif
}  //< namespace _
#endif

#endif  //< _CrabsOperandHXX
