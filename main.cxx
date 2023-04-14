#include "BitwiseAlgos.h"
#include "DeltaBMAlgos.h"
#include "MatchingAlgos.h"
#include "OccurrenceAlgos.h"
#include "StrMatchConfig.h"
#include <chrono>
#include <iomanip>

int main(int argc, char *argv[]) {
  char algoType;
  int inputValue;
  std::string alphPath;
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale(""));
  std::cin.imbue(std::locale(""));
  if (argc < 3) {
    // Version
    std::wcout << argv[0] << " Version " << StrMatching_VERSION_MAJOR << "."
               << StrMatching_VERSION_MINOR << std::endl;
    std::wcout << "Usage: " << argv[0] << " number" << std::endl;
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

  MatchingAlgos matchAlgos = MatchingAlgos(alphPath);
  std::wstring y, x, pathText;
  int delta, gamma, inputT, numAlpBounds[3];
  bool numCase = false;
  std::vector<int> matchIndexes;
  std::wcout << "Enter your text by: (0) Keyboard - (1) PathFile - (2) Numbers "
                "PathFile: "
             << std::endl;
  std::wcin >> inputT;
  switch (inputT) {
  case 0:
    std::wcin >> y;
    break;
  case 1:
    std::wcin >> pathText;
    y = matchAlgos.getText(std::string(pathText.begin(), pathText.end()));
    break;
  case 2:
    numCase = true;
    std::wcin >> pathText;
    std::wcout << "Enter the bounds of your Alphabet: (Begin) (End)"
               << std::endl;
    std::wcin >> numAlpBounds[0] >> numAlpBounds[1];
    y = matchAlgos.getText(std::string(pathText.begin(), pathText.end()), true);
    break;
  default:
    std::wcout << "Bad init parameter! :(" << std::endl;
    return 1;
    break;
  }
  std::wcout << "Enter your pattern: " << std::endl;
  std::wstring num;
  if (numCase) {
    std::wcin >> pathText;
    std::wstringstream ss(pathText);
    long n;
    while (ss >> n) {
      x += static_cast<wchar_t>(n);
      if (ss.peek() == L',')
        ss.ignore();
    }
    // std::wcout << y << std::endl;
    // std::wcout << x << std::endl;
  } else
    std::wcin >> x;

  // Unsync the I/O of C and C++.
  std::ios_base::sync_with_stdio(false);
  double time_taken;

  switch (algoType) {
  case 'B':
  case 'b': {
    BitwiseAlgos bitAlgos;
    if (numCase)
      bitAlgos = BitwiseAlgos(numAlpBounds[0], numAlpBounds[1]);
    else
      bitAlgos = BitwiseAlgos(alphPath);

    switch (inputValue) {
    case 0: {
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = bitAlgos.shiftOr(y, x);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Exact Matching results with Shift Or: " << std::endl;
      break;
    }
    case 1: {
      std::wcout << "Approximate Delta Matching. Insert Delta: " << std::endl;
      std::wcin >> delta;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = bitAlgos.shiftAnd(y, x, delta);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Shift And: " << std::endl;
      break;
    }
    case 2: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = bitAlgos.shiftPlus(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Shift Plus: " << std::endl;
      break;
    }
    case 3: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = bitAlgos.forwardScan(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Forward Scan: " << std::endl;
      break;
    }
    case 4: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = bitAlgos.backwardScan(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Backward Scan: " << std::endl;
      break;
    }
    default:
      std::wcout << "Bad init parameter! :(" << std::endl;
      return 1;
    }
    break;
  }
  case 'd':
  case 'D': {
    DeltaBMAlgos deltaAlgos;
    if (numCase)
      deltaAlgos = DeltaBMAlgos(numAlpBounds[0], numAlpBounds[1]);
    else
      deltaAlgos = DeltaBMAlgos(alphPath);

    switch (inputValue) {
    case 0: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaTunedBM(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Tunned Boyer Moore: " << std::endl;
      break;
    }
    case 3: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaBM3(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Boyer Moore 3 (Hash): " << std::endl;
      break;
    }
    case 4: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaFastSearch(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Fast Search: " << std::endl;
      break;
    }
    case 5: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaForwardFastSearch(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Forward Fast Search: " << std::endl;
      break;
    }
    default:
      std::wcout << "Bad init parameter! :(" << std::endl;
      return 1;
    }
    break;
  }
  case 'o':
  case 'O': {
    OccurrenceAlgos occurAlgos;

    if (numCase)
      occurAlgos = OccurrenceAlgos(numAlpBounds[0], numAlpBounds[1]);
    else
      occurAlgos = OccurrenceAlgos(alphPath);

    switch (inputValue) {
    case 0: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = occurAlgos.bruteForce(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Brute Force: " << std::endl;
      break;
    }
    case 1: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = occurAlgos.deltaSkipSearch(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Skip Search: " << std::endl;
      break;
    }
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

  time_taken *= 1e-9;
  std::wcout << "Time taken by program is : " << std::fixed << time_taken
             << std::setprecision(9);
  std::wcout << " sec" << std::endl;
  return 0;
}
