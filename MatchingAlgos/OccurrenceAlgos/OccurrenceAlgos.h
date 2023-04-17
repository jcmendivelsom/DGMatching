#pragma once
#include "MatchingAlgos.h"
#include <iostream>
#include <unordered_map>
#include <vector>

class OccurrenceAlgos : public MatchingAlgos {

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> bruteForce(std::wstring_view y, std::wstring_view x, int delta,
                                 int gamma = -1);
  std::vector<int> deltaSkipSearch(std::wstring_view t, std::wstring_view p, int delta,
                                   int gamma = -1);
};