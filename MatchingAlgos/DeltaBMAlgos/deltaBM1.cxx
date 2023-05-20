#include "DeltaBMAlgos.h"


/////////////////// ALGORITHMS ///////////////////

std::vector<int> DeltaBMAlgos::deltaBM1(std::wstring_view t,
                                        std::wstring_view p, int delta,
                                        int gamma, int k) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || k > m || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Compute the Delta Factor Trie for all the k-factors.
  DeltaFactorTrie *trie = new DeltaFactorTrie(alph, p, delta, k);
  // Print the entire Delta Factor Trie
  // trie->printTrie();

  // SEARCHING PHASE
  int i = m, l;
  k = trie->k;
  std::vector<int> possiblePos;
  IntervalNode *traveler;
  while (i < n) {
    /*
    if (trie->travelWith(t.substr(i - k, k)) == NULL) {
      i += m - k;
      continue;
    }
    possiblePos = trie->travelWith(t.substr(i - k, k))->positions;
    */
    traveler = trie->root;
    l = 1;
    while (l <= k &&
           traveler->getTransition(alph.getValue(t[i - k + l])) != NULL) {
      traveler = traveler->getTransition(alph.getValue(t[i - k + l]));
      ++l;
    }
    possiblePos = traveler->positions;
    for (const auto &j : possiblePos) {
      // std::wcout << i << " - " << j << " : " << t.substr(i - k, k) << " * "
      // << t.substr(i - k - j + 1, m) << std::endl;
      if (i - l - j + 1 < 0 || i - l - j + m >= n)
        continue;
      if (isDeltaGammaMatch(p, t.substr(i - l - j + 1, m), delta, gamma)) {
        answ.push_back(i - l - j + 1);
      }
    }
    i += m - l + 1;
  }
  if (answ.empty())
    return {-1};
  return answ;
}

std::vector<int> DeltaBMAlgos::trieSearch(std::wstring_view t,
                                          std::wstring_view p, int delta,
                                          int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Compute the Delta Factor Trie for all the k-factors.
  DeltaFactorTrie *trie = new DeltaFactorTrie(alph, p, delta, m);
  // Print the entire Delta Factor Trie
  // trie->printTrie();

  // SEARCHING PHASE

  int i = m - 1, l = 0, last = -1, auxLast = 0;
  IntervalNode *traveler;
  std::unordered_map<int, int> possiblePos;
  // std::vector<int> possiblePos;
  while (i < n) {
    traveler = trie->root;
    l = 0;
    possiblePos = {};
    while (i + l < n &&
           traveler->getTransition(alph.getValue(t[i + l])) != NULL) {
      traveler = traveler->getTransition(alph.getValue(t[i + l]));
      l += 1;
      for (int j = 0; j < traveler->positions.size(); ++j) {
        possiblePos[l - traveler->positions[j]] = l;
      }
    }
    // possiblePos = traveler->positions;
    for (const auto &myPair : possiblePos) {
      // std::wcout << i << " " << i + l << "-" << myPair.first << " // \n";
      if (i + myPair.first - 1 <= last || i + myPair.first - 1 + m >= n)
        continue;
      // std::wcout << i + myPair.first - 1 << " : " << t.substr(i +
      // myPair.first - 1, m) << " \n ";
      if (isDeltaGammaMatch(p, t.substr(i + myPair.first - 1, m), delta,
                            gamma)) {
        answ.push_back(i + myPair.first - 1);
      }
      auxLast = i + myPair.first - 1;
    }
    last = auxLast;
    i += m - l + 1;
  }
  if (answ.empty())
    return {-1};
  return answ;
}

/////////////////// INTERVAL NODE ///////////////////

DeltaBMAlgos::IntervalNode::IntervalNode(int minVal, int maxVal, int a, int b) {
  this->i = Interval(a, b);
  this->alphMaxVal = maxVal;
  this->alphMinVal = minVal;
}

DeltaBMAlgos::IntervalNode *
DeltaBMAlgos::IntervalNode::buildTransition(int c, int delta) {
  Interval inDeltaC = Interval(std::max(alphMinVal, c - delta),
                               std::min(alphMaxVal, c + delta));
  bool intersectFlag = false;
  int lastInter = -1;
  std::unordered_map<int, int> toDelete;
  for (int i = 0; i < childrens.size(); ++i) {
    if (!(childrens[i]->i.second < inDeltaC.first ||
          childrens[i]->i.first > inDeltaC.second) &&
        i != lastInter && toDelete.count(i) == 0) {
      childrens[i]->i.first = std::min(childrens[i]->i.first, inDeltaC.first);
      childrens[i]->i.second =
          std::max(childrens[i]->i.second, inDeltaC.second);
      inDeltaC.first = childrens[i]->i.first;
      inDeltaC.second = childrens[i]->i.second;
      if (lastInter != -1) {
        for (const auto &pos : childrens[lastInter]->positions)
          childrens[i]->positions.push_back(pos);
        toDelete[lastInter] = 0;
      }
      lastInter = i;
      intersectFlag = true;
      i = 0;
    }
  }
  // Delete remaining intervals
  auto it = childrens.begin();
  for (const auto &del : toDelete) {
    childrens.erase(it + del.first);
  }
  // printNode();
  if (intersectFlag)
    return childrens[lastInter];

  // If get here means that we do not find a possible path
  IntervalNode *newChild =
      new IntervalNode(alphMinVal, alphMaxVal, inDeltaC.first, inDeltaC.second);
  childrens.push_back(newChild);
  return newChild;
}

DeltaBMAlgos::IntervalNode *DeltaBMAlgos::IntervalNode::getTransition(int c) {
  for (int i = 0; i < childrens.size(); ++i) {
    if (childrens[i] != NULL && childrens[i]->i.first <= c &&
        c <= childrens[i]->i.second)
      return childrens[i];
  }
  return NULL;
}

void DeltaBMAlgos::IntervalNode::printNode() {
  for (auto chil : childrens)
    std::wcout << "[" << chil->i.first << ", " << chil->i.second << "] ";
  std::wcout << "{ ";
  for (auto pos : positions)
    std::wcout << pos << ", ";
  std::wcout << " }\n";
}

/////////////////// DELTA FACTOR TRIE ///////////////////

void DeltaBMAlgos::DeltaFactorTrie::insertKFactor(std::wstring_view text) {
  int m = text.length();
  std::vector<std::wstring_view> factors;
  for (int i = 0; i <= m - this->k; ++i) {
    factors.push_back(text.substr(i, this->k));
  }
  IntervalNode *traveler;
  for (int i = 1; i <= this->k; ++i) {
    for (int j = 0; j < factors.size(); ++j) {
      traveler = root;
      int l;
      for (l = 0; l < i - 1; ++l) {
        traveler = traveler->getTransition(innerAlph.getValue(factors[j][l]));
      }
      traveler =
          traveler->buildTransition(innerAlph.getValue(factors[j][l]), delta);
      traveler->positions.push_back(j + l);
    }
  }
}

void DeltaBMAlgos::DeltaFactorTrie::insertSuffixes(std::wstring_view text) {
  int m = text.length();
  std::vector<std::wstring_view> factors;
  for (int i = 0; i < m; ++i) {
    factors.push_back(text.substr(i));
  }
  IntervalNode *traveler;
  for (int i = 1; i <= m; ++i) {
    for (int j = 0; j < factors.size(); ++j) {
      if (factors[j].length() >= i) {
        traveler = root;
        int l;
        for (l = 0; l < i - 1; ++l) {
          traveler = traveler->getTransition(innerAlph.getValue(factors[j][l]));
        }
        traveler =
            traveler->buildTransition(innerAlph.getValue(factors[j][l]), delta);
        traveler->positions.push_back(j + l);
      }
    }
    /*
    std::wcout << "###########" << std::endl;
    printTrie();
    std::wcout << "###########" << std::endl;
    */
  }
}

void DeltaBMAlgos::DeltaFactorTrie::print(IntervalNode *inputNode) {
  if (inputNode == NULL)
    return;
  inputNode->printNode();
  for (auto node : inputNode->childrens) {
    print(node);
  }
}

DeltaBMAlgos::DeltaFactorTrie::DeltaFactorTrie(Alphabet &innerAlph,
                                               std::wstring_view text,
                                               int delta, int inK) {
  this->innerAlph = innerAlph;
  this->delta = delta;
  if (inK <= 0)
    this->k = std::floor(std::log2(text.length()));
  else
    this->k = inK;
  this->root = new IntervalNode(
      this->innerAlph.getMinValue(), this->innerAlph.getMaxValue(),
      this->innerAlph.getMinValue(), this->innerAlph.getMaxValue());
  if (k < text.length()) {
    insertKFactor(text);
  } else {
    insertSuffixes(text);
  }
  // fixIntervals(this->root);
}

void DeltaBMAlgos::DeltaFactorTrie::printTrie() {
  std::wcout << "---------------------------" << std::endl;
  print(root);
  std::wcout << "---------------------------" << std::endl;
}

DeltaBMAlgos::IntervalNode *
DeltaBMAlgos::DeltaFactorTrie::travelWith(std::wstring_view p) {
  IntervalNode *traveler = root;
  int i = 0;
  while (i < p.length() &&
         traveler->getTransition(innerAlph.getValue(p[i])) != NULL) {
    traveler = traveler->getTransition(innerAlph.getValue(p[i]));
    ++i;
  }
  return traveler;
}