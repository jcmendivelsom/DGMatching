#include "Alphabet.h"

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
  long characterKey;
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
    // Save in the map the values.
    alphToValues[characterKey] = {value, index};
    indexToAlph[index] = characterKey;
    valueToIndices[value].push_back(index);
    if (value < minVALUE)
      minVALUE = value;
    if (maxVALUE < value)
      maxVALUE = value;
    ++index;
  }
  paramFileW.close();
}
// Return the VALUE of certain character. If char. 'c' is not in the alphabet
// return INT32_MIN.
int Alphabet::getValue(wchar_t c) {
  try {
    return alphToValues.at((long)c).first;
  } catch (const std::exception &e) {
    std::wcout << "The following character is not in the alphabet: " << c
               << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}
int Alphabet::getValueByI(int i) {
  try {
    return alphToValues.at(indexToAlph.at(i)).first;
  } catch (const std::exception &e) {
    std::wcout << "The following index is not in the alphabet: " << i
               << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}
// Return the INDEX of certain character. If char. 'c' is not in the alphabet
// return -1.
int Alphabet::getIndex(wchar_t c) {
  try {
    return alphToValues.at((long)c).second;
  } catch (const std::exception &e) {
    std::wcout << "The following character is not in the alphabet: " << c
               << std::endl;
    throw std::invalid_argument("Invalid character! ");
  }
}
// Return the WCHAR of certain index. If index. 'i' is not in the alphabet
// return ''.
wchar_t Alphabet::getWChar(int i) {
  try {
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
int Alphabet::size() { return alphToValues.size(); }
// Print the whole alphabet.
void Alphabet::print() {
  for (const auto &elem : alphToValues) {
    std::wcout << (wchar_t)elem.first << " " << elem.second.first << " "
               << elem.second.second << "\n";
  }
}
// Getters of the Min and Max value.
int Alphabet::getMaxValue() { return maxVALUE; }
int Alphabet::getMinValue() { return minVALUE; }