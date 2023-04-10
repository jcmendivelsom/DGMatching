#pragma once
#include "Alphabet.h"
#include <iostream>
#include <vector>
class DeltaBMAlgos {
  Alphabet alph;

  int sumDeltaMatch(std::wstring x, std::wstring y, int delta);
  bool isDeltaGammaMatch(std::wstring x, std::wstring y, int delta, int gamma);
  // DELTA TUNED BOYER MOORE
  int minDeltaMatch(std::wstring p, wchar_t a, int delta);

public:
  DeltaBMAlgos(std::string filePath = "");
  std::vector<int> deltaTunedBM(std::wstring t, std::wstring p, int delta,
                                int gamma = -1);
  /*
  std::vector<int> forwardScan(std::wstring t, std::wstring p, int delta,
                               int gamma);
  std::vector<int> shiftPlus(std::wstring t, std::wstring p, int delta,
                             int gamma);
  std::vector<int> shiftAnd(std::wstring t, std::wstring p, int delta);
  */
};