#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

class DeltaBMAlgos : public MatchingAlgos {

  // DELTA TUNED BOYER MOORE
  int sumDeltaMatch(std::wstring x, std::wstring y, int delta);
  bool isDeltaGammaMatch(std::wstring x, std::wstring y, int delta, int gamma);
  int backMinDeltaMatch(std::wstring a, std::wstring p, int delta,
                        bool justSuffix = false);
  std::vector<int> deltaBadCharacter(std::wstring p, int delta);
  // DELTA FAST SEARCH
  std::vector<int> deltaSuffix(std::wstring p, int delta);
  std::vector<int> deltaGoodSuffix(std::wstring p, int delta);
  // DELTA FORWARD FAST SEARCH
  std::vector<std::vector<int>>
  deltaForwardGoodSuffix(std::wstring p, int delta, std::vector<int> dBC = {});
  // DELTA BOYER MOORE 1
  typedef std::pair<int, int> Interval;
  class IntervalNode {
    Interval i;
    std::vector<Interval> childrenIntervals;
    std::vector<IntervalNode *> childrens;
    std::vector<int> positions;

  public:
    // IntervalNode(int a, int b);
    // IntervalNode *buildTransition(int c, int delta);
    // IntervalNode *getTransition(int c);
    // void printNode();
  };
  class DeltaFactorTrie {
    IntervalNode *root;
    std::string text;
    int delta;
    // void insertFactor(int pos);
    // void print(IntervalNode *inputNode);

  public:
    int k;
    // DeltaFactorTrie(std::string text, int delta);
    // IntervalNode *travelWith(std::string p);
    // void printTrie();
  };
  // DELTA BOYER MOORE 2
  // DELTA BOYER MOORE 3
  long hash(std::wstring x);
  std::unordered_map<long int, std::vector<int>>
  computeHashTableIx(std::wstring x, int k, int delta);

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> deltaTunedBM(std::wstring t, std::wstring p, int delta,
                                int gamma = -1);
  std::vector<int> deltaFastSearch(std::wstring t, std::wstring p, int delta,
                                   int gamma = -1);
  std::vector<int> deltaForwardFastSearch(std::wstring t, std::wstring p,
                                          int delta, int gamma = -1);
  std::vector<int> deltaBM3(std::wstring t, std::wstring p, int delta,
                            int gamma = -1, int k = 2);
};