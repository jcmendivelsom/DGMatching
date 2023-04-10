#include "Alphabet.h"
#include "BitwiseAlgos.h"
#include <cmath>
#include <iostream>
#include <vector>

/*
    -> backwardScan computes all the Delta-Gamma matches of a pattern 'p' in a
   text 't' by doing some bitwise operations a precomputed table for
   every element in the alphabet (BTable) and a status match
   bitstring (DState). If there is a zero in the m*l-1 position of
   DState we found a possible Delta-Gamma match.
*/
std::vector<int> BitwiseAlgos::backwardScan(std::wstring t, std::wstring p,
                                            int delta, int gamma) {
  int m = p.length();
  int n = t.length();
  int l = 1 + std::ceil(std::log2(gamma + 1));
  if (m <= 0 || m * l > 64 || m > n || delta < 0 || gamma < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  long BTable[alph.size()]; // Tables for every element in the alphabet.
  // Bitstrings that will help us to calculate the Delta and Gama matches.
  long DState = 0, auxHighBits = 0, H = 0;

  for (int i = 0; i <= alph.size(); ++i) {
    BTable[i] = 0;
    for (int j = 0; j < m; ++j) {
      if (alph.getIndex(p[j]) == -1) {
        std::wcout << "The following character is not in the alphabet: " << p[j]
                   << std::endl;
        throw std::invalid_argument("Invalid parameters! ");
      }
      // Check if there is a Delta match. If so, save the difference, otherwise
      // save Gamma + 1
      if (std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta)
        BTable[i] = (BTable[i] << l) |
                    (long)std::abs(alph.getValueByI(i) - alph.getValue(p[j]));
      else
        BTable[i] = (BTable[i] << l) | (long)(gamma + 1);
    }
  }
  // Initialize auxHigBits as (1*0^(l-1))^m
  for (int i = 0; i < m; i++)
    auxHighBits = (auxHighBits << l) | (1L << l - 1);

  // SEARCHING PHASE
  int pos = 0, j, last;
  while (pos <= n - m) {
    j = m;
    last = m;
    // Set DState as ([2^(l-1)-(Gamma+1)]_l)^m
    for (int i = 0; i < m; i++)
      DState = (DState << l) | ((1L << l - 1) - (gamma + 1));
    while ((DState & auxHighBits) != auxHighBits) {
      if (alph.getIndex(t[pos + j - 1]) == -1) {
        std::wcout << "The following character is not in the alphabet: "
                   << t[pos + j - 1] << std::endl;
        throw std::invalid_argument("Invalid parameters! ");
      }
      // Compute the change led by the entering character.
      H = DState & auxHighBits;
      DState = ((DState & ~H) + BTable[alph.getIndex(t[pos + j - 1])]) | H;
      j -= 1;
      // If there is a zero in the m*l-1 position of DState means that p[0 ..
      // m-j+1] matches t[pos+j ... pos+m]
      if ((DState & (1L << m * l - 1)) == 0) {
        // If there is a match j is negative. Otherwise we keep the last index
        // that matches
        if (j > 0)
          last = j;
        else
          answ.push_back(pos);
      }
      DState = (DState << l) | (1L << l - 1);
    }
    pos += last;
  }
  if (answ.empty())
    return {-1};
  return answ;
}

/*
int main() {
  std::string y, x;
  unsigned int delta, gamma;
  std::cin >> y >> x;
  std::cin >> delta >> gamma;
  std::cout << backwardScan(y, x, delta, gamma) << std::endl;
  return 0;
}
*/