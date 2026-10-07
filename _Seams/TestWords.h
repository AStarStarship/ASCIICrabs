// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABS_TESTWORDS
#define CRABS_TESTWORDS

/* A basic list of test words. */
template<typename CHS = CHR, typename CHT = CHD>
struct TTestWords {
  enum {
    CharsMax = 16,   //< The max characters in a word including term char.
    Total = 128,  //< The max word count.
  };
  /* An array of 128 random words. */
  static const CHS* Words() {
    static const CHS Strings[Total + 1][CharsMax] = {
      {'w', 'h', 'i', 'p'},
      {'j', 'u', 'm', 'b', 'l', 'e', 'd'},
      {'e', 'n', 'e', 'r', 'g', 'e', 't', 'i', 'c'},
      {'s', 'u', 'p', 'e', 'r', 'f', 'i', 'c', 'i', 'a', 'l'},
      {'f', 'a', 'r', 'm'},
      {'w', 'h', 'i', 'n', 'e'},
      {'h', 'u', 'r', 'r', 'i', 'e', 'd'},
      {'c', 'r', 'a', 's', 'h'},
      {'h', 'y', 'd', 'r', 'a', 'n', 't'},
      {'s', 'i', 'n'},
      {'g', 'r', 'o', 'u', 'n', 'd'},
      {'m', 'a', 'l', 'e'},
      {'t', 'r', 'a', 'y'},
      {'d', 'i', 's', 'i', 'l', 'l', 'u', 's', 'i', 'o', 'n', 'e', 'd'},
      {'a', 'p', 'p', 'r', 'o', 'v', 'a', 'l'},
      {'b', 'r', 'a', 's', 'h'},
      {'l', 'e', 't', 't', 'e', 'r', 's'},
      {'r', 'u', 'b'},
      {'f', 'l', 'o', 'a', 't'},
      {'g', 'r', 'o', 'o', 'v', 'y'},
      {'t', 'h', 'e', 'o', 'r', 'y'},
      {'s', 'c', 'h', 'o', 'o', 'l'},
      {'u', 'g', 'l', 'i', 'e', 's', 't'},
      {'p', 'r', 'e', 'f', 'e', 'r'},
      {'p', 'a', 'p', 'e', 'r'},
      {'s', 'i', 's', 't', 'e', 'r'},
      {'c', 'o', 'm', 'p', 'l', 'e', 't', 'e'},
      {'d', 'r', 'a', 'c', 'o', 'n', 'i', 'a', 'n'},
      {'p', 'r', 'a', 'y'},
      {'m', 'e', 't', 'a', 'l'},
      {'u', 'n', 't', 'i', 'd', 'y'},
      {'b', 'o', 'o', 'k'},
      {'a', 'w', 'e', 's', 'o', 'm', 'e'},
      {'a', 't', 't', 'r', 'a', 'c', 't'},
      {'f', 'e', 'i', 'g', 'n', 'e', 'd'},
      {'w', 'r', 'a', 'p'},
      {'a', 'r', 'r', 'i', 'v', 'e'},
      {'r', 'a', 'i', 'n', 's', 't', 'o', 'r', 'm'},
      {'e', 'x', 'c', 'e', 'l', 'l', 'e', 'n', 't'},
      {'o', 'f', 'f', 'e', 'n', 'd'},
      {'o', 'f', 'f', 'e', 'r'},
      {'c', 'o', 'u', 'g', 'h'},
      {'c', 'h', 'i', 'n'},
      {'s', 't', 'o', 'n', 'e'},
      {'s', 'u', 'b', 's', 't', 'a', 'n', 'c', 'e'},
      {'m', 'a', 'r', 'k'},
      {'d', 'o', 'u', 'b', 't'},
      {'t', 'e', 'n', 't'},
      {'p', 'a', 'i', 'n', 'f', 'u', 'l'},
      {'b', 'l', 'e', 'a', 'c', 'h'},
      {'a', 'w', 'a', 'k', 'e'},
      {'d', 'i', 's', 'g', 'u', 's', 't', 'e', 'd'},
      {'a', 'd', 'j', 'o', 'i', 'n', 'i', 'n', 'g'},
      {'b', 'o', 'l', 't'},
      {'a', 'c', 'c', 'e', 'p', 't'},
      {'p', 'r', 'o', 'u', 'd'},
      {'a', 'b', 's', 'e', 'n', 't'},
      {'a', 'd', 'o', 'r', 'a', 'b', 'l', 'e'},
      {'n', 'o', 'x', 'i', 'o', 'u', 's'},
      {'s', 't', 'a', 'm', 'p'},
      {'d', 'a', 'm', 'a', 'g', 'e', 'd'},
      {'s', 'h', 'o', 'c', 'k'},
      {'g', 'r', 'a', 'c', 'e', 'f', 'u', 'l'},
      {'s', 'a', 'b', 'l', 'e'},
      {'f', 'i', 'r', 'e', 'm', 'a', 'n'},
      {'o', 'i', 'l'},
      {'a', 'c', 't', 'u', 'a', 'l', 'l', 'y'},
      {'f', 'o', 'o', 't'},
      {'b', 'a', 'k', 'e'},
      {'p', 'u', 'l', 'l'},
      {'g', 'u', 's', 't', 'y'},
      {'p', 'a', 'i', 'n', 't'},
      {'n', 'i', 'p', 'p', 'y'},
      {'h', 'o', 'r', 's', 'e'},
      {'s', 'e', 'c', 'o', 'n', 'd', 'h', 'a', 'n', 'd'},
      {'s', 'l', 'i', 'p'},
      {'p', 'o', 's', 's', 'i', 'b', 'l', 'e'},
      {'a', 't', 't', 'r', 'a', 'c', 't', 'i', 'o', 'n'},
      {'v', 'o', 'l', 'l', 'e', 'y', 'b', 'a', 'l', 'l'},
      {'f', 'a', 'd', 'e'},
      {'o', 'p', 'p', 'o', 's', 'i', 't', 'e'},
      {'r', 'e', 's', 't'},
      {'s', 'l', 'o', 'w'},
      {'t', 'h', 'i', 'r', 'd'},
      {'i', 'n', 'c', 'o', 'm', 'p', 'e', 't', 'e', 'n', 't'},
      {'a', 'p', 'p', 'r', 'e', 'c', 'i', 'a', 't', 'e'},
      {'g', 'r', 'o', 'a', 'n'},
      {'s', 'm', 'e', 'l', 'l'},
      {'m', 'a', 'i', 'd'},
      {'d', 'e', 'c', 'i', 's', 'i', 'v', 'e'},
      {'c', 'r', 'a', 'w', 'l'},
      {'p', 'o', 'i', 'n', 't'},
      {'s', 'w', 'e', 'e', 't'},
      {'c', 'u', 'p'},
      {'r', 'e', 'g', 'r', 'e', 't'},
      {'s', 't', 'r', 'o', 'n', 'g'},
      {'c', 'o', 'm', 'p', 'l', 'a', 'i', 'n'},
      {'g', 'r', 'e', 'a', 't'},
      {'a', 'g', 'g', 'r', 'e', 's', 's', 'i', 'v', 'e'},
      {'h', 'i', 's', 's', 'i', 'n', 'g'},
      {'r', 'a', 'i', 'n'},
      {'s', 'n', 'e', 'e', 'z', 'e'},
      {'c', 'u', 'r', 'l', 'y'},
      {'f', 'u', 'r', 'n', 'i', 't', 'u', 'r', 'e'},
      {'c', 'o', 'n', 'c', 'e', 'r', 'n'},
      {'b', 'i', 'g'},
      {'b', 'e', 'h', 'a', 'v', 'i', 'o', 'r'},
      {'p', 'r', 'o', 'g', 'r', 'a', 'm'},
      {'r', 'e', 'd'},
      {'a', 'b', 'a', 's', 'h', 'e', 'd'},
      {'t', 'h', 'a', 'n', 'k'},
      {'m', 'i', 'l', 'k'},
      {'s', 't', 'r', 'a', 'n', 'g', 'e', 'r'},
      {'h', 'o', 'o', 'k'},
      {'d', 'o', 'm', 'i', 'n', 'e', 'e', 'r', 'i', 'n', 'g'},
      {'b', 'a', 's', 'i', 'n'},
      {'d', 'e', 'c', 'e', 'i', 'v', 'e'},
      {'c', 'o', 'm', 'p', 'a', 'r', 'e'},
      {'s', 'l', 'e', 'e', 'p'},
      {'i', 'n', 'c', 'r', 'e', 'a', 's', 'e'},
      {'i', 'r', 'a', 't', 'e'},
      {'f', 'a', 'i', 'n', 't'},
      {'s', 'o', 'u', 'n', 'd'},
      {'f', 'l', 'o', 'w'},
      {'g', 'u', 'e', 's', 's'},
      {'d', 'r', 'a', 'i', 'n'},
      {'b', 'r', 'i', 'd', 'g', 'e'},
      {'d', 'o', 'n', 'e'},
      { 'i', 'n', 'v', 'a', 'l', 'i', 'd'}
    };
    return &Strings[0][0];
  }

  /* Gets a test word. */
  static const CHS* Word(ISN index) {
    if (index < 0)
      index = 0;
    else if (index >= Total)
      index = Total;
    return Words()[index];
  }
};

#endif
