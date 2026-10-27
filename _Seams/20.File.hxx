// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_FILE
#include "../File.h"
#if SEAM == CRABS_FILE
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
using namespace ::_;
namespace CRTest {

static const CHA* File(const CHA* args) {
#if SEAM >= CRABS_FILE
  A_TEST_BEGIN;

  static const CHR* TestURI = "_Seams/Test.crabs";

  ::_::File file(TestURI);

  D_COUT("\nTesting File(\"" << TestURI << "\")");

  A_AVOW(TestURI, file.URI());

  A_ASSERT(file.Exists());

  A_ASSERT(!file.IsOpen());

  A_AVOW(ISN(0), file.Open(TestURI));

  A_ASSERT(file.IsOpen());

  D_({
    
  })

  file.Close();

  A_ASSERT(!file.IsOpen());

  // Exercise the Folder API with the ASCII Data Type ISW index.
  D_COUT("\nTesting Folder(\"_Seams/TestFolder\")");
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
  A_AVOW(ISN(0), folder.OpenSorted(TestFolderURI));
  // Directories sort first: 0=".", 1="..", 2="sub"; then files by name:
  // 3="aaa.crabs", 4="bbb.crabs", 5="ccc.crabs".
  ::_::File entry;
  A_AVOW(ISN(0), folder.ReadFile(&entry, 3));
  A_ASSERT(TSEquals<CHR>(entry.Name(), "aaa.crabs") != NILP);
  A_ASSERT(entry.is_reg_ != 0);
  A_AVOW(ISN(0), folder.ReadFile(&entry, 4));
  A_ASSERT(TSEquals<CHR>(entry.Name(), "bbb.crabs") != NILP);
  A_AVOW(ISN(0), folder.ReadFile(&entry, 5));
  A_ASSERT(TSEquals<CHR>(entry.Name(), "ccc.crabs") != NILP);
  // Out-of-range index is an error.
  A_ASSERT(folder.ReadFile(&entry, 99) < 0);
  A_AVOW(ISN(0), folder.OpenSubfolder(2));
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
  D_COUT("\nTesting File write/read round-trip(\"" << TestIOURI << "\")");
  {
    ::_::File out;
    A_AVOW(ISN(0), out.OpenWrite(TestIOURI));
    A_ASSERT(out.IsOpen());
    out << "abc" << 123 << 4.5;
    out.Close();
    A_ASSERT(!out.IsOpen());
  }
  {
    ::_::File in;
    A_AVOW(ISN(0), in.OpenRead(TestIOURI));
    A_ASSERT(in.IsOpen());
    // Expected: "abc" + "123" + float render of 4.5 (Puff/TDecimalFloat).
    CHA int_sock[24];
    A_ASSERT((TSPrintSigned<ISD, IUD, CHA>(int_sock, 24, ISD(123)) != NILP));
    CHA float_sock[32];
    A_ASSERT((TSPrint<CHA>(float_sock, 32, 4.5) != NILP));
    CHA expected[128];
    SPrint(expected, 128, "abc");
    TSConcat<CHR, CHC>(expected, 128, int_sock);
    TSConcat<CHR, CHC>(expected, 128, float_sock);
    ISW size = in.Size();
    A_ASSERT(size > 0);
    A_AVOW(ISW(0), in.Seek(0, SEEK_SET));
    CHA buf[128];
    buf[127] = 0;
    ISW got = in.Read(buf, 127);
    in.Close();
    A_ASSERT(!in.IsOpen());
    A_ASSERT(got > 0);
    A_ASSERT((TSEquals<CHR>(buf, expected) != NILP));
  }
#ifndef _WIN32
  ::remove(TestIOURI);
#endif

  D_COUT("\nFile test complete.");
#endif
  return NILP;
}

}  //< namespace CRTest
