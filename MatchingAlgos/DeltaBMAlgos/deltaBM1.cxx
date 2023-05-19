#include "DeltaBMAlgos.h"
#include <pstl/glue_algorithm_defs.h>

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
  int i = m;
  k = trie->k;
  std::vector<int> possiblePos;
  while (i < t.length()) {
    if (trie->travelWith(t.substr(i - k, k)) == NULL) {
      i += m - k + 1;
      continue;
    }
    possiblePos = trie->travelWith(t.substr(i - k, k))->positions;
    for (int j = 0; j < possiblePos.size(); ++j) {
      if (i - k - possiblePos[j] < 0 || i - k - possiblePos[j] + m > t.length())
        continue;
      if (isDeltaGammaMatch(p, t.substr(i - k - possiblePos[j], m), delta,
                            gamma)) {
        answ.push_back(i - k - possiblePos[j]);
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
      if (i + myPair.first - 1 <= last ||
          i + myPair.first - 1 + m >= n)
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

  for (int i = 0; i < childrens.size(); ++i) {
    if (!(childrens[i]->i.second < inDeltaC.first ||
          childrens[i]->i.first > inDeltaC.second)) {
      childrens[i]->i.first = std::min(childrens[i]->i.first, inDeltaC.first);
      childrens[i]->i.second =
          std::max(childrens[i]->i.second, inDeltaC.second);
      return childrens[i];
    }
  }
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

void DeltaBMAlgos::IntervalNode::checkIntervals() {
  if (childrens.empty())
    return;
  std::vector<IntervalNode *> goodChilds;
  IntervalNode *aux;
  Interval auxInt;
  // std::vector<IntervalNode *>::iterator mit = childrens.begin();
  for (int i = 0; i < childrens.size(); ++i) {
    for (int j = i + 1; j < childrens.size(); ++j) {
      if (childrens[i] != NULL && childrens[j] != NULL &&
          !(childrens[i]->i.second < childrens[j]->i.first ||
            childrens[i]->i.first > childrens[j]->i.second)) {
        aux = new IntervalNode(
            childrens[j]->alphMinVal, childrens[j]->alphMaxVal,
            std::min(childrens[i]->i.first, childrens[j]->i.first),
            std::max(childrens[i]->i.second, childrens[j]->i.second));
        aux->childrens = childrens[i]->childrens;
        for (auto p : childrens[j]->childrens) {
          aux->childrens.push_back(p);
        }
        aux->positions = childrens[i]->positions;
        for (auto p : childrens[j]->positions) {
          aux->positions.push_back(p);
        }
        childrens.push_back(aux);
        childrens[j] = NULL;
        childrens[i] = NULL;
      }
    }
  }
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

void DeltaBMAlgos::DeltaFactorTrie::insertFactor(std::wstring_view factor,
                                                 int pos) {
  IntervalNode *traveler = root;
  for (int i = 0; i < factor.length(); ++i)
    traveler = traveler->buildTransition(innerAlph.getValue(factor[i]), delta);
  traveler->positions.push_back(pos);
}

void DeltaBMAlgos::DeltaFactorTrie::insertSuffix(std::wstring_view factor,
                                                 int pos) {
  IntervalNode *traveler = root;
  for (int i = 0; i < factor.length(); ++i) {
    traveler = traveler->buildTransition(innerAlph.getValue(factor[i]), delta);
    traveler->positions.push_back(pos + i);
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
                                               int delta, int k) {
  this->innerAlph = innerAlph;
  this->delta = delta;
  if (k <= 0)
    this->k = std::floor(std::log2(text.length()));
  else
    this->k = k;
  this->root = new IntervalNode(
      this->innerAlph.getMinValue(), this->innerAlph.getMaxValue(),
      this->innerAlph.getMinValue(), this->innerAlph.getMaxValue());
  if (k < text.length()) {
    for (int i = 0; i <= text.length() - this->k; ++i) {
      insertFactor(text.substr(i, this->k), i);
    }
  } else {
    for (int i = 0; i < text.length(); ++i) {
      insertSuffix(text.substr(i), i);
    }
  }
  // fixIntervals(this->root);
}

void DeltaBMAlgos::DeltaFactorTrie::printTrie() {
  std::wcout << "---------------------------" << std::endl;
  print(root);
  std::wcout << "---------------------------" << std::endl;
}

void DeltaBMAlgos::DeltaFactorTrie::fixIntervals(IntervalNode *inputNode) {
  if (inputNode == NULL)
    return;
  for (auto node : inputNode->childrens) {
    fixIntervals(node);
  }
  inputNode->checkIntervals();
}

DeltaBMAlgos::IntervalNode *
DeltaBMAlgos::DeltaFactorTrie::travelWith(std::wstring_view p) {
  IntervalNode *traveler = root;
  int i = 0;
  while (traveler != NULL && i < p.length()) {
    traveler = traveler->getTransition(innerAlph.getValue(p[i]));
    ++i;
  }
  return traveler;
}

/* Delta Node: [a, b] -> Interval of acceptance
               Children -> map of pointers to child nodes

class DeltaNode
{
public:
    int a;
    int b;
    std::vector<Interval> childrenIntervals;
    std::vector<DeltaNode *> childrens;
    std::vector<int> positions;
    DeltaNode *link;
    DeltaNode(int a, int b)
    {
        link = NULL;
        this->a = a;
        this->b = b;
    }
    DeltaNode *buildTransition(int c, int delta)
    {
        Interval inDeltaC = Interval(std::max(MIN_CHAR, c - delta),
std::min(MAX_CHAR, c + delta));

        for (int i = 0; i < childrenIntervals.size(); ++i)
        {
            if (!(childrenIntervals[i].second < inDeltaC.first ||
inDeltaC.second < childrenIntervals[i].first))
            {
                childrenIntervals[i].first =
std::min(childrenIntervals[i].first, inDeltaC.first);
                childrenIntervals[i].second =
std::max(childrenIntervals[i].second, inDeltaC.second); return childrens[i];
            }
        }
        // If get here means that we do not find a possible path
        childrenIntervals.push_back(inDeltaC);
        DeltaNode *newChild = new DeltaNode(inDeltaC.first, inDeltaC.second);
        childrens.push_back(newChild);
        return newChild;
    }
    DeltaNode *getTransition(int c)
    {
        for (int i = 0; i < childrenIntervals.size(); ++i)
        {
            if (childrenIntervals[i].first <= c && c <=
childrenIntervals[i].second) return childrens[i];
        }
        return NULL;
    }
    void printNode()
    {
        for (auto intvl : childrenIntervals)
            std::cout << "[" << intvl.first - (int)' ' << ", " << intvl.second
- (int)' ' << "] "; std::cout << "{ "; for (auto pos : positions) std::cout <<
pos
<< ", "; std::cout << " }\n";
    }
};
class DeltaFactorTrie
{
    DeltaNode *root;
    std::string text;
    int delta;
    void insertFactor(int pos)
    {
        std::string currFactor = text.substr(pos, k);
        DeltaNode *traveler = root;
        for (int i = 0; i < currFactor.length(); ++i)
            traveler = traveler->buildTransition(currFactor[i], delta);
        traveler->positions.push_back(pos);
    }
    void print(DeltaNode *inputNode)
    {
        if (inputNode == NULL)
            return;
        inputNode->printNode();
        for (auto node : inputNode->childrens)
        {
            print(node);
        }
    }

public:
    int k;
    DeltaFactorTrie(std::string text, int delta)
    {
        this->text = text;
        this->delta = delta;
        root = new DeltaNode(MIN_CHAR - 1, MAX_CHAR);
        k = std::floor(std::log2(text.length()));
        for (int i = 0; i <= text.length() - k; ++i)
        {
            insertFactor(i);
        }
    }

    void printTrie()
    {
        print(root);
    }

    DeltaNode *travelWith(std::string p)
    {
        DeltaNode *traveler = root;
        int i = 0;
        while (traveler != NULL && i < p.length())
        {
            traveler = traveler->getTransition(p[i]);
            ++i;
        }
        return traveler;
    }
};

std::string deltaBM1SkipSearch(std::string t, std::string p, unsigned int
delta)
{
    int m = p.length();
    int n = t.length();
    if (m <= 0 || m > n)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }
    std::string answ = "";
    // Preprocessing
    DeltaFactorTrie *trie = new DeltaFactorTrie(p, delta);
    std::cout << "---------------------------" << std::endl;
    // Print the entire delta hash table
    trie->printTrie();
    std::cout << "---------------------------" << std::endl;
    // Searching
    int i = m, k = trie->k;
    std::vector<int> possiblePos;
    while (i < t.length())
    {
        if (trie->travelWith(t.substr(i - k, k)) == NULL)
        {
            i += m - k + 1;
            continue;
        }
        possiblePos = trie->travelWith(t.substr(i - k, k))->positions;
        for (int j = 0; j < possiblePos.size(); ++j)
        {
            if (i - k - possiblePos[j] + m > t.length())
                continue;
            std::cout << t.substr(i - k - possiblePos[j], m);
            if (isDeltaMatch(p, t.substr(i - k - possiblePos[j], m), delta))
            {
                std::cout<< " Ok " << '\n';
                answ += std::to_string(i - k - possiblePos[j]) + " ";
            } else {
                std::cout<< " X " << '\n';
            }
        }
        i += m - k + 1;
    }
    if (answ != "")
        return answ;
    return "-1";
}

int main()
{
    std::string y, x;
    int delta;
    std::cin >> y >> x;
    std::cin >> delta;
    std::cout << deltaBM1SkipSearch(y, x, delta) << std::endl;
}
*/