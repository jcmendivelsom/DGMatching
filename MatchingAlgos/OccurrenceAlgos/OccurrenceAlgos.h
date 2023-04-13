#pragma once
#include "MatchingAlgos.h"
#include <iostream>
#include <unordered_map>
#include <vector>

class OccurrenceAlgos : public MatchingAlgos {

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> bruteForce(std::wstring y, std::wstring x, int delta,
                                 int gamma = -1);
  std::vector<int> deltaSkipSearch(std::wstring t, std::wstring p, int delta,
                                   int gamma = -1);
};