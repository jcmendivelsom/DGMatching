#include "MatchingAlgos.h"

/*
    -> sumDeltaMatch return the sum of the value differences char. by char. But
   if find a Delta mismatch returns -1.
*/
int MatchingAlgos::sumDeltaMatch(std::wstring_view x, std::wstring_view y,
                                 int delta) {
  int sum = 0;
  if (x.length() != y.length() || delta < 0)
    return -1;
  for (int i = 0; i < x.length(); ++i) {
    if (std::abs(alph.getValue(x[i]) - alph.getValue(y[i])) > delta)
      return -1;
    sum += std::abs(alph.getValue(x[i]) - alph.getValue(y[i]));
  }
  return sum;
}

/*
    CONSTRUCTOR of the class MatchingAlgos just initialize the alphabet.
*/
MatchingAlgos::MatchingAlgos(std::string alphPath) {
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale(""));
  std::cin.imbue(std::locale(""));
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
  // alph.print();
}
MatchingAlgos::MatchingAlgos(int begin, int end, int step) {
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale(""));
  std::cin.imbue(std::locale(""));
  alph = Alphabet(begin, end, step);
  // alph.print();
}
/*
     -> isDeltaGammaMatch return whether 'x' Delta match 'y' or not. If Gamma is
   negative just check the Delta match.
*/
bool MatchingAlgos::isDeltaGammaMatch(std::wstring_view x, std::wstring_view y,
                                      int delta, int gamma) {
  int aux = sumDeltaMatch(x, y, delta);
  return 0 <= aux && (aux <= gamma || gamma < 0);
}

///////////////////////////////////////////////////////////

std::wstring getText(std::string filePath, bool isNumber) {
  /* If your native locale doesn't use UTF-8 encoding
   * you need to replace the empty string with a
   * locale like "en_US.utf8"
   */
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());
  // Open the file (encoded with UTF-8) and read the values.
  std::wifstream paramFileW(filePath, std::ios::binary);
  paramFileW.imbue(std::locale("en_US.UTF-8"));
  if (paramFileW.fail()) {
    throw std::invalid_argument("Can't open the specified file path: " +
                                filePath);
  }
  std::wstring line, num, answ;
  long n;
  std::wstringstream ss;
  while (paramFileW.good()) {
    std::getline(paramFileW, line);
    ss = std::wstringstream(line);
    if (isNumber) {
      while (ss >> n) {
        answ += static_cast<wchar_t>(n);
        if (ss.peek() == L',')
          ss.ignore();
      }
      /*
       while (std::getline(ss, num, L','))
         answ += (wchar_t)std::stol(num);
       */
    } else {
      answ += line;
    }
  }
  paramFileW.close();
  return answ;
}