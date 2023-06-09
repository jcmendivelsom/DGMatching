#include "DeltaBMAlgos.h"
#include <iostream>

/*
     -> minDeltaMatch compute the first index of 'p' from back to front that
   Delta matches 'a'.
*/
int DeltaBMAlgos::backMinDeltaMatch(wchar_t a, std::wstring_view p, int delta) {
  int m = p.length();
  for (int i = m - 1; i >= 0; --i) {
    if (std::abs(alph.getValue(a) - alph.getValue(p[i])) <= delta)
      return m - 1 - i;
  }
  return m;
}

/*
     -> deltaBadCharacter for every element in the alphabet compute the first
   index of 'p' from back to front that Delta matches.
*/
std::vector<int> DeltaBMAlgos::deltaBadCharacter(std::wstring_view p,
                                                 int delta) {
  // By default all char init with p.length()
  std::vector<int> dBC(alph.size(), p.length());
  /*
  std::vector<bool> visitedC(alph.size(), false);
  for (int i = p.length() - 1; i >= 0; --i) {
    for (int d = 0; d <= delta; ++d) {
      // Get all the indices that have the value p[i] + d (0 <= d <= Delta)
      for (const auto &inPlusDelta :
           alph.getIndicesByVal(alph.getValue(p[i]) + d)) {
        // For each index, check if not calculated. If so, compute
        // backMinDeltaMatch
        if (!visitedC[inPlusDelta]) {
          dBC[inPlusDelta] =
              backMinDeltaMatch(alph.getWChar(inPlusDelta), p, delta);
          visitedC[inPlusDelta] = true;
        }
      }
      // Get all the indices that have the value p[i] - d (0 <= d <= Delta)
      for (const auto &inMinusDelta :
           alph.getIndicesByVal(alph.getValue(p[i]) - d)) {
        // For each index, check if not calculated. If so, compute
        // backMinDeltaMatch
        if (!visitedC[inMinusDelta]) {
          dBC[inMinusDelta] =
              backMinDeltaMatch(alph.getWChar(inMinusDelta), p, delta);
          visitedC[inMinusDelta] = true;
        }
      }
    }

  }
  */
  for (int i = 0; i < dBC.size(); ++i) {
    dBC[i] = backMinDeltaMatch(alph.getWChar(i), p, delta);
  }
  // Print the Delta Bad Character Table
  /*
  for (int i = 0; i < dBC.size(); ++i)
    std::wcout << i << " || " << alph.getWChar(i) << " " << dBC[i] << std::endl;
  std::wcout << std::endl;
  */
  return dBC;
}

/*
    -> deltaTunedBM computes all the Delta-Gamma matches of a pattern 'x' in a
   text 'y' by doing an adapted version of the Tuned Boyer Moore Algorithm.
*/
std::vector<int> DeltaBMAlgos::deltaTunedBM(std::wstring_view t,
                                            std::wstring_view p, int delta,
                                            int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Table Shift for every element in the alphabet.
  std::vector<int> dBCShift = deltaBadCharacter(p, delta);

  // First shift
  int s = backMinDeltaMatch(p[m - 1], p.substr(0, m - 1), 2 * delta);
  s += 1;
  // Add to the end of the text p[m-1]^m.
  // t += std::wstring(m, p[m - 1]);
  // SEARCHING PHASE
  int k, j = m - 1;
  try {
    while (j < n) {
      k = dBCShift[alph.getIndex(t.at(j))];
      while (j < n && k != 0) {
        j += k;
        k = dBCShift[alph.getIndex(t.at(j))];
      }
      // std::wcout << j - (m - 1) << " - " << j << " # "  << std::endl;
      if (j < n && isDeltaGammaMatch(p, t.substr(j - (m - 1), m), delta, gamma)) {
        answ.push_back(j - (m - 1));
      }
      j += s;
    }
  } catch (const std::out_of_range &e) {
    std::wcout<<"";
  }
  if (answ.empty())
    return {-1};
  return answ;
}