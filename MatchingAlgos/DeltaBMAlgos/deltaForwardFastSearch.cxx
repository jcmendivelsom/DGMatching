#include "DeltaBMAlgos.h"

/*
    deltaForwardGoodSuffix returns a int table containing in position (j, c) the
   minimum k that: p[j-k ... m-k-1] is a 2 * delta suffix of p[j ... m-1] and if
   k<=j-1 then p[j-1]!=p[j-1-k] and p[m-k] delta matches c, where m is the
   length of 'p'.
*/
std::vector<std::vector<int>>
DeltaBMAlgos::deltaForwardGoodSuffix(std::wstring_view p, int delta,
                                     std::vector<int> dBC) {
  if (dBC.size() != alph.size())
    dBC = deltaBadCharacter(p, delta);
  int m = p.length();
  std::vector<std::vector<int>> dFGS(m, std::vector<int>(alph.size(), m + 1));
  std::vector<int> dSuffix = deltaSuffix(p, 2 * delta);
  for (int j = m - 1; j >= 0; --j) {
    for (int c = 0; c < alph.size(); ++c)
      for (int k = dBC[c] + 1; k <= m; ++k) {
        // Check the condition if k<=j-1 then p[j-1]!=p[j-1-k]
        if (k <= j - 1 &&
            alph.getValue(p[j - 1]) == alph.getValue(p[j - 1 - k]))
          continue;
        // Check the condition p[m-k] matches the character of index c
        if (std::abs(alph.getValue(p[m - k]) - alph.getValueByI(c)) > delta)
          continue;
        // Check if the substring of 'p' terminated in m-k-1 of length m-j is
        // a 2*Delta suffix of 'p'
        if (std::min(m - j, m - k) <= dSuffix[m - k - 1]) {
          dFGS[j][c] = k;
          break;
        }
      }
  }
  /*
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
  */
  return dFGS;
}

/*
    -> deltaForwardFastSearch returns the indices of all Delta-Gamma matches of
   'p' in 't' by shifting using delta Bad Character Rule, then checking naively
   and moving by Forward Good Suffix Rule.
*/
std::vector<int> DeltaBMAlgos::deltaForwardFastSearch(std::wstring_view t,
                                                      std::wstring_view p,
                                                      int delta, int gamma) {
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
  // t += std::string(m + 1, p[m - 1]);
  int s = 0, sum, k, j;
  try {
    while (s <= n - m) {
      // Shift by Bad Character rule until the last char is matched.
      while (dBCShift[alph.getIndex(t.at(s + m - 1))] > 0) {
        s += dBCShift[alph.getIndex(t.at(s + m - 1))];
      }
      // Compare the characters until we found a mismatch.
      sum = std::abs(alph.getValue(p[m - 1]) - alph.getValue(t.at(s + m - 1)));
      j = m - 2;
      while (j >= 0 && std::abs(alph.getValue(p[j]) -
                                alph.getValue(t.at(s + j))) <= delta) {
        sum += std::abs(alph.getValue(p[j]) - alph.getValue(t.at(s + j)));
        j -= 1;
      }
      // If the length of the matched string is greater than the pattern we
      // found a match.
      if (j < 0 && (gamma < 0 || sum <= gamma))
        answ.push_back(s);
      s += dForwardGS[j + 1][alph.getIndex(t.at(s + m))];
    }
  } catch (const std::out_of_range &e) {
    std::wcout << "";
  }
  if (answ.empty())
    return {-1};
  return answ;
}