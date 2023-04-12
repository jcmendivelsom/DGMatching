#pragma once
#include "Alphabet.h"
#include <iostream>
#include <vector>
class MatchingAlgos {
protected:
  Alphabet alph;

public:
  MatchingAlgos(std::string filePath = "");
};