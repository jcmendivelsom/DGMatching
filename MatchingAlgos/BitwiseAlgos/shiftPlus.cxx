#include "BitwiseAlgos.h"

unsigned long long BitwiseAlgos::crop(BitSet x) {
#if USE_MORE_MACHINE_WORD
  return (x & ~(~BitSet(0) << WORD_LEN)).to_ullong();
#else
  return x;
#endif
};

BitSet BitwiseAlgos::sum(BitSet a, BitSet b) {
  unsigned long long aSub=0, bSub=0;
  // bool carry = false;
  BitSet answ(0);
  for (short int i = 0; WORD_LEN * i < MAX_BITS; ++i) {
    aSub = aSub < bSub;
    aSub += crop(a >> (WORD_LEN * i));
    bSub = crop(b >> (WORD_LEN * i));
    // std::cout<< aSub << " " << bSub << std::endl;
    aSub += bSub;
    // aSub += carry;
    answ |= BitSet(aSub) << (WORD_LEN * i);
  }
  return answ;
}

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
  if (m <= 0 || m * d > MAX_BITS || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! -" +
                                std::to_string(m * d) + " ? " +
                                std::to_string(MAX_BITS));
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  BitSet DTable[alph.size()]; // Tables for every element in the alphabet.
  BitSet GTable[alph.size()];
  // Bitstrings in which we are going to carry the Delta and Gama matches.
  BitSet DState(0);
  BitSet GState(0);

  bool deltaComp;
  for (int i = 0; i < alph.size(); ++i) {
    DTable[i] = BitSet(0);
    GTable[i] = BitSet(0);
    for (int j = 0; j < m; ++j) {
      // Check if there is a Delta match.
      deltaComp = std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta;
      // Move the necessary bits to the left.
      DTable[i] |= BitSet(deltaComp) << j;
      GTable[i] = GTable[i] << d;
      // Only if there is a Delta match we save our computation in GTable.
      if (deltaComp) {
        GTable[i] |=
            BitSet((long)std::abs(alph.getValueByI(i) - alph.getValue(p[j])));
      }
    }
  }

  // SEARCHING PHASE
  for (int i = 0; i < n; ++i) {
    // Compute the change led by the entering character.
    DState = ((DState << 1) | BitSet(1)) & DTable[alph.getIndex(t[i])];
// Add in the first bits the char difference.
#if USE_MORE_MACHINE_WORD
    GState = sum((GState >> d), GTable[alph.getIndex(t[i])]);
#else
    GState = (GState >> d) + GTable[alph.getIndex(t[i])];
#endif
    // One in the m-1 position means we found a Delta match.
    // The las d bits are less or equal to Gamma means we have a Gamma match.
    if ((~DState & (BitSet(1) << (m - 1))) == 0 &&
        crop(GState & ~(~BitSet(1) << (d - 1))) <= (long)gamma)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}