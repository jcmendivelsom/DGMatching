#include "BitwiseAlgos.h"

/*
    -> forwardScan computes all the Delta-Gamma matches of a pattern 'p' in a
   text 't' by doing some bitwise operations a precomputed table for
   every element in the alphabet (BTable) and a status match
   bitstring (DState). If there is a zero in the m*l-1 position of
   DState we found a Delta-Gamma match.
*/
std::vector<int> BitwiseAlgos::forwardScan(std::wstring_view t,
                                           std::wstring_view p, int delta,
                                           int gamma) {
  int m = p.length();
  int n = t.length();
  if (gamma < 0)
    gamma = m * delta;
  // int l = 1 + std::floor(std::log2(gamma + 1)) + 1;
  int l = 1 + std::ceil(std::log2(gamma + 1));
  if (m <= 0 || m * l > MAX_BITS || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! -" +
                                std::to_string(m * l) + " ? " +
                                std::to_string(MAX_BITS));
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  BitSet BTable[alph.size()]; // Tables for every element in the alphabet.
  // Bitstrings that will help us to calculate the Delta and Gama matches.
  BitSet DState(0), auxHighBits(0), H(0);

  for (int i = 0; i < alph.size(); ++i) {
    BTable[i] = BitSet(0);
    for (int j = m - 1; j >= 0; --j) {
      // Check if there is a Delta match. If so, save the difference, otherwise
      // save Gamma + 1
      if (std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta)
        BTable[i] =
            (BTable[i] << l) |
            BitSet((long)std::abs(alph.getValueByI(i) - alph.getValue(p[j])));
      else
        BTable[i] = (BTable[i] << l) | BitSet((long)(gamma + 1));
    }
  }
  // Initialize auxHigBits as (1*0^(l-1))^m
  for (int i = 0; i < m; i++)
    auxHighBits = (auxHighBits << l) | (BitSet(1) << (l - 1));

  // SEARCHING PHASE
  DState = auxHighBits;
  for (int j = 0; j < n; ++j) {
    // Shift l bits to the left and set those l bits as 2^(l-1)-(Gamma+1)
    DState = (DState << l) | BitSet((1L << (l - 1)) - (gamma + 1));
    H = DState & auxHighBits;
// Compute the change led by the entering character.
#if USE_MORE_MACHINE_WORD
    DState = sum((DState & ~H), BTable[alph.getIndex(t[j])]) | H;
    // DState = ((DState & ~H) + BTable[alph.getIndex(t[j])]) | H;
#else
    DState = ((DState & ~H) + BTable[alph.getIndex(t[j])]) | H;
#endif
    // If there is a zero in the m*l-1 position of DState we found a Delta-Gamma
    // match.
    if ((DState & (BitSet(1) << (m * l - 1))) == BitSet(0))
      answ.push_back(j - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}