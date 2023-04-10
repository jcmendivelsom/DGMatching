#pragma once
#include "Alphabet.h"
#include <iostream>
#include <vector>
class BitwiseAlgos {
  Alphabet alph;

public:
  BitwiseAlgos(std::string filePath = "");
  std::vector<int> backwardScan(std::wstring t, std::wstring p, int delta,
                             int gamma);
  std::vector<int> forwardScan(std::wstring t, std::wstring p, int delta,
                             int gamma);
  std::vector<int> shiftPlus(std::wstring t, std::wstring p, int delta,
                             int gamma);
  std::vector<int> shiftAnd(std::wstring t, std::wstring p, int delta);
  std::vector<int> shiftOr(std::wstring t, std::wstring p);
};
