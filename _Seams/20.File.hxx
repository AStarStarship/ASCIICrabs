// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsFile
#include "../File.h"
#if _Seam == _CrabsFile
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#ifndef _WIN32
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#endif
namespace CRTest {
using namespace ::_;

static const CHA* File(const CHA* args) {
#if _Seam >= _CrabsFile
  _ATestBegin;

  static const CHR* TestURI = "_Seams/Test.crabs";

  ::_::File file(TestURI);

  _DROut("\nTesting File(\"" << TestURI << "\")");

  _AAvow(TestURI, file.URI());

  _AAssert(file.Exists());

  _AAssert(!file.IsOpen());

  _AAvow(ISN(0), file.Open(TestURI));

  _AAssert(file.IsOpen());

  _D({
    
  })

  file.Close();

  _AAssert(!file.IsOpen());

  // Exercise the Folder API with the ASCII Data Type ISW index.
  _DROut("\nTesting Folder(\"_Seams/TestFolder\")");
  static const CHR* TestFolderURI = "_Seams/TestFolder";
#ifndef _WIN32
  // Clean slate in case an earlier run crashed mid-test.
  ::remove("_Seams/TestFolder/ccc.crabs");
  ::remove("_Seams/TestFolder/aaa.crabs");
  ::remove("_Seams/TestFolder/bbb.crabs");
  ::rmdir("_Seams/TestFolder/sub");
  ::rmdir("_Seams/TestFolder");
  // Files are created out of order so the sort is actually exercised.
  ::mkdir("_Seams/TestFolder", 0755);
  ::close(::open("_Seams/TestFolder/ccc.crabs", O_CREAT | O_WRONLY, 0644));
  ::close(::open("_Seams/TestFolder/aaa.crabs", O_CREAT | O_WRONLY, 0644));
  ::close(::open("_Seams/TestFolder/bbb.crabs", O_CREAT | O_WRONLY, 0644));
  ::mkdir("_Seams/TestFolder/sub", 0755);
#endif
  Folder folder(TestFolderURI);
  _AAvow(ISN(0), folder.OpenSorted(TestFolderURI));
  // Directories sort first: 0=".", 1="..", 2="sub"; then files by name:
  // 3="aaa.crabs", 4="bbb.crabs", 5="ccc.crabs".
  ::_::File entry;
  _AAvow(ISN(0), folder.ReadFile(&entry, 3));
  _AAssert(TSEquals<CHR>(entry.Name(), "aaa.crabs") != _NilP);
  _AAssert(entry.is_reg_ != 0);
  _AAvow(ISN(0), folder.ReadFile(&entry, 4));
  _AAssert(TSEquals<CHR>(entry.Name(), "bbb.crabs") != _NilP);
  _AAvow(ISN(0), folder.ReadFile(&entry, 5));
  _AAssert(TSEquals<CHR>(entry.Name(), "ccc.crabs") != _NilP);
  // Out-of-range index is an error.
  _AAssert(folder.ReadFile(&entry, 99) < 0);
  _AAvow(ISN(0), folder.OpenSubfolder(2));
  folder.Close();
#ifndef _WIN32
  ::remove("_Seams/TestFolder/ccc.crabs");
  ::remove("_Seams/TestFolder/aaa.crabs");
  ::remove("_Seams/TestFolder/bbb.crabs");
  ::rmdir("_Seams/TestFolder/sub");
  ::rmdir("_Seams/TestFolder");
#endif

  // WRITE-then-READ round-trip: operator<< writes, Read() reads.
  static const CHR* TestIOURI = "_Seams/TestIO.crabs";
  _DROut("\nTesting File write/read round-trip(\"" << TestIOURI << "\")");
  {
    ::_::File out;
    _AAvow(ISN(0), out.OpenWrite(TestIOURI));
    _AAssert(out.IsOpen());
    out << "abc" << 123 << 4.5;
    out.Close();
    _AAssert(!out.IsOpen());
  }
  {
    ::_::File in;
    _AAvow(ISN(0), in.OpenRead(TestIOURI));
    _AAssert(in.IsOpen());
    // Expected: "abc" + "123" + float render of 4.5 (Puff/TDecimalFloat).
    CHA int_sock[24];
    _AAssert((TSPrintSigned<ISD, IUD, CHA>(int_sock, 24, ISD(123)) != _NilP));
    CHA float_sock[32];
    _AAssert((TSPrint<CHA>(float_sock, 32, 4.5) != _NilP));
    CHA expected[128];
    SPrint(expected, 128, "abc");
    TSConcat<CHR, CHC>(expected, 128, int_sock);
    TSConcat<CHR, CHC>(expected, 128, float_sock);
    ISW size = in.Size();
    _AAssert(size > 0);
    _AAvow(ISW(0), in.Seek(0, SEEK_SET));
    CHA buf[128];
    buf[127] = 0;
    ISW got = in.Read(buf, 127);
    in.Close();
    _AAssert(!in.IsOpen());
    _AAssert(got > 0);
    _AAssert((TSEquals<CHR>(buf, expected) != _NilP));
  }
#ifndef _WIN32
  ::remove(TestIOURI);
#endif

  _DROut("\nFile test complete.");
#endif
  return _NilP;
}

}  //< namespace CRTest
