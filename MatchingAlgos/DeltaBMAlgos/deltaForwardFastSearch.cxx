#include "DeltaBMAlgos.h"

std::vector<std::vector<int>>
DeltaBMAlgos::deltaForwardGoodSuffix(std::wstring p, int delta,
                                     std::vector<int> dBC) {
  if (dBC.size() != alph.size())
    dBC = deltaBadCharacter(p, delta);
  int m = p.length();
  std::vector<std::vector<int>> dFGS(m, std::vector<int>(alph.size(), m + 1));
  std::vector<int> dSuffix = deltaSuffix(p, 2 * delta);
  for (int j = m - 1; j >= 0; --j) {
    for (int c = 0; c < alph.size(); ++c)
      if (dBC[c] != m)
        for (int k = 1; k <= m; ++k) {
          // Check the condition p[m-k] matches the character of index c
          if (std::abs(alph.getValue(p[m - k]) - alph.getValueByI(c)) > delta)
            continue;
          // Check the condition if k<=j-1 then p[j-1]!=p[j-1-k]
          if (k <= j - 1 &&
              alph.getValue(p[j - 1]) == alph.getValue(p[j - 1 - k]))
            continue;
          // Check if the substring of 'p' terminated in m-k-1 of length m-j is
          // a 2*Delta suffix of 'p'
          if (m - j <= dSuffix[m - k - 1]) {
            dFGS[j][c] = k;
            break;
          }
        }
  }

  // Print Forward Good Suffix Table
  std::wcout << "-------------------" << '\n';
  std::wcout << " * ";
  for (int i = 0; i < alph.size(); ++i)
    std::wcout << " " << alph.getWChar(i) << " ";
  std::wcout << std::endl;
  for (int i = 0; i < m; ++i) {
    std::wcout << " " << p[i] << " ";
    for (int j = 0; j < alph.size(); ++j)
      std::wcout << " " << dFGS[i][j] << " ";
    std::wcout << std::endl;
  }
  std::wcout << "-------------------" << '\n';

  return dFGS;
}

std::vector<int> DeltaBMAlgos::deltaForwardFastSearch(std::wstring t,
                                                      std::wstring p, int delta,
                                                      int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Delta Bad Character - Table Shift for every element in the alphabet.
  std::vector<int> dBCShift = deltaBadCharacter(p, delta);
  // Delta Forward Good Suffix
  std::vector<std::vector<int>> dForwardGS =
      deltaForwardGoodSuffix(p, delta, dBCShift);

  // SEARCHING PHASE
  t += std::wstring(m + 1, p[m - 1]);
  int s = 0, sum, k, j;
  while (s <= n - m) {
    // Shift by Bad Character rule until the last char is matched.
    while (dBCShift[alph.getIndex(t[s + m - 1])] > 0) {
      s += dBCShift[alph.getIndex(t[s + m - 1])];
    }
    // Compare the characters until we found a mismatch.
    sum = std::abs(alph.getValue(p[m - 1]) - alph.getValue(t[s + m - 1]));
    j = m - 2;
    while (j >= 0 &&
           std::abs(alph.getValue(p[j]) - alph.getValue(t[s + j])) <= delta) {
      sum += std::abs(alph.getValue(p[j]) - alph.getValue(t[s + j]));
      j -= 1;
    }
    // If the length of the matched string is greater than the pattern we found
    // a match.
    if (j < 0 && (gamma < 0 || sum <= gamma))
      answ.push_back(s);
    s += dForwardGS[j + 1][alph.getIndex(t[s + m])];
  }
  if (answ.empty())
    return {-1};
  return answ;
}