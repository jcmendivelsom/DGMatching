#include "MatchingAlgos.h"

/*
    -> sumDeltaMatch return the sum of the value differences char. by char. But
   if find a Delta mismatch returns -1.
*/
int MatchingAlgos::sumDeltaMatch(std::wstring x, std::wstring y, int delta) {
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
    CONSTRUCTOR of the class MatchingAlgos just initialize the alphabet.
*/
MatchingAlgos::MatchingAlgos(std::string alphPath) {
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
}

/*
     -> isDeltaGammaMatch return whether 'x' Delta match 'y' or not. If Gamma is
   negative just check the Delta match.
*/
bool MatchingAlgos::isDeltaGammaMatch(std::wstring x, std::wstring y, int delta,
                                     int gamma) {
  int aux = sumDeltaMatch(x, y, delta);
  return 0 <= aux && (aux <= gamma || gamma < 0);
}