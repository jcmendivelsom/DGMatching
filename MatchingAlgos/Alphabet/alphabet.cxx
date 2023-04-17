#include "Alphabet.h"

Alphabet::Alphabet(int begin, int end, int step) {
  int index = 0, num;
  if (begin < 0 || end < begin)
    throw std::invalid_argument(
        "Invalid alphabet, begin must be non-negative and end >= begin.");
#if !USE_UNORDERED_MAP
  alphToValues = std::vector<int>(end + 1, INT32_MIN);
  alphToIndex = std::vector<int>(end + 1, -1);
  indexToAlph = std::vector<long>((end - begin) / step, INT32_MIN);
#endif
  for (num = begin; num <= end; num += step) {
    // Save in the map the values.
    alphToValues[(long)num] = num;
    alphToIndex[(long)num] = index;
    indexToAlph[index] = (long)num;
    valueToIndices[num].push_back(index);
    index++;
  }
  minVALUE = begin;
  maxVALUE = num;
}

Alphabet::Alphabet(std::string filePath) {
  /* If your native locale doesn't use UTF-8 encoding
   * you need to replace the empty string with a
   * locale like "en_US.utf8"
   */
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());
  // Open the file (encoded with UTF-8) and read the values.
  std::wifstream paramFileW(filePath);
  if (paramFileW.fail()) {
    throw std::invalid_argument("Can't open the specified file path: " +
                                filePath);
  }
  std::wstring line;
  long characterKey, maxCHAR = 0;
  int value, index = 0;
  maxVALUE = INT32_MIN;
  minVALUE = INT32_MAX;
  // Read the while there is a line: 'UTF-8 Character' 'Integer'
  while (paramFileW.good()) {
    std::getline(paramFileW, line);
    if (line.length() < 3) {
      paramFileW.close();
      throw std::invalid_argument(
          "Invalid alphabet file format. One of the lines is too short.");
    }
    characterKey = (long)line[0]; // First char is always the alph. char
    // The value of the character must be separated by one character and must
    // be an integer.
    try {
      value = std::stoi(line.substr(2));
    } catch (const std::exception &e) {
      paramFileW.close();
      throw std::invalid_argument("Invalid alphabet file format. Can't "
                                  "convert to int certain wchar.");
    }
#if USE_UNORDERED_MAP
    // Save in the map the values.
    alphToValues[characterKey] = value;
    alphToIndex[characterKey] = index;
    indexToAlph[index] = characterKey;
#endif
    valueToIndices[value].push_back(index);
    if (value < minVALUE)
      minVALUE = value;
    if (maxVALUE < value)
      maxVALUE = value;
    if (maxCHAR < characterKey)
      maxCHAR = characterKey;
    ++index;
  }
#if !USE_UNORDERED_MAP
  // If we are using only vectors, we have to read one more time because the
  // first one just get the max values to init vectors.
  alphToValues = std::vector<int>(maxCHAR + 1, INT32_MIN);
  alphToIndex = std::vector<int>(maxCHAR + 1, -1);
  indexToAlph = std::vector<long>(index, INT32_MIN);

  paramFileW.clear();
  paramFileW.seekg(0, std::ios::beg);
  index = 0;

  while (paramFileW.good()) {
    std::getline(paramFileW, line);
    characterKey = (long)line[0]; // First char is always the alph. char
    // The value of the character must be separated by one character and must
    // be an integer.
    value = std::stoi(line.substr(2));
    try {
      alphToValues.at(characterKey) = value;
      alphToIndex.at(characterKey) = index;
      indexToAlph.at(index) = characterKey;
    } catch (const std::out_of_range &oor) {
      std::wcout << characterKey << " ALPH OUT OF BOUNDS " << value
                 << std::endl;
    }
    ++index;
  }
#endif

  paramFileW.close();
}

// Return the VALUE of certain character. If char. 'c' is not in the alphabet
// return INT32_MIN.
int Alphabet::getValue(wchar_t c) {
  try {
    if (alphToValues.at((long)c) == INT32_MIN)
      throw std::invalid_argument("");
    return alphToValues.at((long)c);
  } catch (const std::exception &e) {
    std::wcout << "The following character is not in the alphabet: " << c
               << " - " << (long)c << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}
int Alphabet::getValueByI(int i) {
  try {
    if (indexToAlph.at(i) == INT32_MIN ||
        alphToValues.at(indexToAlph.at(i)) == INT32_MIN)
      throw std::invalid_argument("");
    return alphToValues.at(indexToAlph.at(i));
  } catch (const std::exception &e) {
    std::wcout << "The following index (*) is not in the alphabet: " << i
               << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}

// Return the INDEX of certain character. If char. 'c' is not in the alphabet
// return -1.
int Alphabet::getIndex(wchar_t c) {
  try {
    if (alphToIndex.at((long)c) < 0)
      throw std::invalid_argument("");
    return alphToIndex.at((long)c);
  } catch (const std::exception &e) {
    std::wcout << "The following character is not in the alphabet: " << c
               << " - " << (long)c << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}

// Return the WCHAR of certain index. If index. 'i' is not in the alphabet
// return ''.
wchar_t Alphabet::getWChar(int i) {
  try {
    if (indexToAlph.at(i) == INT32_MIN)
      throw std::invalid_argument("");
    return (wchar_t)indexToAlph.at(i);
  } catch (const std::exception &e) {
    std::wcout << "The following index is not in the alphabet: " << i
               << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}

// Get all the char indices that have that have 'val' as value.
std::vector<int> Alphabet::getIndicesByVal(int val) {
  try {
    return valueToIndices.at(val);
  } catch (const std::exception &e) {
    return {};
  }
}

// Size of the alphabet (number of characters.)
int Alphabet::size() { return indexToAlph.size(); }

// Print the whole alphabet.
void Alphabet::print() {
#if USE_UNORDERED_MAP
  for (const auto &elem : alphToValues) {
    std::wcout << getIndex((wchar_t)elem.first) << " " << (wchar_t)elem.first
               << " " << elem.second << "\n";
  }
#else
  for (int i = 0; i < indexToAlph.size(); i++) {
    std::wcout << i << " " << (wchar_t)indexToAlph[i] << " " << getValueByI(i)
               << "\n";
  }
#endif
}

// Getters of the Min and Max value.
int Alphabet::getMaxValue() { return maxVALUE; }
int Alphabet::getMinValue() { return minVALUE; }