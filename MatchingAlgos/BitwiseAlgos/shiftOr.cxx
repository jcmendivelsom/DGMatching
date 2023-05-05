#include "BitwiseAlgos.h"

/*
    -> shiftOr computes all the exact matches of a pattern 'x' in a text 'y' by
   doing some bitwise operations using a precomputed table for every element in
   the alphabet (ETable) and a status match bitstring (EState). If there is a
   zero in the m-1 position of EState we found a match!
*/
std::vector<int> BitwiseAlgos::shiftOr(std::wstring_view t, std::wstring_view p) {
  int m = p.length();
  if (m <= 0 || m > 64 || m > t.length()) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  long ETable[alph.size()]; // Table for every element in the alphabet
  // Bitstring in which we are going to carry the match records
  unsigned long EState = ~0;

  for (int i = 0; i < alph.size(); ++i)
    ETable[i] = ~0;
  for (int i = 0; i < m; ++i) {
    // Put a zero in the i^th position if there is a match.
    ETable[alph.getIndex(p[i])] &= ~(1L << i);
  }

  // SEARCHING PHASE
  for (int i = 0; i < t.length(); ++i) {
    // Compute the change led by the entering character.
    EState = (EState << 1) | ETable[alph.getIndex(t[i])];
    // Zero in the m-1 position means we found a match.
    if ((EState & (1L << (m - 1))) == 0)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}