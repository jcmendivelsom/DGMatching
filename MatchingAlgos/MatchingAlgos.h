#pragma once
#include "Alphabet.h"
#include <iostream>
#include <vector>
class MatchingAlgos {
protected:
  Alphabet alph;

  int sumDeltaMatch(std::wstring x, std::wstring y, int delta);

public:
  MatchingAlgos(std::string filePath = "");
  bool isDeltaGammaMatch(std::wstring x, std::wstring y, int delta,
                         int gamma = -1);
};