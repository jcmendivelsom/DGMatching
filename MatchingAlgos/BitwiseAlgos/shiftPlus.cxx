#include "BitwiseAlgos.h"
#include <iostream>

unsigned long long BitwiseAlgos::crop(BitSet x) {
#if USE_MORE_MACHINE_WORD
  return (x & ~(~BitSet(0) << WORD_LEN)).to_ullong();
  // return x.get();
#else
  return x;
#endif
};

BitwiseAlgos::BitSet BitwiseAlgos::sum(BitSet a, BitSet b) {
  unsigned long long aSub = 0, bSub = 0;
  // bool carry = false;
  BitSet answ(0);
#if USE_MORE_MACHINE_WORD
  size_t i = 0;
  while (WORD_LEN * i < MAX_BITS) {
    aSub = aSub < bSub;
    // aSub += crop(a >> (WORD_LEN * i));
    aSub += crop(a);
    // bSub = crop(b >> (WORD_LEN * i));
    bSub = crop(b);
    // std::cout<< aSub << " " << bSub << std::endl;
    aSub += bSub;
    // aSub += carry;
    answ |= BitSet(aSub) << (WORD_LEN * i);
    a >>= WORD_LEN;
    b >>= WORD_LEN;
    i++;
  }
#endif
  return answ;
}

std::vector<int> BitwiseAlgos::shiftPlus(std::wstring_view t,
                                         std::wstring_view p, int delta,
                                         int gamma) {
#if USE_MORE_MACHINE_WORD
  return auxBackwardScan(t, p, delta, gamma);
#else
  int m = p.length();
  int n = t.length();
  if (gamma < 0)
    gamma = m * delta;
  int d = std::floor(std::log2(delta * m)) + 1;
  // int l = 1 + std::floor(std::log2(gamma + 1)) + 1;
  if (m * d <= WORD_LEN) {
    return auxShiftPlus(t, p, delta, gamma);
  }
  std::vector<int> answ;
  int l;
  for (l = m; l > 1 && l * (std::floor(std::log2(delta * l)) + 1) > WORD_LEN; --l);
  if (l <= 1)
    return answ;
  // std::wcout << "* " << std::floor(WORD_LEN / d) << std::endl;
  for (const auto &pos : auxShiftPlus(t, p.substr(0, l), delta, gamma)) {
    // std::wcout << "*" << pos;
    if (isDeltaGammaMatch(t.substr(pos, p.length()), p, delta, gamma))
      answ.push_back(pos);
  }
  // std::wcout << "*" << std::endl;
  return answ;
#endif
};

/*
    -> shiftPlus computes all the Delta-Gamma matches of a pattern 'p' in a text
   't' by doing some bitwise operations using two precomputed table for every
   element in the alphabet (DTable and GTable) and a status match bitstring
   (DState and GState). If there is a one in the m-1 position of DState we found
   a Delta match and if the number formed by the last d bits we have a Gamma
   match.
*/
std::vector<int> BitwiseAlgos::auxShiftPlus(std::wstring_view t,
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
    // GState = (GState >> d) ^ GTable[alph.getIndex(t[i])];
#else
    GState = (GState >> d) + GTable[alph.getIndex(t[i])];
#endif
    // One in the m-1 position means we found a Delta match.
    // The las d bits are less or equal to Gamma means we have a Gamma match.
    if ((~DState & (BitSet(1) << (m - 1))) == BitSet(0) &&
        crop(GState & ~(~BitSet(1) << (d - 1))) <= (long)gamma)
      answ.push_back(i - m + 1);
  }
  // if (answ.empty())
  // return {-1};
  return answ;
}