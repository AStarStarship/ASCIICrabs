// Copyright AStarship <https://astarship.net>.
// Based on TinyDir, a C library for reading directories transformed.
#include "File.h"
#include "Array.h"
#include "Autoject.h"
#if SEAM >= CRABS_FILE
#include "Stringf.hpp"
#ifdef _MSC_VER
enum {
  Win32FindDataBytes = 592
};
#endif

#ifdef _WIN32
#define _TINYDIR_DRIVE_MAX 3
#endif

namespace _ {

File::File(const CHR* uri) : is_directory_(0), is_reg_(0), path(), name(),
                             uri_(uri), is_open_(0), extension(NILP),
                             last_time_modified_(0), fp_(NILP) {}

File& File::Star() { return *this; }

TextFile File::Text() {
  static TextFile text_file(*this);
  return text_file;
}

const CHR* File::Extension() {
  // Directory-scan entries carry their filename in `name` with a nil uri_;
  // URI-constructed files have the opposite.
  const CHR* source = name[0] ? name : uri_;
  const CHR* ext = TSFindLast<CHR>(source, '.');
  if (ext != NILP) ++ext;
  return ext;
}

inline const CHR* File::URI() { return uri_; }

inline const CHR* File::URISet(const CHR* uri) {
  if (IsError(uri)) return uri;

  uri_ = uri;

  return uri;
}

inline TME File::LastTimeModified() {
  struct stat result;
  if (stat(uri_, &result) == 0) {
    // Explicit narrowing at the OS boundary: time_t -> ASCII timestamp.
    return TME(result.st_mtime);
  }
  return 0;
}

inline BOL File::HasBeenModified() {
  TME last_time_modified = LastTimeModified();
  if (last_time_modified == last_time_modified_) return 0;
  last_time_modified_ = last_time_modified;
  return last_time_modified;
}

inline BOL File::Exists() {
  return uri_ ? ::access(uri_, F_OK) == 0 : false;
}

inline BOL File::IsOpen() { return is_open_ != 0; }

inline ISN File::Open(const CHR* path) {
  if (IsError(path) || TSCodeCount<CHR, CHC, ISW>(path) == 0) {
    return -AErrorInvalidInput;
  }
  Folder dir(path);
  ISN result = 0;
  ISN found = 0;
  CHR dir_name_buf[URIPathLengthMax];
  CHR file_name_buf[URLFilenameLengthMax];
  CHR* dir_name;
  CHR* base_name;
#if (defined _MSC_VER || defined __MINGW32__)
  CHR drive_buf[URIPathLengthMax];
  CHR ext_buf[URLFilenameLengthMax];
#endif

  if (TSCodeCount<CHR, CHC, ISW>(path) + FilenamePad >= URIPathLengthMax) {
    errno = ENAMETOOLONG;
    return -1;
  }

  /* Get the parent path */
#if (defined _MSC_VER || defined __MINGW32__)
#if ((defined _MSC_VER) && (_MSC_VER >= 1400))
  errno = TSSplitPath<CHR, CHC>(path, drive_buf, _TINYDIR_DRIVE_MAX, dir_name_buf,
    URLFilenameLengthMax, file_name_buf, URLFilenameLengthMax,
    ext_buf, URLFilenameLengthMax);
#else
  _tsplitpath(path, drive_buf, dir_name_buf, file_name_buf, ext_buf);
#endif

  if (errno)
    return -1;

  // _splitpath_s not work fine with only filename and widechar support.
#ifdef _UNICODE
  if (drive_buf[0] == L'\xFEFE') drive_buf[0] = '\0';
  if (dir_name_buf[0] == L'\xFEFE') dir_name_buf[0] = '\0';
#endif

  // Emulate the behavior of dirname by returning "." for dir name if it's
  // empty
  if (drive_buf[0] == '\0' && dir_name_buf[0] == '\0') {
    SPrint(dir_name_buf, URIPathLengthMax, '.');
  }
  // Concatenate the drive letter and dir name to form full dir name.
  TSConcat<CHR, CHC>(drive_buf, URIPathLengthMax, dir_name_buf);
  dir_name = drive_buf;
  // Concatenate the file name and extension to form base name.
  TSConcat<CHR, CHC>(file_name_buf, URLFilenameLengthMax, ext_buf);
  base_name = file_name_buf;
#else
  SPrint(dir_name_buf, URIPathLengthMax, path);
  dir_name = dirname(dir_name_buf);
  SPrint(file_name_buf, URIPathLengthMax, path);
  base_name = basename(file_name_buf);
#endif

  // Open the parent directory.
  if (dir.Open(dir_name) == -1) {
    return -1;
  }

  // Read through the parent directory and look for the file.
  while (dir.HasNext()) {
    File entry;
    ISN read = dir.Read(&entry);
    if (read < 0) {
      result = -1;
      goto bail;
    }
    if (TSEquals<CHR>(entry.Name(), base_name) != NILP) {
      found = 1;
      break;
    }
    dir.Next(&dir);
  }
  if (!found) {
    result = -1;
    errno = ENOENT;
  }

bail:
  dir.Close();
  is_open_ = result == 0 ? 1 : 0;
  return result;
}

ISN File::OpenRead(const CHR* path) {
  if (IsError(path) || TSCodeCount<CHR, CHC, ISW>(path) == 0) {
    errno = EINVAL;
    return -AErrorInvalidInput;
  }
  if (fp_ != NILP) {
    fclose(fp_);
    fp_ = NILP;
  }
  fp_ = fopen(path, "rb");
  if (fp_ == NILP) {
    return -1;
  }
  is_open_ = 1;
  return 0;
}

ISN File::OpenWrite(const CHR* path) {
  if (IsError(path) || TSCodeCount<CHR, CHC, ISW>(path) == 0) {
    errno = EINVAL;
    return -AErrorInvalidInput;
  }
  if (fp_ != NILP) {
    fclose(fp_);
    fp_ = NILP;
  }
  fp_ = fopen(path, "wb");
  if (fp_ == NILP) {
    return -1;
  }
  is_open_ = 1;
  return 0;
}

ISW File::Read(void* buf, ISW bytes) {
  if (IsError(buf) || bytes < 0) {
    errno = EINVAL;
    return 0;
  }
  if (fp_ == NILP) {
    errno = EBADF;
    return 0;
  }
  if (bytes == 0) {
    return 0;
  }
  return ISW(fread(buf, 1, bytes, fp_));
}

ISW File::Seek(ISW offset, ISN whence) {
  if (fp_ == NILP) {
    errno = EBADF;
    return 0;
  }
  if (fseek(fp_, offset, whence) == -1) {
    return 0;
  }
  return ISW(ftell(fp_));
}

ISW File::Size() {
  if (fp_ == NILP) {
    errno = EBADF;
    return 0;
  }
  ISW position = ISW(ftell(fp_));
  if (position < 0) {
    return 0;
  }
  if (fseek(fp_, 0, SEEK_END) == -1) {
    return 0;
  }
  ISW size = ISW(ftell(fp_));
  if (size < 0 || fseek(fp_, position, SEEK_SET) == -1) {
    return 0;
  }
  return size;
}

BOL File::CompareNames(const File* other) {
  return TSEquals<CHR>(name, other->name) != NILP;
}

inline void File::Close() {
  if (fp_ != NILP) {
    fclose(fp_);
    fp_ = NILP;
  }
  is_open_ = 0;
}

inline File& File::Write(CHA item) {
  if (fp_ != NILP) {
    fputc(item, fp_);
  }
  return *this;
}

inline File& File::Write(const CHA* item) {
  if (fp_ != NILP) {
    fwrite(item, 1, TSCodeCount<CHA, CHC, ISW>(item), fp_);
  }
  return *this;
}

#if USING_STB == YES_0
inline File& File::Write(CHB item) {
  return Write(CHA(item));
}

inline File& File::Write(const CHB* item) {
  while (*item != 0) {
    Write(CHA(*item++));
  }
  return *this;
}

#endif
#if USING_STC == YES_0
inline File& File::Write(CHC item) {
  return Write(CHA(item));
}

inline File& File::Write(const CHC* item) {
  while (*item != 0) {
    Write(CHA(*item++));
  }
  return *this;
}

#endif

inline File& File::Write(ISA item) {
  return Write(ISD(item));
}

inline File& File::Write(ISB item) {
  return Write(ISD(item));
}

inline File& File::Write(ISC item) {
  return Write(ISD(item));
}

inline File& File::Write(ISD item) {
  if (fp_ == NILP) {
    return *this;
  }
  enum { Size = 24 };
  CHA socket[Size];
  TSPrintSigned<ISD, IUD, CHA>(socket, Size, item);
  Write(socket);
  return *this;
}

inline File& File::Write(IUA item) {
  return Write(IUD(item));
}

inline File& File::Write(IUB item) {
  return Write(IUD(item));
}

inline File& File::Write(IUC item) {
  return Write(IUD(item));
}

inline File& File::Write(IUD item) {
  if (fp_ == NILP) {
    return *this;
  }
  enum { Size = 24 };
  CHA socket[Size];
  TSPrintUnsigned<IUD, CHA>(socket, Size, item);
  Write(socket);
  return *this;
}

inline File& File::Write(FPC item) {
  if (fp_ == NILP) {
    return *this;
  }
  enum { Size = 32 };
  CHA socket[Size];
  TSPrint<CHA>(socket, Size, item);
  Write(socket);
  return *this;
}

inline File& File::Write(FPD item) {
  if (fp_ == NILP) {
    return *this;
  }
  enum { Size = 32 };
  CHA socket[Size];
  TSPrint<CHA>(socket, Size, item);
  Write(socket);
  return *this;
}



TextFile::TextFile(File& file) : file_(file) {}

TextFile& TextFile::Star() { return *this; }

const CHR* TextFile::Extension() { return file_.Extension(); }

inline const CHR* TextFile::URI() { return file_.URI(); }

inline const CHR* TextFile::URISet(const CHR* uri) { return file_.URISet(uri); }

inline TME TextFile::LastTimeModified() { return file_.LastTimeModified(); }

inline BOL TextFile::HasBeenModified() { return file_.HasBeenModified(); }

inline BOL TextFile::Exists() { return file_.Exists(); }

inline BOL TextFile::IsOpen() { return file_.IsOpen(); }

inline ISN TextFile::Open() { return file_.Open(file_.URI()); }

inline void TextFile::Close() { file_.Close(); }

TextFile& TextFile::Print(CHA item) {
  file_.Write(item);
  return *this;
}

TextFile& TextFile::Print(const CHA* item) {
  return TPrint<TextFile, CHA>(*this, item);
}

#if USING_STB == YES_0
TextFile& TextFile::Print(CHB item) {
  file_.Write(item);
  return *this;
}

TextFile& TextFile::Print(const CHB* item) {
  return TPrint<TextFile, CHB>(*this, item);
}
#endif

#if USING_STC == YES_0
TextFile& TextFile::Print(CHC item) {
  file_.Write(item);
  return *this;
}
TextFile& TextFile::Print(const CHC* item) {
  return TPrint<TextFile, CHC>(*this, item);
}
#endif

TextFile& TextFile::Print(ISC item) {
#if SEAM <= CRABS_ITOS
  STD_COUT << item;
#else
#if CPU_SIZE == CPU_8_BYTE
  Print(ISD(item));
#else
  enum { Size = 24 };
  CHA socket[Size];
  TSPrintSigned<ISD, IUD, CHA>(socket, Size, ISD(item));
  Print(socket);
#endif
#endif
  return *this;
}

TextFile& TextFile::Print(IUC item) {
#if SEAM <= CRABS_ITOS
  STD_COUT << item;
#else
#if CPU_SIZE == CPU_8_BYTE
  Print((item));
#else
  enum { Size = 24 };
  CHA boofer[Size];
  TSPrintUnsigned<IUC, CHA>(boofer, Size, item);
  Print(boofer);
#endif
#endif
  return *this;
}

TextFile& TextFile::Print(ISD item) {
#if SEAM <= CRABS_ITOS
  STD_COUT << item;
#else
  enum { Size = 24 };
  CHA boofer[Size];
  auto result = TSPrint<CHA>(boofer, Size, item);
  Print(boofer);
#endif
  return *this;
}

TextFile& TextFile::Print(IUD item) {
#if SEAM <= CRABS_ITOS
  STD_COUT << item;
#else
  enum { Size = 24 };
  CHA boofer[Size];
  TSPrint<CHA>(boofer, Size, item);
  Print(boofer);
#endif
  return *this;
}

TextFile& TextFile::Print(FPC item) {
#if SEAM <= CRABS_FTOS
  STD_COUT << item;
#else
  enum { Size = 16 };
  CHA boofer[Size];
  TSPrint<CHA>(boofer, Size, item);
  Print(boofer);
#endif
  return *this;
}

TextFile& TextFile::Print(FPD item) {
#if SEAM <= CRABS_FTOS
  STD_COUT << item;
#else
  enum { Size = 32 };
  CHA boofer[Size];
  TSPrint<CHA>(boofer, Size, item);
  Print(boofer);
#endif
  return *this;
}

TextFile& TextFile::Print(Hexf& item) {
  return TPrintHex<TextFile>(*this, item.element.Value(), item.element.count);
}

TextFile& TextFile::Print(Binaryf& item) {
  return TPrintBinary<TextFile>(*this, item.element.Value(), item.element.count);
}

TextFile& TextFile::Print(Centerf& item) {
  return TPrintCenter<TextFile>(*this, item.element);
}

TextFile& TextFile::Print(Rightf& item) {
  return TPrintRight<TextFile>(*this, item.element);
}

TextFile& TextFile::Print(Linef& item) { return TPrint<TextFile>(*this, item); }

TextFile& TextFile::Print(Repeatf& item) { return TPrint<TextFile>(*this, item); }

TextFile& TextFile::Print(Headingf& item) { return TPrint<TextFile>(*this, item); }

TextFile& TextFile::Print(Indentf& item) {
  return ::_::TPrintIndent<::_::TextFile>(*this, item.indent_count);
}

TextFile& TextFile::Print(Charsf& item) {
  return ::_::TPrint<TextFile>(*this, item);
}

TextFile& TextFile::NL() { return Print('\n'); }
TextFile& TextFile::NL(CHA item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(const CHA* item) {
  NL();
  return Print(item);
}
#if USING_STB == YES_0
TextFile& TextFile::NL(CHB item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(const CHB* item) {
  NL();
  return Print(item);
}
#endif
#if USING_STC == YES_0
TextFile& TextFile::NL(CHC item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(const CHC* item) {
  NL();
  return Print(item);
}
#endif

TextFile& TextFile::NL(ISC item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(IUC item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(ISD item) {
  NL();
  return Print(item);
}
TextFile& TextFile::NL(IUD item) {
  NL();
  return Print(item);
}
#if USING_FPC == YES_0
TextFile& TextFile::NL(FPC item) {
  NL();
  return Print(item);
}
#endif
#if USING_FPD == YES_0
TextFile& TextFile::NL(FPD item) {
  NL();
  return Print(item);
}
#endif

TextFile& TextFile::Hex(const void* item) { return TPrintHex<TextFile>(*this, item); }
TextFile& TextFile::Hex(const void* item, ISW size) {
  return TPrintHex<TextFile>(*this, item, size);
}
TextFile& TextFile::Hex(const void* item, const void* item_stop) {
  return TPrintHex<TextFile>(*this, item, item_stop);
}
TextFile& TextFile::Hex(ISA item) { return TPrintHex<TextFile, ISA, IUA>(*this, item); }
TextFile& TextFile::Hex(IUA item) { return TPrintHex<TextFile, IUA>(*this, item); }
TextFile& TextFile::Hex(ISB item) { return TPrintHex<TextFile, ISB, IUB>(*this, item); }
TextFile& TextFile::Hex(IUB item) { return TPrintHex<TextFile, IUB>(*this, item); }
TextFile& TextFile::Hex(ISC item) { return TPrintHex<TextFile, ISC, IUC>(*this, item); }
TextFile& TextFile::Hex(IUC item) { return TPrintHex<TextFile, IUC>(*this, item); }
TextFile& TextFile::Hex(ISD item) { return TPrintHex<TextFile, ISD, IUD>(*this, item); }
TextFile& TextFile::Hex(IUD item) { return TPrintHex<TextFile, IUD>(*this, item); }
#if USING_FPC == YES_0
TextFile& TextFile::Hex(FPC item) { return TPrintHex<TextFile>(*this, item); }
#endif
#if USING_FPD == YES_0
TextFile& TextFile::Hex(FPD item) { return TPrintHex<TextFile>(*this, item); }
#endif

/* Stores the given item to the boofer and store the size. */
TextFile& TextFile::Binary(const void* item) { return TPrintBinary<TextFile>(*this, item); }
TextFile& TextFile::Binary(const void* item, ISW size) {
  return TPrintBinary<TextFile>(*this, item, size);
}
TextFile& TextFile::Binary(const void* start, const void* stop) {
  return TPrintBinary<TextFile>(
    *this, start,
    TPtr<const CHA>(stop) - TPtr<const CHA>(start));
}
TextFile& TextFile::Binary(ISA item) {
  return TPrintBinary<TextFile, ISA, IUA>(*this, item);
}
TextFile& TextFile::Binary(IUA item) { return TPrintBinary<TextFile, IUA>(*this, item); }
TextFile& TextFile::Binary(ISB item) {
  return TPrintBinary<TextFile, ISB, IUB>(*this, item);
}
TextFile& TextFile::Binary(IUB item) { return TPrintBinary<TextFile, IUB>(*this, item); }
TextFile& TextFile::Binary(ISC item) {
  return TPrintBinary<TextFile, ISC, IUC>(*this, item);
}
TextFile& TextFile::Binary(IUC item) { return TPrintBinary<TextFile, IUC>(*this, item); }
TextFile& TextFile::Binary(ISD item) {
  return TPrintBinary<TextFile, ISD, IUD>(*this, item);
}
TextFile& TextFile::Binary(IUD item) { return TPrintBinary<TextFile, IUD>(*this, item); }
#if USING_FPC == YES_0
TextFile& TextFile::Binary(FPC item) { return TPrintBinary<TextFile>(*this, item); }
#endif
#if USING_FPD == YES_0
TextFile& TextFile::Binary(FPD item) { return TPrintBinary<TextFile>(*this, item); }
#endif

Folder::Folder(const CHR* uri) : path_() {
  SPrint(path_, URIPathLengthMax, uri);
}

ISN Folder::Open(const CHR* path) {
#ifndef _MSC_VER
#ifndef _TINYDIR_USE_READDIR
  ISN error;
  ISN size;
#endif
#else
  CHR path_buf[URIPathLengthMax];
#endif
  CHR* pathp;

  if (IsError(path) || TSCodeCount<CHR, CHC, ISW>(path) == 0) {
    errno = EINVAL;
    return AErrorInvalidInput;
  }
  if (TSCodeCount<CHR, CHC, ISW>(path) + FilenamePad >= URIPathLengthMax) {
    errno = ENAMETOOLONG;
    return AErrorInvalidInput;
  }

  // initialize dir.
  files_ = NILP;
#ifdef _MSC_VER
  _h = INVALID_HANDLE_VALUE;
#else
  _d = NILP;
  _e = NILP;
#endif
  Close();

  SPrint(path_, URIPathLengthMax, path);
  // Remove trailing slashes.
  pathp = const_cast<CHR*>(&path[TSCodeCount<CHR, CHC, ISW>(path) - 1]);
  CHR c = *pathp;
  while (pathp != path && (c == '\\' || c == '/')) {
    *pathp-- = 0;
  }
#ifdef _MSC_VER
  SPrint(path_buf, URIPathLengthMax, path);
  TSConcat<CHR, CHC>(path_buf, URIPathLengthMax, "\\*");
#if (defined WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
  _h = FindFirstFileEx(path_buf, FindExInfoStandard, &_f,
    FindExSearchNameMatch, NILP, 0);
#else
  _h = FindFirstFile(path_buf, &_f);
#endif
  if (_h == INVALID_HANDLE_VALUE) {
    errno = ENOENT;
#else
  _d = _og_opendir(path);
  if (_d == NILP) {
#endif
    goto bail;
  }

  // Read first file.
  has_next_ = 1;
#ifndef _MSC_VER
  _e = _og_readdir(_d);
  if (_e == NILP) {
    has_next_ = 0;
  }
#endif

  return 0;

bail:
  Close();
  return -1;
}

/* Compares two Folder entries without qsort: directories first, then the
names byte-wise with ASCII Data Types (the OS I/O shim stays syscall-only). */
static ISN FolderCompare(const File* a, const File* b) {
  if (a->is_directory_ != b->is_directory_) {
    return a->is_directory_ ? -1 : 1;
  }
  return 0 - TSCompare<CHR>(a->name, b->name);
}

/* In-place quicksort: middle pivot, the smaller partition recurses first so
the stack stays log-depth. */
static void FolderSort(File* files, ISN lo, ISN hi) {
  while (lo < hi) {
    const File pivot = files[(lo + hi) / 2];
    ISN i = lo, j = hi;
    do {
      while (FolderCompare(&files[i], &pivot) < 0) ++i;
      while (FolderCompare(&files[j], &pivot) > 0) --j;
      if (i <= j) {
        File tmp = files[i];
        files[i] = files[j];
        files[j] = tmp;
        ++i;
        --j;
      }
    } while (i <= j);
    if (j - lo < hi - i) {
      FolderSort(files, lo, j);
      lo = i;
    } else {
      FolderSort(files, i, hi);
      hi = j;
    }
  }
}

ISN Folder::OpenSorted(const CHR* path) {
  // Count the number of files first, to pre-allocate the files array.
  ISW file_count = 0;
  if (Open(path) == -1) return -1;
  while (has_next_) {
    ++file_count;
    if (Next(this) == -1)
      goto bail;
  }
  Close();

  if (file_count == 0 || Open(path) == -1) return -1;

  files_ = TPtr<File>(ObjectFactoryHeap(NILP, ISW(sizeof(File)) * file_count));
  if (IsError(files_)) goto bail;
  file_count = 0;
  while (has_next_) {
    File* p_file = &files_[file_count];
    if (Read(p_file) == -1) goto bail;
    ++file_count;
    if (Next(this) == -1) goto bail;
  }
  file_count_ = file_count;

  FolderSort(files_, ISN(0), ISN(file_count) - 1);

  return 0;

bail:
  Close();
  return -1;
}

inline void Folder::Close() {
  ArrayFill(path_, sizeof(path_), 0);
  has_next_ = 0;
  file_count_ = 0;
  if (files_) ObjectFactoryHeap(TPtr<IUW>(files_), RAMFactoryDelete);
  files_ = NILP;
#ifdef _MSC_VER
  if (_h != INVALID_HANDLE_VALUE) {
    FindClose(_h);
  }
  _h = INVALID_HANDLE_VALUE;
#else
  if (_d) {
    _og_closedir(_d);
  }
  _d = NILP;
  _e = NILP;
#endif
}

inline ISN Folder::Next(Folder* dir) {
  (void)dir;
  if (!has_next_) {
    errno = ENOENT;
    return -1;
  }

#ifdef _MSC_VER
  if (FindNextFile(_h, &_f) == 0)
#else
  _e = _og_readdir(_d);
  if (_e == NILP)
#endif
  {
    has_next_ = 0;
#ifdef _MSC_VER
    if (GetLastError() != ERROR_SUCCESS &&
      GetLastError() != ERROR_NO_MORE_FILES) {
      Close();
      errno = EIO;
      return -1;
    }
#endif
  }

  return 0;
}

ISN Folder::HasNext() { return has_next_; }

ISN Folder::Read(File* file) {
  if (IsError(file)) {
    errno = EINVAL;
    return AErrorNil;
  }
  const CHR* filename = NILP;
#ifdef _MSC_VER
  if (_h == INVALID_HANDLE_VALUE)
#else
  if (IsError(_e))
#endif
  {
    errno = ENOENT;
    return -1;
  }
#ifdef _MSC_VER
  filename = _f.cFileName;
#else
  filename = _e->d_name;
#endif
  ISW length = TSCodeCount<CHR, CHC, ISW>(path_) + 
    TSCodeCount<CHR, CHC, ISW>(filename) + 1 + FilenamePad;
  if (length >= URIPathLengthMax) {
    // the path for the file will be too long.
    errno = ENAMETOOLONG;
    return -1;
  }
  if (TSCodeCount<CHR, CHC, ISW>(filename) >= URLFilenameLengthMax) {
    errno = ENAMETOOLONG;
    return -1;
  }

  SPrint(file->Path(), URIPathLengthMax, path_);
  // Join the directory path and the entry name: the stat() below needs the
  // full entry path, not just the directory.
  ISW dir_len = TSCodeCount<CHR, CHC, ISW>(path_);
  CHR* cursor = file->Path() + dir_len;
  if (dir_len && cursor[-1] != '/')
    *cursor++ = '/';
  SPrint(cursor, URIPathLengthMax - ISW(cursor - file->Path()), filename);
  SPrint(file->Name(), URLFilenameLengthMax, filename);
#ifndef _MSC_VER
  struct stat file_stat;
#ifdef __MINGW32__
  if (_tstat(file->Path(), &file_stat) == -1) {
    return -1;
  }
#elif (defined _BSD_SOURCE) || (defined _DEFAULT_SOURCE) || \
    ((defined _XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) ||  \
    ((defined _POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
  if (lstat(file->Path(), &file_stat) == -1) {
    return -1;
  }
#else
  if (stat(file->Path(), &file_stat) == -1) {
    return -1;
  }
#endif
  file->is_directory_ = S_ISDIR(file_stat.st_mode) ? 1 : 0;
  file->is_reg_ = S_ISREG(file_stat.st_mode) ? 1 : 0;
#endif
  file->Extension();
  return 0;
}

inline ISN Folder::ReadFile(File* file, ISW i) {
  if (IsError(file)) {
    errno = EINVAL;
    return -1;
  }
  if (i >= file_count_) {
    errno = ENOENT;
    return -1;
  }

  ArrayCopy(file, sizeof(File), &files_[i], sizeof(File));
  file->Extension();

  return 0;
}

inline ISN Folder::OpenSubfolder(ISW i) {
  CHR path[URIPathLengthMax];
  if (IsError(this)) {
    errno = EINVAL;
    return -1;
  }
  if (i >= file_count_ || !files_[i].is_directory_) {
    errno = ENOENT;
    return -1;
  }

  SPrint(path, URIPathLengthMax, files_[i].path);
  Close();
  if (OpenSorted(path) == -1) return -1;

  return 0;
}

}  // namespace

inline ::_::File& operator<<(::_::File& o, CHA item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, const CHA* item) { return o.Write(item); }
#if USING_STB == YES_0
inline ::_::File& operator<<(::_::File& o, CHB item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, const CHB* item) { return o.Write(item); }
#endif
#if USING_STC == YES_0
inline ::_::File& operator<<(::_::File& o, CHC item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, const CHC* item) { return o.Write(item); }
#endif
inline ::_::File& operator<<(::_::File& o, ISA item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, IUA item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, ISB item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, IUB item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, ISC item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, IUC item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, ISD item) { return o.Write(item); }
inline ::_::File& operator<<(::_::File& o, IUD item) { return o.Write(item); }
#if USING_FPC == YES_0
inline ::_::File& operator<<(::_::File& o, FPC item) { return o.Write(item); }
#endif
#if USING_FPD == YES_0
inline ::_::File& operator<<(::_::File& o, FPD item) { return o.Write(item); }
#endif


inline ::_::TextFile& operator<<(::_::TextFile& o, CHA item) { 
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, const CHA* item) { 
  return o.Print(item);
}
#if USING_STB == YES_0
inline ::_::TextFile& operator<<(::_::TextFile& o, CHB item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, const CHB* item) {
  return o.Print(item);
}
#endif
#if USING_STC == YES_0
inline ::_::TextFile& operator<<(::_::TextFile& o, CHC item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, const CHC* item) {
  return o.Print(item);
}
#endif
inline ::_::TextFile& operator<<(::_::TextFile& o, ISA item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, IUA item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ISB item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, IUB item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ISC item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, IUC item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ISD item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, IUD item) {
  return o.Print(item);
}
#if USING_FPC == YES_0
inline ::_::TextFile& operator<<(::_::TextFile& o, FPC item) {
  return o.Print(item);
}
#endif
#if USING_FPD == YES_0
inline ::_::TextFile& operator<<(::_::TextFile& o, FPD item) {
  return o.Print(item);
}
#endif
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Hexf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Binaryf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Centerf item) {
  return o.Print(item); }
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Rightf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Linef item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Repeatf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Headingf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Indentf item) {
  return o.Print(item);
}
inline ::_::TextFile& operator<<(::_::TextFile& o, ::_::Charsf item) {
  return o.Print(item);
}
::_::TextFile& operator<<(::_::TextFile& o, ::_::TextFile item) {
  return o;
}


#endif
