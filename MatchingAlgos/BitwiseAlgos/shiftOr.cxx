#include "BitwiseAlgos.h"
#include <string>

std::vector<int> BitwiseAlgos::shiftOr(std::wstring_view t,
                                       std::wstring_view p) {
#if USE_MORE_MACHINE_WORD
  return auxShiftOr(t, p);
#else

  if (p.length() <= MAX_BITS) {
    return auxShiftOr(t, p);
  }
  std::vector<int> answ;
  for (const auto &pos : auxShiftOr(t, p.substr(0, WORD_LEN)))
    if (isDeltaGammaMatch(t.substr(pos, p.length()), p, 0))
      answ.push_back(pos);
  return answ;
#endif
};

/*
    -> shiftOr computes all the exact matches of a pattern 'x' in a text 'y' by
   doing some bitwise operations using a precomputed table for every element in
   the alphabet (ETable) and a status match bitstring (EState). If there is a
   zero in the m-1 position of EState we found a match!
*/
std::vector<int> BitwiseAlgos::auxShiftOr(std::wstring_view t,
                                          std::wstring_view p) {
  int m = p.length();
  if (m <= 0 || m > MAX_BITS || m > t.length()) {
    throw std::invalid_argument("Invalid parameters! -" + std::to_string(m));
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  BitSet ETable[alph.size()]; // Table for every element in the alphabet
  // Bitstring in which we are going to carry the match records
  BitSet EState = ~BitSet(0);

  for (int i = 0; i < alph.size(); ++i)
    ETable[i] = ~BitSet(0);
  for (int i = 0; i < m; ++i) {
    // Put a zero in the i^th position if there is a match.
    ETable[alph.getIndex(p[i])] &= ~(BitSet(1) << i);
  }

  // SEARCHING PHASE
  for (int i = 0; i < t.length(); ++i) {
    // Compute the change led by the entering character.
    EState = (EState << 1) | ETable[alph.getIndex(t[i])];
    // Zero in the m-1 position means we found a match.
    if ((EState & (BitSet(1) << (m - 1))) == 0)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}