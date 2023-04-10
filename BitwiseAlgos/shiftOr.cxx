#include "Alphabet.h"
#include "BitwiseAlgos.h"
#include <iostream>
#include <vector>

/*
    CONSTRUCTOR of the class BitwiseAlgos
*/
BitwiseAlgos::BitwiseAlgos(std::string alphPath) {
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
}

/*
    -> shiftOr computes all the exact matches of a pattern 'x' in a text 'y' by
   doing some bitwise operations using a precomputed table for every element in
   the alphabet (ETable) and a status match bitstring (EState). If there is a
   zero in the m-1 position of EState we found a match!
*/
std::vector<int> BitwiseAlgos::shiftOr(std::wstring t, std::wstring p) {
  int m = p.length();
  if (m <= 0 || m > 64 || m > t.length()) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  long ETable[alph.size()]; // Table for every element in the alphabet
  // Bitstring in which we are going to carry the match records
  long EState = ~0;

  for (int i = 0; i < alph.size(); ++i)
    ETable[i] = ~0;
  for (int i = 0; i < m; ++i) {
    if (alph.getIndex(p[i]) == -1) {
      std::wcout << "The following character is not in the alphabet: " << p[i]
                 << std::endl;
      throw std::invalid_argument("Invalid parameters! ");
    }
    // Put a zero in the i^th position if there is a match.
    ETable[alph.getIndex(p[i])] &= ~(1L << i);
  }

  // SEARCHING PHASE
  for (int i = 0; i < t.length(); ++i) {
    if (alph.getIndex(t[i]) == -1) {
      std::wcout << "The following character is not in the alphabet: " << t[i]
                 << std::endl;
      throw std::invalid_argument("Invalid parameters! ");
    }
    // Compute the change led by the entering character.
    EState = (EState << 1) | ETable[alph.getIndex(t[i])];
    // Zero in the m-1 position means we found a match.
    if ((EState & (1L << m - 1)) == 0)
      answ.push_back(i - m + 1);
  }
  if (answ.empty())
    return {-1};
  return answ;
}