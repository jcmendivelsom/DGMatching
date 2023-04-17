#include "DeltaBMAlgos.h"

/*
    -> hash compute the hash of a string 'x', i.e. the sum of the value of their
   characters.
*/
long DeltaBMAlgos::hash(std::wstring_view x) {
  long sum = 0;
  for (int i = 0; i < x.length(); i++) {
    sum += alph.getValue(x[i]);
  }
  return sum;
}

/*
    -> computeHashTableIx return the the Delta hash table - hash(x) : possible
   positions for factors of 'x' of length k.
*/
std::unordered_map<long int, std::vector<int>>
DeltaBMAlgos::computeHashTableIx(std::wstring_view x, int k, int delta) {
  std::unordered_map<long int, std::vector<int>> deltaHashTable;
  long begin, end;
  std::wstring_view xFactorK;
  // Iterate over all factors of length k
  for (int pos = 0; pos <= x.length() - k; ++pos) {
    xFactorK = x.substr(pos, k);
    // Limit the possible factors by the max and min values of the hash.
    begin = hash(xFactorK) - k * delta;
    if (begin < k * alph.getMinValue())
      begin = k * alph.getMinValue();
    end = hash(xFactorK) + k * delta;
    if (end > k * alph.getMaxValue())
      end = k * alph.getMaxValue();
    for (long i = begin; i <= end; ++i) {
      // If i isn't present: Add this hash value with an array with position
      // Otherwise just add to the end the position
      deltaHashTable[i].push_back(pos);
    }
  }
  return deltaHashTable;
}

/*
    -> DeltaBM3 using a hash function computes all the Delta-Gamma matches of a
   pattern 'x' in a text 'y' by checking if in the window a substring of length
   k have the same hash of any 'k-factor of 'x'.
*/
std::vector<int> DeltaBMAlgos::deltaBM3(std::wstring_view t, std::wstring_view p,
                                        int delta, int gamma, int k) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || k > m || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Compute the hash table for all the k-factors.
  std::unordered_map<long int, std::vector<int>> deltaHashTable =
      computeHashTableIx(p, k, delta);
  /*
  // Print the Delta Hash Table
    for (auto elem : deltaHashTable) {
    std::cout << elem.first << " ->";
    for (auto arr : elem.second)
        std::cout << " " << arr << ",";
    std::cout << std::endl;
    }
  */

  // SEARCHING PHASE
  int j, i = m;
  long h;
  while (i < t.length()) {
    h = hash(t.substr(i - k, k));
    // If the hash is not in the Hash Table just continue (Shift by m - k + 1).
    if (deltaHashTable.count(h) == 0) {
      i += m - k + 1;
      continue;
    }
    // Iterate over all possible positions of the hash h.
    for (int j = 0; j < deltaHashTable[h].size(); ++j) {
      if (i - k - deltaHashTable[h][j] + m > t.length())
        continue;
      if (isDeltaGammaMatch(p, t.substr(i - k - deltaHashTable[h][j], m), delta,
                            gamma)) {
        answ.push_back(i - k - deltaHashTable[h][j]);
      }
    }
    i += m - k + 1;
  }
  if (answ.empty())
    return {-1};
  return answ;
}