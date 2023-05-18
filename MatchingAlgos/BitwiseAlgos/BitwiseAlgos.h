#pragma once
#include "MatchingAlgos.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>

/* /////////////////// */
#define USE_MORE_MACHINE_WORD 1 // Change this to 1 and MAX_BITS use your desired word length.
/* /////////////////// */

#define WORD_LEN 64

#if USE_MORE_MACHINE_WORD
#define MAX_BITS 64 // Change this to use your desired word length.
typedef std::bitset<MAX_BITS> BitSet;
#else
#define MAX_BITS WORD_LEN
typedef unsigned long long BitSet;
#endif

class BitwiseAlgos : public MatchingAlgos {

  BitSet sum(BitSet a, BitSet b);
  unsigned long long crop(BitSet x);

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> backwardScan(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);
  std::vector<int> forwardScan(std::wstring_view t, std::wstring_view p,
                               int delta, int gamma = -1);
  std::vector<int> shiftPlus(std::wstring_view t, std::wstring_view p,
                             int delta, int gamma = -1);
  std::vector<int> shiftAnd(std::wstring_view t, std::wstring_view p,
                            int delta);
  std::vector<int> shiftOr(std::wstring_view t, std::wstring_view p);
};
