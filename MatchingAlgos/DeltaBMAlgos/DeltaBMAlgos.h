#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>

class DeltaBMAlgos : public MatchingAlgos {

  // DELTA TUNED BOYER MOORE
  int backMinDeltaMatch(wchar_t a, std::wstring_view p, int delta);
  std::vector<int> deltaBadCharacter(std::wstring_view p, int delta);
  // MAXIMAL SHIFT
  std::vector<int> deltaMax(std::wstring_view p, int delta,
                            std::vector<int> &perm);
  std::vector<int> maxShiftPerm(std::wstring_view p, int delta);
  // DELTA FAST SEARCH
  bool is_prefix(std::wstring_view p, ssize_t pos, int delta);
  size_t suffix_length(std::wstring_view p, ssize_t pos, int delta);
  std::vector<int> make_delta2(std::wstring_view pat, int delta);
  std::vector<int> deltaSuffix(std::wstring_view p, int delta);
  std::vector<int> deltaGoodSuffix(std::wstring_view p, int delta);
  // DELTA FORWARD FAST SEARCH
  std::vector<std::vector<int>>
  deltaForwardGoodSuffix(std::wstring_view p, int delta,
                         std::vector<int> dBC = {});
  // DELTA BOYER MOORE 3
  long hash(std::wstring_view x);
  std::unordered_map<long int, std::vector<int>>
  computeHashTableIx(std::wstring_view x, int k, int delta, int gamma);
  // DELTA BOYER MOORE 1
  using Interval = std::pair<int, int>;
  class IntervalNode;
  class DeltaFactorTrie;
  // DELTA BOYER MOORE 2
  class IntervalState;
  class DeltaSuffixAutomaton;

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> deltaTunedBM(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);
  std::vector<int> deltaFastSearch(std::wstring_view t, std::wstring_view p,
                                   int delta, int gamma = -1);
  std::vector<int> deltaMaximalShift(std::wstring_view t, std::wstring_view p,
                                     int delta, int gamma = -1);
  std::vector<int> deltaForwardFastSearch(std::wstring_view t,
                                          std::wstring_view p, int delta,
                                          int gamma = -1);
  std::vector<int> deltaBM1(std::wstring_view t, std::wstring_view p, int delta,
                            int gamma = -1, int k = -1);
  std::vector<int> trieSearch(std::wstring_view t, std::wstring_view p,
                              int delta, int gamma);
  std::vector<int> deltaBM2(std::wstring_view t, std::wstring_view p, int delta,
                            int gamma = -1);
  std::vector<int> deltaBM3(std::wstring_view t, std::wstring_view p, int delta,
                            int gamma = -1, int k = 2);
};

class DeltaBMAlgos::IntervalNode {
  int alphMinVal;
  int alphMaxVal;
  Interval i;

public:
  std::set<int> positions;
  std::vector<IntervalNode *> childrens;
  IntervalNode(int minVal, int maxVal, int a, int b);
  IntervalNode *buildTransition(int c, int delta);
  IntervalNode *getTransition(int c);
  void printNode();
};

class DeltaBMAlgos::DeltaFactorTrie {
  Alphabet innerAlph;
  int delta;
  void insertKFactor(std::wstring_view text);
  void insertSuffixes(std::wstring_view text);
  void print(IntervalNode *inputNode);

public:
  IntervalNode *root;
  int k;
  DeltaFactorTrie(Alphabet &innerAlph, std::wstring_view text, int delta,
                  int k = -1);
  IntervalNode *travelWith(std::wstring_view p);
  void printTrie();
};

class DeltaBMAlgos::IntervalState {
public:
  std::map<Interval, int> intervalTransitions;
  std::unordered_map<int, int> travelTransitions;
  Interval boundVals;
  int len;
  int link;
  bool terminal;

  IntervalState();
  void addTransition(Interval in, int s);
  // Returns the index of a state or -1 if no transition exists for c
  // int getTransition(Interval in);
  // Returns the index of a state or -1 if no transition exists for c
  auto intervalTravelWith(int c) -> std::vector<int>;
  // Updates the transition through c to a new index i
  // void updateTransition(Interval in, int s);
  void setTravelTransitions();
  void setBoundVals();
};

class DeltaBMAlgos::DeltaSuffixAutomaton {
  std::vector<IntervalState> states;
  int delta;
  int size;
  int last;

  // Returns the state at index i
  auto getState(int i) -> IntervalState;
  void dSuffixAExtend(Interval in);
  // MAKE IT DETERMINISTIC FINITE AUTOMATON
  void makeDeterministic();
  void fillTravelTransitions();

public:
  DeltaSuffixAutomaton(Alphabet alpha, std::wstring_view s, int delta);
  void printAutomaton();
  auto travelWith(int indexState, int val) -> int;
  auto isTerminal(int indexState) -> bool;
};