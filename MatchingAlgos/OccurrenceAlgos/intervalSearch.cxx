#include "OccurrenceAlgos.h"
#include <iostream>

///////////// ALGORITHM //////////////////

std::vector<int> OccurrenceAlgos::intervalSearch(std::wstring_view t,
                                                 std::wstring_view p, int delta,
                                                 int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }
  std::vector<int> answ;

  IntervalTree iTree = IntervalTree();
  Interval aux;
  for (int i = 0; i < p.length(); ++i) {
    aux = {std::max(alph.getMinValue(), alph.getValue(p[i]) - delta),
           std::min(alph.getMaxValue(), alph.getValue(p[i]) + delta)};
    iTree.insert(aux, i);
  }
  // iTree.print();
  int i = m - 1;
  while (i < n) {
    for (auto pos : iTree.getIntersections(alph.getValue(t[i]))) {
      // std::wcout << i - pos << " * " << t.substr(i - pos, m) << "\n";
      if (i - pos < 0 || i - pos + m >= n)
        continue;
      if (isDeltaGammaMatch(p, t.substr(i - pos, m), delta, gamma)) {
        answ.push_back(i - pos);
      }
    }
    i += m;
  }
  return answ;
};

///////////////////////////////////////////////////
void OccurrenceAlgos::IntervalTree::initializeNULLNode(IntervalNode *node,
                                                       IntervalNode *parent) {
  node->i = {INT32_MIN, INT32_MIN};
  node->parent = parent;
  node->leftChild = nullptr;
  node->rightChild = nullptr;
  node->color = 0;
};

void OccurrenceAlgos::IntervalTree::rightRotate(IntervalNode *x) {
  IntervalNode *y = x->leftChild;
  x->leftChild = y->rightChild;
  if (y->rightChild != tNILL) {
    y->rightChild->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == nullptr) {
    this->root = y;
  } else if (x == x->parent->rightChild) {
    x->parent->rightChild = y;
  } else {
    x->parent->leftChild = y;
  }
  y->rightChild = x;
  x->parent = y;
};

void OccurrenceAlgos::IntervalTree::leftRotate(IntervalNode *x) {
  IntervalNode *y = x->rightChild;
  x->rightChild = y->leftChild;
  if (y->leftChild != tNILL) {
    y->leftChild->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == nullptr) {
    this->root = y;
  } else if (x == x->parent->leftChild) {
    x->parent->leftChild = y;
  } else {
    x->parent->rightChild = y;
  }
  y->leftChild = x;
  x->parent = y;
};

void OccurrenceAlgos::IntervalTree::fixInsert(IntervalNode *k) {
  IntervalNode *u;
  while (k->parent->color == 1) {
    if (k->parent == k->parent->parent->rightChild) {
      u = k->parent->parent->leftChild; // uncle
      if (u->color == 1) {
        // case 3.1
        u->color = 0;
        k->parent->color = 0;
        k->parent->parent->color = 1;
        k = k->parent->parent;
      } else {
        if (k == k->parent->leftChild) {
          // case 3.2.2
          k = k->parent;
          rightRotate(k);
        }
        // case 3.2.1
        k->parent->color = 0;
        k->parent->parent->color = 1;
        leftRotate(k->parent->parent);
      }
    } else {
      u = k->parent->parent->rightChild; // uncle

      if (u->color == 1) {
        // mirror case 3.1
        u->color = 0;
        k->parent->color = 0;
        k->parent->parent->color = 1;
        k = k->parent->parent;
      } else {
        if (k == k->parent->rightChild) {
          // mirror case 3.2.2
          k = k->parent;
          leftRotate(k);
        }
        // mirror case 3.2.1
        k->parent->color = 0;
        k->parent->parent->color = 1;
        rightRotate(k->parent->parent);
      }
    }
    if (k == root) {
      break;
    }
  }
  root->color = 0;
};

OccurrenceAlgos::IntervalTree::IntervalTree() {
  tNILL = new IntervalNode;
  tNILL->color = 0;
  tNILL->i = {INT32_MIN, INT32_MIN};
  tNILL->max = INT32_MIN;
  tNILL->leftChild = nullptr;
  tNILL->rightChild = nullptr;
  root = tNILL;
};

void OccurrenceAlgos::IntervalTree::insert(Interval i, int pos) {
  IntervalNode *node = new IntervalNode();
  node->parent = nullptr;
  node->i = i;
  node->leftChild = tNILL;
  node->rightChild = tNILL;
  node->color = 1; // new node must be red
  node->max = i.second;
  node->positions.push_back(pos);

  IntervalNode *y = nullptr;
  IntervalNode *x = this->root;

  while (x != tNILL) {
    y = x;
    if (node->i.first < x->i.first) {
      x = x->leftChild;
    } else {
      x = x->rightChild;
    }
    if (y->max < i.second)
      y->max = i.second;
  }

  // y is parent of x
  node->parent = y;
  if (y == nullptr) {
    root = node;
  } else if (node->i.first < y->i.first) {
    y->leftChild = node;
  } else if (node->i.first == y->i.first && node->i.second == y->i.second) {
    y->positions.push_back(pos);
  } else {
    y->rightChild = node;
  }

  if (y != nullptr && y->max < i.second)
    y->max = i.second;

  // if new node is a root node, simply return
  if (node->parent == nullptr) {
    node->color = 0;
    return;
  }

  // if the grandparent is null, simply return
  if (node->parent->parent == nullptr) {
    return;
  }

  // Fix the tree
  fixInsert(node);
};

void OccurrenceAlgos::IntervalTree::printAux(IntervalNode *x) {
  if (x == tNILL)
    return;
  std::wcout << "[" << x->i.first << "," << x->i.second << "]"
             << " - " << x->max << " : ";
  for (auto pos : x->positions)
    std::wcout << pos << " ";
  std::wcout << std::endl;
  printAux(x->leftChild);
  printAux(x->rightChild);
};

void OccurrenceAlgos::IntervalTree::print() { printAux(root); };

void OccurrenceAlgos::IntervalTree::search(IntervalNode *x, int p,
                                           std::vector<int> *result) {
  if (x == tNILL)
    return;
  // If p is to the right of the rightmost point of any interval
  // in this node and all children, there won't be any matches.
  if (p > x->max)
    return;

  // Search left children
  search(x->leftChild, p, result);

  // Check this node
  if (x->i.first <= p && p <= x->i.second) {
    for (auto pos : x->positions) {
      // std::wcout << pos << " ";
      result->push_back(pos);
    }
    // result.add(n.getKey());
    // std::wcout << std::endl;
  }

  // If p is to the left of the start of this interval,
  // then it can't be in any child to the right.
  if (p < x->i.first)
    return;

  // Otherwise, search right children
  search(x->rightChild, p, result);
};
std::vector<int> OccurrenceAlgos::IntervalTree::getIntersections(int c) {
  std::vector<int> answ;
  search(root, c, &answ);
  return answ;
};