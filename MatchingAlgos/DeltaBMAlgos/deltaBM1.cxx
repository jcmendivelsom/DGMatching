#include "DeltaBMAlgos.h"
#include <chrono>
#include <iomanip>

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
  k = trie->k;
  int i = m - 1, l;
  // std::set<int> possiblePos;
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
    l = 0;
    while (l <= k &&
           traveler->getTransition(alph.getValue(t[i - k + l + 1])) != NULL) {
      // std::wcout << "->" << i - k + l + 1 << " + " << alph.getValue(t[i - k +
      // l + 1])<<"\n";
      traveler = traveler->getTransition(alph.getValue(t[i - k + l + 1]));
      l++;
    }
    // std::wcout << "%" << l << "\n";
    if (l < k) {
      i += m - k + 1;
      continue;
    }
    // possiblePos = traveler->positions;
    for (const auto &j : traveler->positions) {
      // std::wcout << i << " - " << j << " : " << i - j << " * " << i - j + m -
      // 1 << std::endl;
      if (i - j < 0 || i - j + m - 1 >= n)
        continue;
      if (isDeltaGammaMatch(p, t.substr(i - j, m), delta, gamma)) {
        answ.push_back(i - j);
      }
    }
    i += m - k + 1;
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
  // auto start = std::chrono::high_resolution_clock::now();
  DeltaFactorTrie *trie = new DeltaFactorTrie(alph, p, delta, m);
  // auto end = std::chrono::high_resolution_clock::now();
  // double time_taken =
  // std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  // time_taken *= 1e-9;
  /*
  std::wcout << "Time building trie : " << std::fixed << time_taken
             << std::setprecision(9);
  std::wcout << " sec" << std::endl;
  */
  // Print the entire Delta Factor Trie
  // trie->printTrie();

  // SEARCHING PHASE

  int i = m - 1, l = 0, last = -1, auxLast = 0;
  IntervalNode *traveler;
  std::set<int> possiblePos;
  // std::vector<int> possiblePos;
  while (i < n) {
    traveler = trie->root;
    l = 0;
    possiblePos = {};
    while (i + l < n &&
           traveler->getTransition(alph.getValue(t[i + l])) != NULL) {
      traveler = traveler->getTransition(alph.getValue(t[i + l]));
      /*
      for (const auto &pos : traveler->positions) {
        possiblePos.insert(l - pos - 1);
      }
      */
      // possiblePos.merge(traveler->positions);
      l += 1;
      if (traveler->positions.count(m - 1) != 0) {
        possiblePos.insert(m - 1 + l - 1);
        // auxLast = std::max(i + l - m - 2, auxLast);
      }
    }
    for (const auto &pos : traveler->positions) {
      possiblePos.insert(pos + l - 1);
    }
    // possiblePos = traveler->positions;
    // std::wcout << "**" << last << " \n ";
    for (const auto &j : possiblePos) {
      // std::wcout << i << " " << i + l << "-" << myPair.first << " // \n";
      if (i - j <= last || i - j + m - 1 >= n)
        continue;
      // std::wcout << i + myPair.first - 1 << " : " << t.substr(i +
      // myPair.first - 1, m) << " \n ";
      // std::wcout << "::" << i - j << " \n ";
      if (isDeltaGammaMatch(p, t.substr(i - j, m), delta, gamma)) {
        answ.push_back(i - j);
      }
      auxLast = std::max(i - j, auxLast);
    }
    last = auxLast;
    i = std::max(i + m - l + 1, last + 1);
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
          childrens[i]->positions.insert(pos);
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
      traveler->positions.insert(j + l);
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
        traveler->positions.insert(j + l);
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