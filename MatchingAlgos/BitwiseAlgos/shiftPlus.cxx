#include "BitwiseAlgos.h"

/*
    -> shiftPlus computes all the Delta-Gamma matches of a pattern 'p' in a text
   't' by doing some bitwise operations using two precomputed table for every
   element in the alphabet (DTable and GTable) and a status match bitstring
   (DState and GState). If there is a one in the m-1 position of DState we found
   a Delta match and if the number formed by the last d bits we have a Gamma
   match.
*/
std::vector<int> BitwiseAlgos::shiftPlus(std::wstring_view t,
                                         std::wstring_view p, int delta,
                                         int gamma) {
  int m = p.length();
  int n = t.length();
  if (gamma < 0)
    gamma = m * delta;
  int d = std::floor(std::log2(delta * m)) + 1;
  if (m <= 0 || m * d > 64 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  long DTable[alph.size()]; // Tables for every element in the alphabet.
  long GTable[alph.size()];
  // Bitstrings in which we are going to carry the Delta and Gama matches.
  long DState = 0;
  long GState = 0;

  bool deltaComp;
  for (int i = 0; i < alph.size(); ++i) {
    DTable[i] = 0;
    GTable[i] = 0;
    for (int j = 0; j < m; ++j) {
      // Check if there is a Delta match.
      deltaComp = std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta;
      // Move the necessary bits to the left.
      DTable[i] |= deltaComp << j;
      GTable[i] = GTable[i] << d;
      // Only if there is a Delta match we save our computation in GTable.
      if (deltaComp) {
        GTable[i] |= (long)std::abs(alph.getValueByI(i) - alph.getValue(p[j]));
      }
    }
  }

  // SEARCHING PHASE
  for (int i = 0; i < n; ++i) {
    // Compute the change led by the entering character.
    DState = ((DState << 1) | 1L) & DTable[alph.getIndex(t[i])];
    // Add in the first bits the char difference.
    GState = (GState >> d) + GTable[alph.getIndex(t[i])];
    // One in the m-1 position means we found a Delta match.
    // The las d bits are less or equal to Gamma means we have a Gamma match.
    if ((~DState & (1L << (m - 1))) == 0 &&
        (GState & ~(~1L << (d - 1))) <= (long)gamma)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}