#pragma once
#include "MatchingAlgos.h"
#include <cmath>
#include <queue>
#include <set>
#include <unordered_map>
#include <map>

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
  class IntervalState;
  class DeltaSuffixAutomaton;

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
  std::vector<int> deltaBM2(std::wstring_view t, std::wstring_view p, int delta);
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

class DeltaBMAlgos::IntervalState {
  public:
  int len;
  int link;
  int first;
  int index;
  bool clone;
  bool terminal;
  std::map<Interval, int> transitions;
  std::unordered_map<int, int> travelTransitions;
  std::vector<int> suffixReferences;
  Interval boundVals;

  IntervalState();
  void addTransition(Interval in, int s);
  // Returns the index of a state or -1 if no transition exists for c
  int getTransition(Interval in);
  // Returns the index of a state or -1 if no transition exists for c
  std::vector<int> travelWith(int c);
  int travel(int c);
  // Updates the transition through c to a new index i
  void updateTransition(Interval in, int s);
};

class DeltaBMAlgos::DeltaSuffixAutomaton {
  public:
  bool suffixReferences = false;
  std::vector<IntervalState> states;
  int delta;
  // Returns the state at index i
  IntervalState getState(int i);
  // Create a new state and return its index (requires t0 already initialized)
  int addState(int len);
  // Populate each state with a vector of its children in the link tree
  void computeSuffixReferences();

  DeltaSuffixAutomaton(Alphabet alpha, std::wstring_view s, int delta);
  // MAKE IT DETERMINISTIC FINITE AUTOMATON
  void makeDeterministic();
  void fillTransitions();
  void printAutomaton();
};