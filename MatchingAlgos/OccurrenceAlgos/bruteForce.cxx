#include "OccurrenceAlgos.h"

/*
    -> bruteForceDGm computes all the Delta-Gamma matches of a
pattern 'x' in a text input 'y' by checking every index in the text (brute
force). If Gamma is negative just check the Delta match.
*/
std::vector<int> OccurrenceAlgos::bruteForce(std::wstring_view t, std::wstring_view p,
                                                int delta, int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }
  // SEARCHING PHASE
  std::vector<int> answ; // Vector to save the matching indexes answers
  int sum, j;
  for (int i = 0; i <= n - m; ++i) {
    // Check if from index 'i' there is a Delta-Gamma match.
    if (isDeltaGammaMatch(p, t.substr(i, m), delta, gamma))
      answ.push_back(i);
  }
  if (answ.empty())
    return {-1};
  return answ;
}