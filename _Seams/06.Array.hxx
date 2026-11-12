// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsArray
#include "../Array.hpp"
#if _Seam == _CrabsArray
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

#if _Seam >= _CrabsArray
template<typename T, typename ISZ>
void TestArray(const CHA* args) {
  _DROut("Testing AArray<ISZ" << CHA('@' + sizeof(T)) << ",ISZ" << 
         CHA('@' + sizeof(ISZ)) << ">...\n");

  AArray<T, ISZ, 8> array;

  _DROut(Linef("\n+---\n| AArray: total:") << array.Total() << 
         " bytes:" << array.CBytes() << " size_words:" << array.SizeWords());
  _DROutOBJ(array);
}
#endif

static const CHA* Array(const CHA* args) {
#if _Seam >= _CrabsArray
  _ATestBegin;

  /* ArrayFill regression: FillWord skipped (value << 40), so with a non-zero
     fill byte the 6th byte of every 64-bit word kept its old value. This
     walks the aligned body and both unaligned edges and also checks that the
     fill does not touch the bytes outside [begin, begin + count). */
  _DROut(Headingf("Testing ArrayFill..."));
  {
    IUA buffer[3 * ACPUBytes + 3] = {};
    const IUA fill_byte = IUA(0xA5);
    IUA* begin = buffer + 3;
    const ISW count = ISW(2 * ACPUBytes + 1);
    ArrayFill(begin, count, CHA(fill_byte));
    for (ISW i = 0; i < count; ++i)
      _AAssert(begin[i] == fill_byte);
    for (ISW i = 0; i < 3; ++i)
      _AAssert(buffer[i] == 0);
    for (ISW i = 3 + count; i < ISW(sizeof(buffer)); ++i)
      _AAssert(buffer[i] == 0);
  }

  _DROut(Headingf("Testing RAMCompare with offset sequential arrays..."));

  enum {
    OffsetMax = 2 * ACPUBytes,
    TestByteCount = 256,
    BufferSizeBytes = TestByteCount + 2 * OffsetMax,
    BufferSizeWords = BufferSizeBytes >> ACPUBytesLog2,
  };
  IUW source[BufferSizeWords],
      destination[BufferSizeWords + OffsetMax];
  for (ISN s_offset = 0; s_offset < OffsetMax; ++s_offset) {
    // Create an array of bytes valued sequentially 0 through 255.
    // i is the offset of the source array and d_offset is the offset of the 
    // destination array so we can check all memory alignment configurations.
    IUA* cursor  = TRef<IUA>(source),
       * s_end   = cursor + BufferSizeBytes,
       * s_start = TRef<IUA>(source) + s_offset;
    // @todo We only need to zero out the i-d_offset bytes
    while (cursor < s_start) *cursor++ = 0;
    for (ISN i = 0; i < TestByteCount; ++i) *cursor++ = ISA(i);
    while (cursor < s_end) *cursor++ = 0;
    //_DROut(Charsf(source, cBufferSizeBytes));
    // Copy those bytes into a buffer with an offset to exhaustively test each 
    // memory layout variation modulo the word size.
    for (ISN d_offset = 0; d_offset < OffsetMax; ++d_offset) {
      cursor = TRef<IUA>(destination);
      IUA * d_start = cursor + d_offset,
          * d_end = d_start + BufferSizeBytes;
      while (cursor < d_start) *cursor++ = 0;
      for (ISN i = 0; i < TestByteCount; ++i)
        *cursor++ = ISA(i);
      while (cursor < d_end) *cursor++ = 0;

      ISW result = ArrayCompareSlow(s_start, TestByteCount, d_start, 
                                    TestByteCount);
      if (result <= 0) {
        result = result * -1 - 1;
        _DROut("\n\nArrayCompareSlow failed at byte " << result << 
               " with s_offset:" << s_offset << " d_offset:" << d_offset <<
               "\na_cursor[-1]: " << Hexf(*(s_start + result - 1)) <<
               "  b_cursor[-1]: " << Hexf(*(d_start + result - 1)) <<
               "\na_cursor[ 0]: " << Hexf(*(s_start + result    )) <<
               "  b_cursor[ 0]: " << Hexf(*(d_start + result    )) <<
               "\na_cursor[+1]: " << Hexf(*(s_start + result + 1)) <<
               "  b_cursor[+1]: " << Hexf(*(d_start + result + 1)) <<
               "\n\nsource:"      << Charsf(source, BufferSizeBytes) <<
               "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
        _AAssert(result < 0);
      }
      for (ISN k = TestByteCount - 1; k > 0; --k) {
        result = ArrayCompareFast(s_start, ISW(TestByteCount) - k, d_start,
                                  ISW(TestByteCount) - k);
        if (result <= 0) {
          result = result * -1 - 1;
          _DROut("\n\nArrayCompareFast failed at byte " << result << 
                 " with s_offset:" << s_offset << " d_offset:" << d_offset << 
                 "\na_cursor[-1]: " << Hexf(*(s_start + result - 1)) << 
                 "  b_cursor[-1]: " << Hexf(*(d_start + result - 1)) << 
                 "\na_cursor[ 0]: " << Hexf(*(s_start + result    )) << 
                 "  b_cursor[ 0]: " << Hexf(*(d_start + result    )) << 
                 "\na_cursor[+1]: " << Hexf(*(s_start + result + 1)) << 
                 "  b_cursor[+1]: " << Hexf(*(d_start + result + 1)) << 
                 "\n\nsource:"      << Charsf(source, BufferSizeBytes) << 
                 "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
          _AAssert(result < 0);
        }
      }
    }
  }

  _DROut(Headingf("Test ArrayCopy"));

  for (IUW s_offset = 0; s_offset < OffsetMax; ++s_offset) {
    for (IUW d_offset = 0; d_offset < OffsetMax; ++d_offset) {
      //_DROut("\n\ns_offset: " << s_offset << "  d_offset: " << d_offset);
      ArrayFill(destination, BufferSizeBytes, 0);
      IUA* cursor = TRef<IUA>(source);

      for (IUW i = 0; i < s_offset; ++i) *cursor++ = 0;
      for (IUW i = 0; i < TestByteCount; ++i) *cursor++ = IUA(i);
      for (IUW i = 0; i < OffsetMax - s_offset; ++i) *cursor++ = 0;
      IUA *s_start = TRef<IUA>(source) + s_offset,
          *d_start = TRef<IUA>(destination) + d_offset;
      
      ISW result = ArrayCopyFast(d_start, TestByteCount, s_start, 
                                 TestByteCount);
      _AAssert(result == TestByteCount);
      //_DROut("\n\nsource:" << Charsf(source, BufferSizeBytes) << 
      //       "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
      result = ArrayCompareSlow(s_start, TestByteCount, d_start, TestByteCount);
      if (result <= 0) {
        _DROut("\n\nArrayCompareFast failed at byte: " << result << 
               "\n\nsource:" << Charsf(source, BufferSizeBytes) << 
               "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
        _AAssert(result >= 0);
      }
      d_start = TRef<IUA>(destination);
      for (IUW i = 0; i < d_offset; ++i) {
        IUA current = *d_start++;
        if (current != 0) {
          _DROut("\n\nArrayCompareFast printed bad data at byte: " << i <<
                 "\nd_offset: " << d_offset <<
                 "\n\nsource:" << Charsf(source, BufferSizeBytes) << 
                 "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
          _AAssert(current == 0);
        }
      }
      d_start = TRef<IUA>(destination) + d_offset + TestByteCount;
      IUA* d_end = TRef<IUA>(destination) + BufferSizeBytes;
      for (IUW i = 0; i < (OffsetMax - d_offset); ++i) {
        IUA current = *d_start++;
        if (current != 0) {
          _DROut("\n\nArrayCompareFast printed bad data at byte: "
                 << TDelta(destination, d_start)
                 << "\n\nsource:" << Charsf(source, BufferSizeBytes)
                 << "\n\ndestination:" << Charsf(destination, BufferSizeBytes));
          _AAssert(current == 0);
        }
      }
    }
  }
  _DROut(Headingf("TestArray"));

  // TestArray<ISC, ISD>(args); is illegal
  TestArray<ISC, ISC>(args);
  TestArray<ISD, ISD>(args);
  TestArray<ISD, ISC>(args);
#endif
  return _NilP;
}
}  //< namespace CRTest
