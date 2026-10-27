# Low-Level Task — File API: make `<<` write and add `>>` read (real I/O)

From: astar-mary (director). To: Low Level (this session).
Read `Agents/LowLevelContext.md` FIRST — it's your own pre-compaction dump and
your in-flight work is the base. You already did the ASCII-Data-Type migration
(size_t→ISW, malloc→ObjectFactoryHeap, OS includes consolidated into File.h,
ArrayCopy/ArrayFill for memory ops, TSEquals polarity fix) and it's verified
passing. This task is the NEXT increment on top of that: make File actually do
I/O.

## The one-sentence goal
Make `File` behave like `std::cout`/`std::cin`: `file << 123 << "hello"` WRITES
to the file, `file >> ...` READS from it. Currently the `File::Write(...)`
overloads are no-ops (`return *this;`) and there is NO `operator>>` (read) at
all. Implement the real I/O.

## Target API (the operator-overload set)
`namespace _`, types CHA/ISC/IUC/ISD/IUD/FPC/FPD/ISW/ISN/BOL. SEAM-gated under
`CRABS_FILE` (=20). Keep the OS stdio (fopen/fread/fwrite/fseek/ftell/fclose) as
an ASCIICrabs-INTERNAL detail (File.hxx only) — the public surface is only the
`_::` operator/set, matching how File.hxx already uses <dirent.h>/<sys/stat.h>.

### File (struct, File.h decl + File.hxx impl)
- Store a real stream handle. Add a private `FILE* fp_` (OS detail; the public
  surface never exposes FILE*). `Open` must actually open the stream.
  - Keep the existing `Open(const CHR* path)` stat/exists behavior for the
    directory-scan path (Folder uses it) — but ADD the ability to open for
    reading and for writing. Suggested: `Open` keeps its meaning; add
    `ISN OpenRead(const CHR* path);` and `ISN OpenWrite(const CHR* path);`
    (or an open-mode enum) that set `fp_`. Do not break Folder's use of Open.
- WRITE via `<<`: the `::_::File& operator<<(::_::File& o, ...)` free functions
  are ALREADY DECLARED (File.h lines ~286-311) but have NO bodies. Implement
  them to write to `fp_`. Int-to-string MUST be Puff-powered — reuse the
  existing `TSPrintSigned`/`TSPrintUnsigned`/`TSPrint` socket-buffer idiom that
  `TextFile::Print` already uses (File.hxx ~line 403-470), then fwrite the
  rendered bytes. Do NOT re-invent int-to-string.
  Overloads needed (match the declared set): const CHA* (string), CHA, ISC,
  IUC, ISD, IUD, FPC, FPD. A trailing `File& operator<<(File&, NL)` or a
  `Newline()`/`NL()` for line ends (consumers do `file << ... << "\n"` today, so
  a `const CHA*`/`CHA` overload already covers `"\n"` — keep it simple).
- Also implement the member `File::Write(...)` overloads to route to the same
  `fp_` write (they're currently no-ops). Make `TextFile::Print`/`TextFile::
  operator<<` actually write to the file's `fp_` too (verify the socket buffer
  reaches the file, not just a scratch buffer — this is the "File::Write(const
  CHA*) is an unimplemented no-op" gap from your context file).
- READ via `>>`: ADD `operator>>` overloads (there are none). Needed for the
  consumers below:
  - `File& operator>>(File&, void* buf, ISW bytes)` — raw binary read (fread).
  - `ISW File::Seek(ISW offset, int whence)` + `ISW File::Size()` (ftell/
    fseek) so a caller can size a whole-file read (SEEK_END→ftell→SEEK_SET→
    fread), mirroring the consumer's current ReadFile.
  - Optionally `File& operator>>(File&, ISC&)` / `(File&, FPD&)` for text
    reads, but the binary + seek + size is what the consumers actually need.
- `Close()` must fclose `fp_` (guard: only if open).

### Do NOT
- Do NOT add MemCpy/MemSet/MemMove (they don't exist; use ArrayCopy/ArrayFill —
  your context file already says this).
- Do NOT change the ASCII-Data-Type migration you already did (ISW, etc.).
- Do NOT touch Array.h/Array.hxx/Math.h/Math.hxx/Stringf.h/Stringf.hxx (those
  are astar-mary's, and I am finishing the MUSEWorld conversion on them).
- Scope strictly to File.h / File.hxx / _Seams/20.File.hxx (per your own
  context-file coordination note). Re-read each immediately before patching
  (astar-mary may have the tree open).
- No git commit/push (Captain owns git). No new files unless this task needs a
  test fixture (you're allowed the _Seams/20.File.hxx test edits).

## Concrete consumers (this is what "done" means)
The MUSEWorld C++ strip (astar-mary, finishing now) has two files that today
use raw C stdio and will switch to your API. Your API must let them do exactly
this (translate the current C into the new operators):

1. **Whole-file binary read** (OrderCompare.hxx `ReadFile`):
   ```
   // today:
   FILE* f = fopen(path, "rb"); fseek(f,0,SEEK_END); long sz=ftell(f);
   fseek(f,0,SEEK_SET); char* buf=new char[sz+1]; size_t got=fread(buf,1,sz,f);
   buf[got]=0; fclose(f);
   // must become something like:
   _::File in(path);  in.OpenRead(path);
   ISW sz = in.Size();
   CHA* buf = (CHA*)ObjectFactoryHeap(NILP, sz + 1);   // or the in-house alloc
   ISW got = 0;  in >> (buf, sz);   // or in.Seek(0,SEEK_SET); in >> buf, sz;
   buf[got] = 0;  in.Close();
   ```
   (You decide the exact read-call shape; the requirement is: open-for-read,
   get size, read sz raw bytes into a caller buffer, close. No FILE* leaks.)

2. **Text/JSON line writes** (OrderCompare.hxx curve_log):
   ```
   // today: snprintf(path,...); FILE* f=fopen(path,"w");
   //        fprintf(f, "{\"order\":\"%s\",\"step\":%d,...}\n", ...);
   //        fclose(f);
   // must become:
   _::File out;  out.OpenWrite(path);
   out << "{\"order\":\"" << order_name << "\",\"step\":" << step
       << ",\"loss\":" << loss << "}" << "\n";
   out.Close();
   ```
   (The int/float-to-string must be Puff-correct: %d → ISC/ISD, %.6f → FPD.
   Verify the rendered float has the right digits.)

3. **Raw random-access binary read** (HeadWaveInstrument.h safetensors reader):
   open-for-read, `Seek(offset, SEEK_SET)`, read N raw bytes (BF16 tensor),
   close. Same read + seek + size as (1).

## Acceptance (raw evidence, not a plan)
- `g++ -std=gnu++23 -x c++ -g -DSEAM=20 -I _Seams -I . -o /tmp/crabs20 _Seams/
  _Main.hxx` builds clean; the 20.File.hxx seam test exercises a WRITE then a
  READ round-trip (write a known string + an int + a float to a temp file, read
  it back, A_AVOW the round-trip, then delete the temp file) — and it PASSES
  ("Unit tests completed successfully!").
- Full tree: `g++ -std=gnu++23 -x c++ -g -I _Seams -I . -o Crabs _Seams/_Main.
  hxx && ./Crabs` builds clean and passes (SEAM=23).
- A 3-line proof that the two consumer patterns above (whole-file read + JSON
  line write) compile against your new API (a tiny throwaway in /tmp, show me
  the source + the clean compile).
- Report: exact signatures of the new Open/OpenRead/OpenWrite/Seek/Size/
  operator<< /operator>> so astar-mary can wire OrderCompare.hxx +
  HeadWaveInstrument.h to them.

## Coordination
- You own File.h / File.hxx / _Seams/20.File.hxx. astar-mary owns
  Array/Math/Stringf + the MUSEWorld *.h conversion. Do not cross those.
- When your round-trip test passes, ping the Captain (Astarcale) with the
  signature report; astar-mary will then finish the two MUSEWorld files against
  your API. Windows/MSVC path can stay untested (Linux GCC only, per your
  context note).
