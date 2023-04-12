#include "DeltaBMAlgos.h"

/*
    -> sumDeltaMatch return the sum of the value differences char. by char. But
   if find a Delta mismatch returns -1.
*/
int DeltaBMAlgos::sumDeltaMatch(std::wstring x, std::wstring y, int delta) {
  int sum = 0;
  if (x.length() != y.length() || delta < 0)
    return -1;
  for (int i = 0; i < x.length(); ++i) {
    if (std::abs(alph.getValue(x[i]) - alph.getValue(y[i])) > delta)
      return -1;
    sum += std::abs(alph.getValue(x[i]) - alph.getValue(y[i]));
  }
  return sum;
}

/*
     -> isDeltaGammaMatch return whether 'x' Delta match 'y' or not. If Gamma is
   negative just check the Delta match.
*/
bool DeltaBMAlgos::isDeltaGammaMatch(std::wstring x, std::wstring y, int delta,
                                     int gamma = -1) {
  int aux = sumDeltaMatch(x, y, delta);
  return 0 <= aux && (aux <= gamma || gamma < 0);
}

/*
     -> minDeltaMatch compute the first index of 'p' from back to front that
   Delta matches 'a'.
*/
int DeltaBMAlgos::backMinDeltaMatch(std::wstring a, std::wstring p, int delta,
                                    bool justSuffix) {
  int m = p.length();
  int k = a.length();
  if (k > m)
    return m;
  if (justSuffix)
    return isDeltaGammaMatch(a, p.substr(m - k), delta);
  for (int i = m - k; i >= 0; --i) {
    if (isDeltaGammaMatch(a, p.substr(i, k), delta))
      return m - (i + 1);
  }
  return m;
}

/*
     -> deltaBadCharacter for every element in the alphabet compute the first
   index of 'p' from back to front that Delta matches.
*/
std::vector<int> DeltaBMAlgos::deltaBadCharacter(std::wstring p, int delta) {
  // By default all char init with p.length()
  std::vector<int> dBC(alph.size(), p.length());
  std::vector<bool> visitedC(alph.size(), false);
  for (int i = p.length() - 1; i >= 0; --i) {
    for (int d = 0; d <= delta; ++d) {
      // Get all the indices that have the value p[i] + d (0 <= d <= Delta)
      for (const auto &inPlusDelta :
           alph.getIndicesByVal(alph.getValue(p[i]) + d)) {
        // For each index, check if not calculated. If so, compute
        // backMinDeltaMatch
        if (!visitedC[inPlusDelta]) {
          dBC[inPlusDelta] = backMinDeltaMatch(
              std::wstring(1, alph.getWChar(inPlusDelta)), p, delta);
          visitedC[inPlusDelta] = true;
        }
      }
      // Get all the indices that have the value p[i] - d (0 <= d <= Delta)
      for (const auto &inMinusDelta :
           alph.getIndicesByVal(alph.getValue(p[i]) - d)) {
        // For each index, check if not calculated. If so, compute
        // backMinDeltaMatch
        if (!visitedC[inMinusDelta]) {
          dBC[inMinusDelta] = backMinDeltaMatch(
              std::wstring(1, alph.getWChar(inMinusDelta)), p, delta);
          visitedC[inMinusDelta] = true;
        }
      }
    }
  }

  // Print the Delta Bad Character Table
  for (int i = 0; i < dBC.size(); ++i)
    std::wcout << " || " << alph.getWChar(i) << " " << dBC[i];
  std::wcout << std::endl;
  
  return dBC;
}

/*
    -> deltaTunedBM computes all the Delta-Gamma matches of a pattern 'x' in a
   text 'y' by doing an adapted version of the Tuned Boyer Moore Algorithm.
*/
std::vector<int> DeltaBMAlgos::deltaTunedBM(std::wstring t, std::wstring p,
                                            int delta, int gamma) {
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
  int s = backMinDeltaMatch(p.substr(m - 1, 1), p.substr(0, m - 1), 2 * delta);
  s += 1;
  // Add to the end of the text p[m-1]^m.
  t += std::wstring(m, p[m - 1]);

  // SEARCHING PHASE
  int k, j = m;
  while (j <= n) {
    k = dBCShift[alph.getIndex(t[j - 1])];
    while (k != 0) {
      j += k;
      k = dBCShift[alph.getIndex(t[j - 1])];
    }
    if (isDeltaGammaMatch(p, t.substr(j - m, m), delta, gamma) && j <= n)
      answ.push_back(j - m);
    j += s;
  }
  if (answ.empty())
    return {-1};
  return answ;
}