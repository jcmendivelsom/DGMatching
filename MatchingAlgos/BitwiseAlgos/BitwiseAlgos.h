#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <iostream>
#include <vector>

class BitwiseAlgos : public MatchingAlgos {

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> backwardScan(std::wstring t, std::wstring p, int delta,
                                int gamma);
  std::vector<int> forwardScan(std::wstring t, std::wstring p, int delta,
                               int gamma);
  std::vector<int> shiftPlus(std::wstring t, std::wstring p, int delta,
                             int gamma);
  std::vector<int> shiftAnd(std::wstring t, std::wstring p, int delta);
  std::vector<int> shiftOr(std::wstring t, std::wstring p);
};
