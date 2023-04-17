#pragma once
#include "Alphabet.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string_view>

class MatchingAlgos {
protected:
  Alphabet alph;

  int sumDeltaMatch(std::wstring_view x, std::wstring_view y, int delta);

public:
  MatchingAlgos(std::string filePath = "");
  MatchingAlgos(int begin, int end, int step = 1);

  bool isDeltaGammaMatch(std::wstring_view x, std::wstring_view y, int delta,
                         int gamma = -1);
};
// UTIL FUNCTIONS
std::wstring getText(std::string filePath, bool isNumber = false);