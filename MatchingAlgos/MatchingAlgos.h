#pragma once
#include "Alphabet.h"
#include <iostream>
#include <sstream>
#include <vector>

class MatchingAlgos {
protected:
  Alphabet alph;

  int sumDeltaMatch(std::wstring x, std::wstring y, int delta);

public:
  MatchingAlgos(std::string filePath = "");
  MatchingAlgos(int begin, int end, int step = 1);

  bool isDeltaGammaMatch(std::wstring x, std::wstring y, int delta,
                         int gamma = -1);
  std::wstring getText(std::string filePath, bool isNumber = false);
};