#include "DeltaBMAlgos.h"

// true if the suffix of word starting from word[pos] is a prefix
// of word
bool DeltaBMAlgos::is_prefix(std::wstring_view p, ssize_t pos, int delta) {
  int suffixLen = p.length() - pos;
  for (int i = 0; i < suffixLen; i++) {
    // std::wcout << i << " " << pos + i << std::endl;
    if (std::abs(alph.getValue(p[i]) - alph.getValue(p[pos + i])) > delta) {
      return false;
    }
  }
  return true;
}

// length of the longest suffix of word ending on word[pos].
// suffix_length("dddbcabc", 8, 4) = 2
size_t DeltaBMAlgos::suffix_length(std::wstring_view p, ssize_t pos, int delta) {
  size_t i, pLen = p.length();
  // increment suffix length i to the first mismatch or beginning
  // of the word
  for (i = 0; (i <= pos) && (std::abs(alph.getValue(p[pos - i]) -
                                      alph.getValue(p[pLen - 1 - i])) <= delta);
       i++)
    ;
  return i;
}

std::vector<int> DeltaBMAlgos::make_delta2(std::wstring_view pat, int delta) {
  ssize_t p;
  size_t last_prefix_index = 1, patLen = pat.length();
  std::vector<int> delta2(patLen);
  // first loop
  for (p = patLen - 1; p >= 0; p--) {
    // std::wcout << " p=" << p << std::endl;
    if (is_prefix(pat, p + 1, 2 * delta)) {
      std::wcout << p <<" Prefeijo" << '\n';
      last_prefix_index = p + 1;
    }
    delta2[p] = (patLen - 1 - p) + last_prefix_index;
  }
  // std::wcout << " OK FIRST " << std::endl;
  std::wcout << "-------V2----------" << '\n';
  for (int i = 0; i < patLen; ++i)
    std::wcout << pat[i] << " dGS: " << delta2[i] << std::endl;
  std::wcout << "-------------------" << '\n';
  // second loop
  size_t slen;
  for (p = 0; p < patLen - 1; p++) {
    // std::wcout << "p=" << p << std::endl;
    slen = suffix_length(pat, p, 2 * delta);
    if (pat[p - slen] != pat[patLen - 1 - slen]) {
      delta2[patLen - 1 - slen] = patLen - 1 - p + slen;
    }
  }
  // std::wcout << " OK SECOND " << std::endl;
  // Print Good Suffix Table
  std::wcout << "-------V2----------" << '\n';
  for (int i = 0; i < patLen; ++i)
    std::wcout << pat[i] << " dGS: " << delta2[i] << std::endl;
  std::wcout << "-------------------" << '\n';

  return delta2;
}

/*
    deltaSuffix returns a int vector containing in position i the length of the
   longest suffix of 'p' that Delta matches a substring of p terminated in
   position i of 'p'.
*/
std::vector<int> DeltaBMAlgos::deltaSuffix(std::wstring_view p, int delta) {
  int m = p.length(), j = 0;
  std::vector<int> dSuffix(m, m);
  for (int i = m - 2; i >= 0; --i) {
    j = 0;
    while (std::abs(alph.getValue(p[m - 1 - j]) - alph.getValue(p[i - j])) <=
           delta) {
      j++;
      if (i - j < 0)
        break;
    }
    dSuffix[i] = j;
  }
  /*
    // Print Suffix Table
    std::wcout << "-------------------" << '\n';
    for (int i = 0; i < m; ++i)
      std::wcout << p[i] << " Suffix: " << dSuffix[i] << std::endl;
    std::wcout << "-------------------" << '\n';
  */
  return dSuffix;
}

/*
    deltaGoodSuffix returns a int vector containing in position j the minimum k
   that: p[j-k ... m-k-1] is a 2 * delta suffix of p[j ... m-1] and if k<=j-1
   then p[j-1]!=p[j-1-k], where m is the length of 'p'.
*/
std::vector<int> DeltaBMAlgos::deltaGoodSuffix(std::wstring_view p, int delta) {
  int m = p.length();
  std::vector<int> dGS(m, m), dSuffix = deltaSuffix(p, 2 * delta);
  for (int j = m - 1; j >= 0; --j) {
    for (int k = 1; k < m; ++k) {
      // Check the condition if k<=j-1 then p[j-1]!=p[j-1-k]
      if (k <= j - 1 && alph.getValue(p[j - 1]) == alph.getValue(p[j - 1 - k]))
        continue;
      // Check if the substring of 'p' terminated in m-k-1 of length m-j is a
      // 2*Delta suffix of 'p'
      if (std::min(m - j, m - k) <= dSuffix[m - k - 1]) {
        dGS[j] = k;
        break;
      }
    }
  }
  /*
  // Print Good Suffix Table
  std::wcout << "--------V1---------" << '\n';
  for (int i = 0; i < m; ++i)
    std::wcout << p[i] << " dGS: " << dGS[i] << std::endl;
  std::wcout << "-------------------" << '\n';
  */

  return dGS;
}

/*
    -> deltaFastSearch returns the indices of all Delta-Gamma matches of 'p' in
   't' by shifting using delta Bad Character Rule, then checking naively and
   moving by Good Suffix Rule.
*/
std::vector<int> DeltaBMAlgos::deltaFastSearch(std::wstring_view t,
                                               std::wstring_view p, int delta,
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
  // Delta Good Suffix - Table Shift for every char in the pattern.
  std::vector<int> dGSShift = deltaGoodSuffix(p, delta);
  // std::vector<int> ver2DGS = make_delta2(p, delta);
  // SEARCHING PHASE
  // t += std::wstring(m + 1, p[m - 1]);
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
      s += dGSShift[j + 1];
    }
  } catch (const std::out_of_range &e) {
    std::wcout << "";
  }
  if (answ.empty())
    return {-1};
  return answ;
}