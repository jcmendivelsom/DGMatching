#include "BitwiseAlgos.h"
#include "DeltaBMAlgos.h"
#include "OccurrenceAlgos.h"
#include "StrMatchConfig.h"
#include <iostream>

int main(int argc, char *argv[]) {
  char algoType;
  int inputValue;
  std::string alphPath;
  if (argc < 3) {
    // Version
    std::cout << argv[0] << " Version " << StrMatching_VERSION_MAJOR << "."
              << StrMatching_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  } else if (argc < 4) {
    algoType = argv[1][0];
    inputValue = std::stoi(argv[2]);
    alphPath = "";
  } else {
    algoType = argv[1][0];
    inputValue = std::stoi(argv[2]);
    alphPath = argv[3];
  }

  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());
  std::cin.imbue(std::locale());

  std::wstring y, x;
  int delta, gamma;
  std::vector<int> matchIndexes;
  std::wcin >> y;
  std::wcin >> x;

  switch (algoType) {
  case 'B':
  case 'b': {
    BitwiseAlgos bitAlgos = BitwiseAlgos(alphPath);

    switch (inputValue) {
    case 0:
      matchIndexes = bitAlgos.shiftOr(y, x);
      std::wcout << "Exact Matching results with Shift Or: " << std::endl;
      break;
    case 1:
      std::wcout << "Approximate Delta Matching. Insert Delta: " << std::endl;
      std::wcin >> delta;
      matchIndexes = bitAlgos.shiftAnd(y, x, delta);
      std::wcout << "Results with Shift And: " << std::endl;
      break;
    case 2:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = bitAlgos.shiftPlus(y, x, delta, gamma);
      std::wcout << "Results with Shift Plus: " << std::endl;
      break;
    case 3:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = bitAlgos.forwardScan(y, x, delta, gamma);
      std::wcout << "Results with Forward Scan: " << std::endl;
      break;
    case 4:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = bitAlgos.backwardScan(y, x, delta, gamma);
      std::wcout << "Results with Backward Scan: " << std::endl;
      break;
    default:
      std::wcout << "Bad init parameter! :(" << std::endl;
      return 1;
    }
    break;
  }
  case 'd':
  case 'D': {
    DeltaBMAlgos deltaAlgos = DeltaBMAlgos(alphPath);

    switch (inputValue) {
    case 0:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = deltaAlgos.deltaTunedBM(y, x, delta, gamma);
      std::wcout << "Results with Delta Tunned Boyer Moore: " << std::endl;
      break;
    case 3:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = deltaAlgos.deltaBM3(y, x, delta, gamma);
      std::wcout << "Results with Delta Boyer Moore 3 (Hash): " << std::endl;
      break;
    case 4:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = deltaAlgos.deltaFastSearch(y, x, delta, gamma);
      std::wcout << "Results with Delta Fast Search: " << std::endl;
      break;
    case 5:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = deltaAlgos.deltaForwardFastSearch(y, x, delta, gamma);
      std::wcout << "Results with Delta Forward Fast Search: " << std::endl;
      break;
    default:
      std::wcout << "Bad init parameter! :(" << std::endl;
      return 1;
    }
    break;
  }
  case 'o':
  case 'O': {
    OccurrenceAlgos occurAlgos = OccurrenceAlgos(alphPath);
    switch (inputValue) {
    case 0:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = occurAlgos.bruteForce(y, x, delta, gamma);
      std::wcout << "Results with Brute Force: " << std::endl;
      break;
    case 1:
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      matchIndexes = occurAlgos.deltaSkipSearch(y, x, delta, gamma);
      std::wcout << "Results with Delta Skip Search: " << std::endl;
      break;
    default:
      std::wcout << "Bad init parameter! :(" << std::endl;
      return 1;
    }
    break;
  }
  default:
    std::wcout << "Bad init parameter! :(" << std::endl;
    return 1;
  }
  for (auto const &i : matchIndexes)
    std::wcout << i << " ";
  std::wcout << std::endl;
  return 0;
}
