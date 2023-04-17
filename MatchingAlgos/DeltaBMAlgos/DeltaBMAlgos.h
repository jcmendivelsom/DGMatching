#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <unordered_map>

class DeltaBMAlgos : public MatchingAlgos {

  // DELTA TUNED BOYER MOORE
  int backMinDeltaMatch(wchar_t a, std::wstring_view p, int delta);
  std::vector<int> deltaBadCharacter(std::wstring_view p, int delta);
  // DELTA FAST SEARCH
  std::vector<int> deltaSuffix(std::wstring_view p, int delta);
  std::vector<int> deltaGoodSuffix(std::wstring_view p, int delta);
  // DELTA FORWARD FAST SEARCH
  std::vector<std::vector<int>>
  deltaForwardGoodSuffix(std::wstring_view p, int delta,
                         std::vector<int> dBC = {});
  // DELTA BOYER MOORE 3
  long hash(std::wstring_view x);
  std::unordered_map<long int, std::vector<int>>
  computeHashTableIx(std::wstring_view x, int k, int delta);
  // DELTA BOYER MOORE 1
  typedef std::pair<int, int> Interval;
  class IntervalNode;
  class DeltaFactorTrie;
  // DELTA BOYER MOORE 2
public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> deltaTunedBM(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);
  std::vector<int> deltaFastSearch(std::wstring_view t, std::wstring_view p,
                                   int delta, int gamma = -1);
  std::vector<int> deltaForwardFastSearch(std::wstring_view t,
                                          std::wstring_view p, int delta,
                                          int gamma = -1);
  std::vector<int> deltaBM1(std::wstring_view t, std::wstring_view p, int delta,
                            int gamma = -1, int k = -1);
  std::vector<int> deltaBM3(std::wstring_view t, std::wstring_view p, int delta,
                            int gamma = -1, int k = 2);
};

class DeltaBMAlgos::IntervalNode {
  int alphMinVal;
  int alphMaxVal;
  Interval i;

public:
  std::vector<int> positions;
  std::vector<IntervalNode *> childrens;
  IntervalNode(int minVal, int maxVal, int a, int b);
  IntervalNode *buildTransition(int c, int delta);
  IntervalNode *getTransition(int c);
  void printNode();
};

class DeltaBMAlgos::DeltaFactorTrie {
  Alphabet innerAlph;
  IntervalNode *root;
  int delta;
  void insertFactor(std::wstring_view factor, int pos);
  void print(IntervalNode *inputNode);

public:
  int k;
  DeltaFactorTrie(Alphabet &innerAlph, std::wstring_view text, int delta,
                  int k = -1);
  IntervalNode *travelWith(std::wstring_view p);
  void printTrie();
};