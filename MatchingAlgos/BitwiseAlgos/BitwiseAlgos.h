#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <iostream>
#include <vector>

class BitwiseAlgos : public MatchingAlgos {

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> backwardScan(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);
  std::vector<int> forwardScan(std::wstring_view t, std::wstring_view p,
                               int delta, int gamma = -1);
  std::vector<int> shiftPlus(std::wstring_view t, std::wstring_view p,
                             int delta, int gamma = -1);
  std::vector<int> shiftAnd(std::wstring_view t, std::wstring_view p,
                            int delta);
  std::vector<int> shiftOr(std::wstring_view t, std::wstring_view p);
};
