#include "OccurrenceAlgos.h"

std::vector<int> OccurrenceAlgos::deltaSkipSearch(std::wstring_view t,
                                                  std::wstring_view p, int delta,
                                                  int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }
  std::vector<int> answ;
  // PREPROCESSING PHASE.
  std::vector<int> posiTable[alph.size()];
  for (int i = 0; i < alph.size(); ++i) {
    for (int j = 0; j < m; ++j) {
      if (std::abs(alph.getValueByI(i) - alph.getValue(p[j])) <= delta)
        posiTable[i].push_back(j);
    }
  }
  // SEARCHING PHASE.
  int j = m - 1;
  while (j < n) {
    for (auto const &i : posiTable[alph.getIndex(t[j])]) {
      if (isDeltaGammaMatch(p, t.substr(j - i, m), delta, gamma)) {
        answ.push_back(j - i);
      }
    }
    j += m;
  }

  if (answ.empty())
    return {-1};
  return answ;
}