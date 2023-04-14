#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

// PATH TO DEFAULT ALPHABET
#define PATH_DEFAULT_ALPH "../MatchingAlgos/Alphabet/hexAlph.txt"
class Alphabet {
  // Map that stores the values and an index of every character.
  // Index play the role as a enumeration value.
  // CHARACTER - (VALUE, INDEX)
  std::unordered_map<long, std::pair<int, int>> alphToValues;
  std::unordered_map<int, long> indexToAlph;
  std::unordered_map<int, std::vector<int>> valueToIndices;
  int maxVALUE;
  int minVALUE;

public:
  Alphabet(std::string filePath = PATH_DEFAULT_ALPH);
  Alphabet(int begin, int end, int step = 1);
  int getValue(wchar_t c);
  int getValueByI(int i);
  int getIndex(wchar_t c);
  wchar_t getWChar(int i);
  std::vector<int> getIndicesByVal(int val);
  int size();
  void print();
  int getMaxValue();
  int getMinValue();
};