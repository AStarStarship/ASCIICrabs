// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_FILE_DECL
#define CRABS_FILE_DECL
#include <_Config.h>
#if SEAM >= CRABS_FILE
// OS I/O shim layer: the platform headers live with the declarations so
// File.hxx stays free of stdlib includes.
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <dirent.h>
#include <libgen.h>
#endif
#ifndef _MSC_VER
#if (defined __MINGW32__) && (defined _UNICODE)
// MINGW32 has two versions of dirent, ASCII and UNICODE.
#define _TINYDIR_DIR _WDIR
#define _og_dirent _wdirent
#define _og_opendir _wopendir
#define _og_readdir _wreaddir
#define _og_closedir _wclosedir
#else
#define _TINYDIR_DIR DIR
#define _og_dirent dirent
#define _og_opendir opendir
#define _og_readdir readdir
#define _og_closedir closedir
#endif
#endif
#include "Uniprinter.hpp"
namespace _ {

struct TextFile;

/* A file in a filesystem. */
struct File {

  /* Constructs a File from the given URI string. */
  File(const CHR* uri);

  /* Default constructor (for directory-scan scratch entries). */
  File() : is_directory_(0), is_reg_(0), path(), name(),
           uri_(NILP), is_open_(0), extension(NILP),
           last_time_modified_(0), fp_(NILP) {}

  /* Returns reference to this. */
  File& Star();

  TextFile Text();
  
  // Gets the file extension.
  const CHR* Extension();

  /* Gets the Uniform Resource Identifier. */
  const CHR* URI();
  
  /* Sets the Uniform Resource Identifier. */
  const CHR* URISet(const CHR* uri);

  /* Checks to see if the given file exists. */
  BOL Exists();

  /* Checks to see if the file is closed. */
  BOL IsOpen();

  /* Opens the file. */
  ISN Open(const CHR* path);

  /* Opens the stream for raw reading (binary mode). */
  ISN OpenRead(const CHR* path);

  /* Opens the stream for raw writing (binary mode). */
  ISN OpenWrite(const CHR* path);

  /* Reads raw bytes from the stream. Returns the bytes read. */
  ISW Read(void* buf, ISW bytes);

  /* Moves the stream position. Returns the new absolute position. */
  ISW Seek(ISW offset, ISN whence);

  /* Returns the stream size in bytes, leaving the position unchanged. */
  ISW Size();

  /* Closes the file. */
  void Close();

  // Returns true if this and the other File's name are the same.
  BOL CompareNames(const File* other);

  // Gets the last time this file was modified.
  TME LastTimeModified();
  
  /* Checks to see if the file has been modified by comparing the last time it was modified. */
  BOL HasBeenModified();

  /* Writes the given item to the file. */
  File& Write(CHA item);
  File& Write(const CHA* item);
#if USING_STB == YES_0
  File& Write(CHB item);
  File& Write(const CHB* item);
#endif
#if USING_STC == YES_0
  File& Write(CHC item);
  File& Write(const CHC* item);
#endif
  File& Write(ISA item);
  File& Write(IUA item);
  File& Write(ISB item);
  File& Write(IUB item);
  File& Write(ISC item);
  File& Write(IUC item);
  File& Write(ISD item);
  File& Write(IUD item);
  File& Write(FPC item);
  File& Write(FPD item);

  /* Accessors (data members). */
  CHR* Path() { return path; }
  const CHR* Path() const { return path; }
  CHR* Name() { return name; }
  const CHR* Name() const { return name; }
  CHR is_directory_,  //< flag for if this is a directory.
    is_reg_;          //< flag for if this is a regular file.
  CHR path[URIPathLengthMax];
  CHR name[URLFilenameLengthMax];

 private:
  const CHR* uri_;          //< Uniform Resource Identifier.
  CHR is_open_;             //< 1 after a successful Open().
  CHR* extension;
  TME last_time_modified_;  //< Last time the URL was modified.
  FILE* fp_;                //< Stream handle (OpenRead/OpenWrite).
};

/* A text file in a filesystem. */
struct TextFile {

  /* Constructs a TextFile from a File. */
  TextFile(File& file);

  /* Returns reference to this. */
  TextFile& Star();

  // Gets the file extension.
  const CHR* Extension();

  /* Gets the Uniform Resource Identifier. */
  const CHR* URI();

  /* Sets the Uniform Resource Identifier. */
  const CHR* URISet(const CHR* uri);

  /* Checks to see if the given file exists. */
  BOL Exists();

  /* Checks to see if the file is closed. */
  BOL IsOpen();

  /* Opens the file. */
  ISN Open();

  /* Closes the file. */
  void Close();

  /* Gets the last time this file was modified. */
  TME LastTimeModified();

  /* Checks to see if the file has been modified by comparing the last time it
  was modified. */
  BOL HasBeenModified();

  /* Prints the given item to the stdout. */
  TextFile& Print(CHA item);
  TextFile& Print(const CHA* item);
#if USING_STB == YES_0
  TextFile& Print(CHB item);
  TextFile& Print(const CHB* item);
#endif
#if USING_STC == YES_0
  TextFile& Print(CHC item);
  TextFile& Print(const CHC* item);
#endif
  TextFile& Print(ISC item);
  TextFile& Print(IUC item);
  TextFile& Print(ISD item);
  TextFile& Print(IUD item);
  TextFile& Print(FPC item);
  TextFile& Print(FPD item);
  TextFile& Print(Hexf& item);
  TextFile& Print(Binaryf& item);
  TextFile& Print(Centerf& item);
  TextFile& Print(Rightf& item);
  TextFile& Print(Linef& item);
  TextFile& Print(Repeatf& item);
  TextFile& Print(Headingf& item);
  TextFile& Print(Indentf& item);
  TextFile& Print(Charsf& item);
  //TextFile& Print(TypeWordValue item);

  /* Stream operator (for TPrint compatibility). */
  template<typename T>
  TextFile& operator<<(const T& item) {
    return Print(*const_cast<T*>(&item));
  }

  /* Prints a new line followed by the given item to the stdout. */
  TextFile& NL();

  /* Prints a new line followed by the given item to the stdout. */
  TextFile& NL(CHA item);
  TextFile& NL(const CHA* item);
#if USING_STB == YES_0
  TextFile& NL(CHB item);
  TextFile& NL(const CHB* item);
#endif
#if USING_STC == YES_0
  TextFile& NL(CHC item);
  TextFile& NL(const CHC* item);
#endif
  TextFile& NL(ISC item);
  TextFile& NL(IUC item);
  TextFile& NL(ISD item);
  TextFile& NL(IUD item);
  TextFile& NL(FPC item);
  TextFile& NL(FPD item);

  /* Stores the given item to the boofer and store the size. */
  TextFile& Hex(const void* item);
  TextFile& Hex(const void* item, ISW size);
  TextFile& Hex(const void* item, const void* item_stop);
  TextFile& Hex(ISA item);
  TextFile& Hex(IUA item);
  TextFile& Hex(ISB item);
  TextFile& Hex(IUB item);
  TextFile& Hex(ISC item);
  TextFile& Hex(IUC item);
  TextFile& Hex(ISD item);
  TextFile& Hex(IUD item);
  TextFile& Hex(FPC item);
  TextFile& Hex(FPD item);

  /* Stores the given item to the boofer and store the size. */
  TextFile& Binary(const void* item);
  TextFile& Binary(const void* item, ISW size);
  TextFile& Binary(const void* item, const void* item_stop);
  TextFile& Binary(ISA item);
  TextFile& Binary(IUA item);
  TextFile& Binary(ISB item);
  TextFile& Binary(IUB item);
  TextFile& Binary(ISC item);
  TextFile& Binary(IUC item);
  TextFile& Binary(ISD item);
  TextFile& Binary(IUD item);
  TextFile& Binary(FPC item);
  TextFile& Binary(FPD item);

private:
  File& file_;
};

/* A file in a filesystem. */
struct Folder {

  Folder(const CHR* uri);

  ISN Open(const CHR* path);
  ISN OpenSorted(const CHR* path);
  ISN Read(File* file);
  ISN ReadFile(File* file, ISW i);
  ISN Next(Folder* dir);
  ISN HasNext();
  ISN OpenSubfolder(ISW i);
  void Close();
 private:
   CHR path_[URIPathLengthMax];
   ISN has_next_;
   ISW file_count_;

   File* files_;
#ifdef _MSC_VER
   void* _h;
   ISC _f[148]; //< WIN32_FIND_DATA
#else
   _TINYDIR_DIR* _d;
   struct _og_dirent* _e;
#endif
};

}  //< namespace _

::_::File& operator<<(::_::File& o, CHA item);
::_::File& operator<<(::_::File& o, const CHA* item);

#if USING_STB == YES_0
::_::File& operator<<(::_::File& o, CHB item);
::_::File& operator<<(::_::File& o, const CHB* item);
#endif
#if USING_STC == YES_0
::_::File& operator<<(::_::File& o, CHC item);
::_::File& operator<<(::_::File& o, const CHC* item);
#endif
::_::File& operator<<(::_::File& o, IUA item);
::_::File& operator<<(::_::File& o, ISA item);
::_::File& operator<<(::_::File& o, IUB item);
::_::File& operator<<(::_::File& o, ISB item);
::_::File& operator<<(::_::File& o, IUC item);
::_::File& operator<<(::_::File& o, ISC item);
::_::File& operator<<(::_::File& o, IUC item);
::_::File& operator<<(::_::File& o, ISC item);
::_::File& operator<<(::_::File& o, IUD item);
::_::File& operator<<(::_::File& o, ISD item);
#if USING_FPC == YES_0
::_::File& operator<<(::_::File& o, FPC item);
#endif
#if USING_FPD == YES_0
::_::File& operator<<(::_::File& o, FPD item);
#endif

::_::TextFile& operator<<(::_::TextFile& o, CHA item);
::_::TextFile& operator<<(::_::TextFile& o, const CHA* item);

#if USING_STB == YES_0
::_::TextFile& operator<<(::_::TextFile& o, CHB item);
::_::TextFile& operator<<(::_::TextFile& o, const CHB* item);
#endif
#if USING_STC == YES_0
::_::TextFile& operator<<(::_::TextFile& o, CHC item);
::_::TextFile& operator<<(::_::TextFile& o, const CHC* item);
#endif
::_::TextFile& operator<<(::_::TextFile& o, IUA item);
::_::TextFile& operator<<(::_::TextFile& o, ISA item);
::_::TextFile& operator<<(::_::TextFile& o, IUB item);
::_::TextFile& operator<<(::_::TextFile& o, ISB item);
::_::TextFile& operator<<(::_::TextFile& o, IUC item);
::_::TextFile& operator<<(::_::TextFile& o, ISC item);
::_::TextFile& operator<<(::_::TextFile& o, IUC item);
::_::TextFile& operator<<(::_::TextFile& o, ISC item);
::_::TextFile& operator<<(::_::TextFile& o, IUD item);
::_::TextFile& operator<<(::_::TextFile& o, ISD item);
#if USING_FPC == YES_0
::_::TextFile& operator<<(::_::TextFile& o, FPC item);
#endif
#if USING_FPD == YES_0
::_::TextFile& operator<<(::_::TextFile& o, FPD item);
#endif
::_::TextFile& operator<<(::_::TextFile& o, ::_::Hexf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Binaryf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Centerf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Rightf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Linef item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Repeatf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Headingf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Indentf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::Charsf item);
::_::TextFile& operator<<(::_::TextFile& o, ::_::TextFile item);
#endif
#endif
