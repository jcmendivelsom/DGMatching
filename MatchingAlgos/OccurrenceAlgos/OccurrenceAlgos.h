#pragma once
#include "MatchingAlgos.h"
#include <iostream>
#include <unordered_map>
#include <vector>

class OccurrenceAlgos : public MatchingAlgos {
  using Interval = std::pair<int, int>;
  struct IntervalNode;
  class IntervalTree;

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> bruteForce(std::wstring_view y, std::wstring_view x,
                              int delta, int gamma = -1);
  std::vector<int> deltaSkipSearch(std::wstring_view t, std::wstring_view p,
                                   int delta, int gamma = -1);
  std::vector<int> intervalSearch(std::wstring_view t, std::wstring_view p,
                                  int delta, int gamma = -1);
};

struct OccurrenceAlgos::IntervalNode {
  Interval i; // Data
  int max;    // Max val.
  std::vector<int> positions;
  IntervalNode *parent;
  IntervalNode *leftChild;
  IntervalNode *rightChild;
  bool color;
};

class OccurrenceAlgos::IntervalTree {
  IntervalNode *root;
  IntervalNode *tNILL;

  void initializeNULLNode(IntervalNode *node, IntervalNode *parent);
  void fixInsert(IntervalNode *x);
  void printAux(IntervalNode *x);

public:
  IntervalTree();
  void insert(Interval i, int pos);
  void rightRotate(IntervalNode *x);
  void leftRotate(IntervalNode *x);
  void print();
  void search(IntervalNode *x, int p, std::vector<int> *result);
  std::vector<int> getIntersections(int c);
};