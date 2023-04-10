#include <iostream>
#include <vector>
/*
    -> bruteForceDGm computes all the Delta-Gamma matches of a
pattern 'x' in a text input 'y' by checking every index in the text (brute
force).
*/
std::vector<int> bruteForceDGm(std::string y, std::string x, int delta,
                               int gamma) {
  if (x.length() > y.length()) {
    std::cout << "Pattern is longer than input text." << std::endl;
    return {-1};
  }
  std::vector<int> answ; // Vector to save the matching indexes answers
  int sum, j;
  for (int i = 0; i <= y.length() - x.length(); ++i) {
    j = 0;
    sum = 0;
    // Check if from index 'i' there is a Delta match
    while (j < x.length() && std::abs(x[j] - y[i + j]) <= delta) {
      sum += std::abs(x[j] - y[i + j]);
      j++;
    }
    // Also check if is a Gamma match
    if (sum <= gamma && j == x.length())
      answ.push_back(i);
  }
  if (answ.empty())
    return {-1};
  return answ;
}

int main() {
  std::string y, x;
  int delta, gamma;
  std::cin >> y >> x;
  std::cin >> delta >> gamma;
  std::vector<int> matchIndexes = bruteForceDGm(y, x, delta, gamma);
  for (auto const &i : matchIndexes)
    std::cout << i << " ";
  std::cout << std::endl;
  return 0;
}