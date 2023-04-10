#pragma once
#include <iostream>
#include <unordered_map>
// PATH TO DEFAULT ALPHABET
#define PATH_DEFAULT_ALPH "../Alphabet/hexAlph.txt"
class Alphabet {
  // Map that stores the values and an index of every character.
  // Index play the role as a enumeration value.
  std::unordered_map<long, std::pair<int, int>> alphToValues;
  std::unordered_map<int, long> indexToAlph;

public:
  Alphabet(std::string filePath = PATH_DEFAULT_ALPH);
  int getValue(wchar_t c);
  int getValueByI(int i);
  int getIndex(wchar_t c);
  wchar_t getWChar(int i);
  int size();
  void print();
};