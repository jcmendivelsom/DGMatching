#include "DeltaBMAlgos.h"
#include <algorithm> // std::sort, std::stable_sort
#include <iostream>
#include <numeric> // std::iota

std::vector<int> DeltaBMAlgos::deltaMaximalShift(std::wstring_view t,
                                                 std::wstring_view p, int delta,
                                                 int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }
  if (gamma < 0)
    gamma = delta * m;
  std::vector<int> answ;
  // PREPROCESSING PHASE.
  std::vector<int> perm = maxShiftPerm(p, delta);
  std::vector<int> dMax = deltaMax(p, delta, perm);
  std::vector<int> dBC = deltaBadCharacter(p, delta);

  // SEARCHING PHASE
  int j = 0, i, sum;
  while (j <= n - m) {
    i = 0;
    sum = 0;
    while (i < m &&
           std::abs(alph.getValue(p[perm[i]]) -
                    alph.getValue(t[j + perm[i]])) <= delta &&
           sum <= gamma) {
      sum +=
          std::abs(alph.getValue(p[perm[i]]) - alph.getValue(t[j + perm[i]]));
      i += 1;
    }
    // std::wcout << i << " $ " << j << " - " << j + (m - 1) << " # " <<
    // std::endl;
    if (i >= m && sum <= gamma)
      answ.push_back(j);
    // std::wcout << " **  " << std::max(dMax[perm[i]], dBC[t[j + m - 1]]) <<
    // std::endl;
    j += j + m < n ? std::max(dMax[perm[i]], dBC[alph.getIndex(t[j + m])]) : m;
  }

  if (answ.empty())
    return {-1};
  return answ;
}
std::vector<int> DeltaBMAlgos::deltaMax(std::wstring_view p, int delta,
                                        std::vector<int> &perm) {
  int m = p.length();
  std::vector<int> dMax(m, 1);
  // Calculate period of p
  int period;
  for (period = 1; period < m; ++period) {
    if (isDeltaGammaMatch(p.substr(0, m - period), p.substr(period), 2 * delta))
      break;
  }
  // D MAX
  bool flag;
  for (int i = 0; i < m; ++i) {
    for (int l = 1; l < m; ++l) {
      if (perm[i] - l >= 0 && std::abs(alph.getValue(p[perm[i] - l]) -
                                       alph.getValue(p[perm[i]])) <= delta)
        continue;
      flag = true;
      for (int j = 0; j < i; ++j) {
        if (perm[j] - l >= 0 &&
            std::abs(alph.getValue(p[perm[j] - l]) -
                     alph.getValue(p[perm[j]])) > 2 * delta) {
          flag = false;
          break;
        }
      }
      if (flag) {
        dMax[perm[i]] = l;
        break;
      }
    }
  }
  // Add dMax for m
  dMax.push_back(period);
  /*
  std::wcout << " DMAX" << std::endl;
  for (const auto &pos : dMax)
    std::wcout << pos << ", ";
  std::wcout << "\n";
  */
  return dMax;
}
std::vector<int> DeltaBMAlgos::maxShiftPerm(std::wstring_view p, int delta) {
  int m = p.length();
  std::vector<int> minShift(p.length());
  std::vector<int> per(p.length());

  int j;
  for (int i = 0; i < m; ++i) {
    for (j = i - 1; j >= 0; --j)
      if (std::abs(alph.getValue(p[j]) - alph.getValue(p[i])) <= delta)
        break;
    minShift[i] = i - j;
  }

  std::iota(per.begin(), per.end(), 0);
  // sort indexes based on comparing values in v
  // using std::stable_sort instead of std::sort
  // to avoid unnecessary index re-orderings
  // when v contains elements of equal values
  std::stable_sort(per.begin(), per.end(), [&minShift](int i1, int i2) {
    return minShift[i1] >= minShift[i2];
  });
  // Permutation of m is m!
  per.push_back(m);
  /*
  std::wcout << "Permutation \n";
  for (const auto &pos : per)
    std::wcout << pos << ", ";
  std::wcout << "\n";
  */
  return per;
}
