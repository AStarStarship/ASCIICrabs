// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_OP
#include "../Crabs.h"
#include "../BIn.hpp"
#include "../BOut.hpp"
#include "../BSeq.hpp"
#include "../Args.h"
#include "../Address.hpp"
#include "../Operand.h"
#if SEAM == CRABS_OP
#include "../_Debug.h"
#else
#include "../_Release.h"
#include "../AType.h"
#endif
using namespace ::_;
namespace CRTest {

class StarTest : public Operand {
public:
  const Op* Star(CHC index, Crabs* crabs) {
    static const Op foo = { "name",
      TBSeq<>().CParams() /*in*/,
      TBSeq<>().CParams() /*out*/,
      "description",
      'a',    //< First function index
      'b',    //< Last function index.
      'a',    //< Default function index.
      NILP    //< Evaluated expression Slot.
    };
    if (!crabs) return &foo;
    return NILP;
  }
};
}
#endif

namespace CRTest {
#if SEAM >= CRABS_OP
/* Return failures to the runner; do not rely on a printed assertion. */
static const CHA* RingRegressions(const CHA*) {
  const DTB* params = TTSQ<_IUC, _CHC, STR_, 32>();
  if (params[0] != 4 || params[1] != _IUC || params[2] != _CHC ||
      params[3] != STR_ || params[4] != 32)
    return "TTSQ must store its count and descriptors in one array";
  const CHR* chars = CCHRSQ<'a', 'b'>();
  if (chars[0] != 2 || chars[1] != 'a' || chars[2] != 'b')
    return "CCHRSQ must store its count and characters in one array";

  struct Output {
    TBOut<ISC> head;
    IUA data[256];
  } output = {};
  struct Input {
    TBIn<ISC> head;
    IUA data[256];
  } input = {};
  BOutInit(&output.head, sizeof(output));
  BInInit(&input.head, sizeof(input));
  IUC value = 0x12345678;
  void* write_args[] = {&value};
  params = TTSQ<_IUC>();
  if (BOutWrite(&output.head, params, write_args, 0))
    return "writing a 32-bit scalar to an empty ring must succeed";
  if (BOutBooferLength(&output.head) != 6)
    return "32-bit frame must have four payload bytes and two hash bytes";
  if (output.data[0] != 0x78 || output.data[1] != 0x56 ||
      output.data[2] != 0x34 || output.data[3] != 0x12)
    return "32-bit payload must be serialized little-endian";
  // Exercise the codec with a separate input header, never type-punned.
  for (ISN i = 0; i < output.head.stop; ++i)
    input.data[i] = output.data[i];
  input.head.stop = output.head.stop;
  struct Result {
    IUC value;
    IUC guard;
  } got = {0, 0xAABBCCDD};
  void* read_args[] = {&got.value};
  if (BInRead(&input.head, params, read_args, 0))
    return "reading a complete 32-bit frame must succeed";
  if (got.value != value || got.guard != 0xAABBCCDD)
    return "32-bit read must not fall through into the 64-bit case";
  if (BInIsReadable(&input.head))
    return "reading a frame must advance the consumer head";

  BOutInit(&output.head, sizeof(output));
  BInInit(&input.head, sizeof(input));
  IUA small = 0xA5;
  IUB medium = 0xBEEF;
  CHC character = U'\U0001F980';
  IUD large = 0xFEDCBA9876543210ULL;
  CHA text[] = "hello, chinese room";
  void* mixed_args[] = {&small, &medium, &value, &character, &large, text};
  params = TTSQ<_IUA, _IUB, _IUC, _CHC, _IUD, STR_, 32>();
  if (BOutWrite(&output.head, params, mixed_args, 0))
    return "writing a mixed scalar/string frame must succeed";
  if (BOutBooferLength(&output.head) !=
      ISN(sizeof(small) + sizeof(medium) + sizeof(value) +
          sizeof(character) + sizeof(large) + sizeof(text) + sizeof(IUB)))
    return "mixed frame must include every argument and its string terminator";
  for (ISN i = 0; i < output.head.stop; ++i)
    input.data[i] = output.data[i];
  input.head.stop = output.head.stop;
  IUA got_small = 0;
  IUB got_medium = 0;
  CHC got_character = 0;
  IUD got_large = 0;
  CHA got_text[32] = {};
  got.value = 0;
  void* mixed_read[] = {&got_small, &got_medium, &got.value,
                       &got_character, &got_large, got_text};
  if (BInRead(&input.head, params, mixed_read, 0))
    return "reading a mixed scalar/string frame must succeed";
  if (got_small != small || got_medium != medium || got.value != value ||
      got_character != character || got_large != large ||
      !TestEq(text, got_text) || got.guard != 0xAABBCCDD)
    return "mixed scalar/string round-trip must preserve every argument";

  if (TSlotLength<IUN>(input.data + 200, input.data + 8, 256) !=
      IUN(256 + 8 - 200))
    return "wrapped ring length must work with an unsigned result type";
  if (TSlotSpace<ISN>(input.data + 200, input.data + 8, 256) !=
      ISN(256 - (256 + 8 - 200)))
    return "wrapped free space must not form a pointer beyond the ring";
  if (SlotLength(input.data + 200, input.data + 8, 256) !=
      ISN(256 + 8 - 200))
    return "non-template ring length must agree with the template helper";
  if (BInEnd(&input.head) != input.data + sizeof(input.data))
    return "input end must point past the payload, not past the header";
  return NILP;
}
#endif

static const CHA* CrabsOp(const CHA* args) {
  #if SEAM == CRABS_OP
  A_TEST_BEGIN;

  enum {
    BooferBytes = 2048,
    BooferWords = BooferBytes / sizeof(IUW),
    StackTotal = 8,
  };
  D_COUT("\nInitializing Crabs Machine");
  IUW boofer[BooferWords],
      unpacked_expr[BooferWords];
  StarTest star_test;
  IUD pc_ctx = 0;
  Crabs* crabs = CrabsInit(boofer, BooferBytes, StackTotal, &star_test,
                           unpacked_expr, BooferBytes, pc_ctx);
  TCrabsPrint<::_::COut>(StdOut(), crabs);

  D_COUT("\n|    Testing Root (@see \"a.h\")...\n");

  void* dez_args[4];
  uint8_t io_number_ = 98;  //< ASCII:'b'
  TBIn<ISC>* bin = CrabsBIn(crabs);
  TBOut<ISC>* bout = CrabsBOut(crabs);

  const Op* result;
  CrabsRingBell(crabs);
  CrabsAckBack(crabs);
  auto params = TTSQ<_IUA, _CHA, _FPD, _CHC>();
  D_COUT("\nBefore:");
  TBOutPrintVars<::_::COut>(StdOut(), bout);
  result = BOutWrite(bout, params, Args(dez_args, TAddress<'A', 'A', 'A'>(),
    &io_number_, "Test", TAddress<_BS, _CR>()), pc_ctx);
  D_COUT("\nAfter:");
  TBOutPrintVars<::_::COut>(StdOut(), bout);
  A_ASSERT(!result);

  TCrabsPrint<::_::COut>(StdOut(), crabs);
  
  //Mirror mirror (bin, bout);
  //CrabsScan(crabs);  //, &mirror);
  //Bypass handshake for testing purposes.
  CrabsScanBIn(crabs);
  TCrabsPrint<::_::COut>(StdOut(), crabs);
  D_COUT("\n| Done with Crabs tests.");
#endif
  return NILP;
}
}  //< namespace CRTest
