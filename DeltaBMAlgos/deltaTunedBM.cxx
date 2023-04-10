#include "Alphabet.h"
#include "DeltaBMAlgos.h"
#include <iostream>
#include <vector>

/*
    CONSTRUCTOR of the class DeltaBMAlgos
*/
DeltaBMAlgos::DeltaBMAlgos(std::string alphPath) {
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
}

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
int DeltaBMAlgos::minDeltaMatch(std::wstring p, wchar_t a, int delta) {
  if (alph.getIndex(a) == -1) {
    std::wcout << "The following character is not in the alphabet: " << a
               << std::endl;
    throw std::invalid_argument("Invalid parameters! ");
  }
  int m = p.length();
  for (int i = m - 1; i >= 0; --i) {
    if (alph.getIndex(p[i]) == -1) {
      std::wcout << "The following character is not in the alphabet: " << p[i]
                 << std::endl;
      throw std::invalid_argument("Invalid parameters! ");
    }
    if (std::abs(alph.getValue(p[i]) - alph.getValue(a)) <= delta)
      return m - (i + 1);
  }
  return m;
}

/*
    -> deltaTunedBM computes all the Delta-Gamma matches of a pattern 'x' in a
   text 'y' by by doing an adapted version of the Tuned Boyer Moore Algorithm.
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
  int DShift[alph.size()]; // Table Shift for every element in the alphabet.
  for (int i = 0; i < alph.size(); ++i) {
    DShift[i] = minDeltaMatch(p, alph.getWChar(i), delta);
  }
  // First shift
  int s = minDeltaMatch(p.substr(0, m - 1), p[m - 1], 2 * delta) + 1;
  // Add to the end of the text p[m-1]^m.
  t += std::wstring(m, p[m - 1]);
  int k, j = m;
  while (j <= n) {
    if (alph.getIndex(t[j - 1]) == -1) {
      std::wcout << "The following character is not in the alphabet: "
                 << t[j - 1] << std::endl;
      throw std::invalid_argument("Invalid parameters! ");
    }
    k = DShift[alph.getIndex(t[j - 1])];
    while (k != 0) {
      std::cout << "J" << j << " ";
      j += k;
      k = DShift[alph.getIndex(t[j - 1])];
    }
    if (isDeltaGammaMatch(p, t.substr(j - m, m), delta, gamma) && j <= n)
      answ.push_back(j - m);
    j += s;
  }
  if (answ.empty())
    return {-1};
  return answ;
}