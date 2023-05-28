#include "BitwiseAlgos.h"

std::vector<int> BitwiseAlgos::shiftAnd(std::wstring_view t,
                                        std::wstring_view p, int delta) {
#if USE_MORE_MACHINE_WORD
  return auxShiftAnd(t, p, delta);
#else

  if (p.length() <= WORD_LEN) {
    return auxShiftAnd(t, p, delta);
  }
  std::vector<int> answ;
  for (const auto &pos : auxShiftAnd(t, p.substr(0, WORD_LEN), delta))
    if (isDeltaGammaMatch(t.substr(pos, p.length()), p, delta))
      answ.push_back(pos);
  return answ;
#endif
};

/*
    -> shiftAnd computes all the Delta matches of a pattern 'p' in a text 't' by
   doing some bitwise operations using a precomputed table for every element in
   the alphabet (DTable) and a status match bitstring (DState). If there is a
   one in the m-1 position of DState we found a Delta match!
*/
std::vector<int> BitwiseAlgos::auxShiftAnd(std::wstring_view t,
                                           std::wstring_view p, int delta) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > MAX_BITS || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! -" + std::to_string(m));
  }
  // alph.print();
  // PREPROCESSING PHASE
  std::vector<int> answ;
  BitSet DTable[alph.size()]; // Table for every element in the alphabet.
  // Bitstring in which we are going to carry the match records.
  BitSet DState(0);

  for (int i = 0; i < alph.size(); ++i) {
    DTable[i] = BitSet(0);
    for (int j = 0; j < m; ++j) {
      // Put a one in the j^th position if there is a Delta match with p[i].
      DTable[i] |=
          BitSet(std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta)
          << j;
    }
  }

  // SEARCHING PHASE
  for (int i = 0; i < n; ++i) {
    // Compute the change led by the entering character.
    DState = ((DState << 1) | BitSet(1)) & DTable[alph.getIndex(t[i])];
    // One in the m-1 position means we found a Delta match.
    if ((~DState & (BitSet(1) << (m - 1))) == 0)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}