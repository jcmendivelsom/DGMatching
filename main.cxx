#include "BitwiseAlgos.h"
#include "DeltaBMAlgos.h"
#include "MatchingAlgos.h"
#include "OccurrenceAlgos.h"
#include "StrMatchConfig.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

int main(int argc, char *argv[]) {
  char algoType;
  int inputValue;
  std::string alphPath;
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale(""));
  std::wcin.imbue(std::locale(""));
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

  std::wstring yIn, xIn, pathText;
  int delta, gamma, inputT, numAlpBounds[3];
  bool numCase = false;
  std::vector<int> matchIndexes;
  std::wcout << "Enter your text by: (0) Keyboard - (1) PathFile - (2) Numbers "
                "PathFile: "
             << std::endl;
  std::wcin >> inputT;
  switch (inputT) {
  case 0:
    std::wcin >> yIn;
    break;
  case 1:
    std::wcin >> pathText;
    yIn = getText(std::string(pathText.begin(), pathText.end()));
    break;
  case 2:
    numCase = true;
    std::wcin >> pathText;
    std::wcout << "Enter the bounds of your Alphabet: (Begin) (End)"
               << std::endl;
    std::wcin >> numAlpBounds[0] >> numAlpBounds[1];
    yIn = getText(std::string(pathText.begin(), pathText.end()), true);
    std::wcout << yIn.size() << std::endl;
    break;
  case 3: {
    std::wcout << "Enter MUSIC path:" << std::endl;
    std::wcin >> pathText;
    getMIDINumbers(std::string(pathText.begin(), pathText.end()));
    return 0;
  }
  case 4: {
    alphPath = "../MatchingAlgos/Alphabet/abc.txt";
    bool numCase = true;
    int numAlpBoundsL = 0;
    int attempt = 2;
    int numAlpBoundsH = 500;
    yIn = getText("../expr/zipf.txt", numCase);
    std::wcout << yIn.length() << std::endl;
    std::wstring_view yStr(yIn);
    std::vector<std::wstring> patterns =
        getArrText("../expr/patZipf.txt", numCase);
    BitwiseAlgos bitAlgos = numCase ? BitwiseAlgos(numAlpBoundsL, numAlpBoundsH)
                                    : BitwiseAlgos(alphPath);
    DeltaBMAlgos deltaAlgos = numCase
                                  ? DeltaBMAlgos(numAlpBoundsL, numAlpBoundsH)
                                  : DeltaBMAlgos(alphPath);
    OccurrenceAlgos occAlgos =
        numCase ? OccurrenceAlgos(numAlpBoundsL, numAlpBoundsH)
                : OccurrenceAlgos(alphPath);
    int i = 0;
    int delta;
    std::vector<int> vecDeltas = {1,  3,  5,  7,  9,  11, 13, 15, 17, 21,
                                  23, 25, 27, 29, 31, 33, 35, 37, 39};
    int gamma;
    int n_pat = 10;
    std::vector<int> indexes;
    std::ios_base::sync_with_stdio(false);
    std::vector<double> time_taken(15, 0);

    // Print Header
    std::wcout << "d;";
    // std::wcout << "m;";
    // std::wcout << "s;";
    std::wcout << "shAND;";
    std::wcout << "shPLUS;";
    std::wcout << "forSCAN;";
    std::wcout << "bakSCAN;";
    std::wcout << "TUNED;";
    std::wcout << "BM1;";
    std::wcout << "BM2;";
    std::wcout << "BM3;";
    std::wcout << "FASE;";
    std::wcout << "FFSE;";
    std::wcout << "varBM1;";
    std::wcout << "MAXSh;";
    std::wcout << "SKIP;";
    std::wcout << "INTSEA;";
    std::wcout << "BRFO;" << std::endl;

    for (int jj = 0; jj < vecDeltas.size(); ++jj)
      // for (int i = 0; i < patterns.size(); ++i) {
      for (int i = attempt * n_pat; i < (attempt + 1) * n_pat; ++i) {
        ////////////////////
        delta = vecDeltas[jj];
        gamma = delta * patterns[i].length() / 2;
        // std::wcout << "???" << patterns[i].length() << std::endl;
        ////////////////////
        ///**///**//
        auto start = std::chrono::high_resolution_clock::now();
        // indexes = bitAlgos.shiftAnd(yStr, patterns[i], delta);
        auto end = std::chrono::high_resolution_clock::now();
        time_taken[0] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout<<"____--___--___--___"<<std::endl;
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = bitAlgos.shiftPlus(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[1] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout<<"____--___--___--___"<<std::endl;
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = bitAlgos.forwardScan(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[2] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout<<"____--___--___--___"<<std::endl;
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = bitAlgos.backwardScan(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[3] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout<<"____--___--___--___"<<std::endl;
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = deltaAlgos.deltaTunedBM(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[4] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        // if(jj < 17)
        //indexes = deltaAlgos.deltaBM1(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[5] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        // indexes = deltaAlgos.deltaBM2(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[6] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        // if (jj < 10) indexes = deltaAlgos.deltaBM3(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[7] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout<<"____--___--___--___"<<std::endl;
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        // indexes = deltaAlgos.deltaFastSearch(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[8] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        // indexes = deltaAlgos.deltaForwardFastSearch(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[9] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        indexes = deltaAlgos.trieSearch(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[10] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = deltaAlgos.deltaMaximalShift(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[11] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = occAlgos.deltaSkipSearch(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[12] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        //indexes = occAlgos.intervalSearch(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[13] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        ///**///**//
        start = std::chrono::high_resolution_clock::now();
        indexes = occAlgos.bruteForce(yStr, patterns[i], delta, gamma);
        end = std::chrono::high_resolution_clock::now();
        time_taken[14] +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();
        // std::wcout << "____--___--___--___" << std::endl;
        /***************************************************/
        if (i % n_pat == n_pat - 1) {
          // std::wcout << patterns[i].length() << ";";
          // std::wcout <<numAlpBoundsH - numAlpBoundsL << ";";
          std::wcout << vecDeltas[jj] << ";";
          for (int k = 0; k < time_taken.size(); ++k) {
            std::wcout << std::fixed << time_taken[k] / n_pat * 1e-9
                       << std::setprecision(9) << ";";
          }
          std::wcout << std::endl;

          time_taken = std::vector<double>(15, 0);
        }
      }
    return 0;
  }
  default:
    std::wcout << "Bad init parameter! :(" << std::endl;
    return 1;
  }
  std::wcout << "Enter your pattern: " << std::endl;
  std::wstring num;
  if (numCase) {
    std::wcin >> pathText;
    std::wstringstream ss(pathText);
    long n;
    while (ss >> n) {
      xIn += static_cast<wchar_t>(n);
      if (ss.peek() == L',')
        ss.ignore();
    }
    // std::wcout << y << std::endl;
    // std::wcout << x << std::endl;
  } else
    std::wcin >> xIn;

  std::wstring_view y(yIn);
  std::wstring_view x(xIn);

  // Unsync the I/O of C and C++.
  std::ios_base::sync_with_stdio(false);
  double time_taken;

  switch (algoType) {
  case 'B':
  case 'b': {
    BitwiseAlgos bitAlgos = numCase
                                ? BitwiseAlgos(numAlpBounds[0], numAlpBounds[1])
                                : BitwiseAlgos(alphPath);
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
    DeltaBMAlgos deltaAlgos =
        numCase ? DeltaBMAlgos(numAlpBounds[0], numAlpBounds[1])
                : DeltaBMAlgos(alphPath);
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
    case 1: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaBM1(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Boyer Moore 1 (Trie): " << std::endl;
      break;
    }
    case 2: {
      std::wcout << "Approximate Delta Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaBM2(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Boyer Moore 2 (Suffix Automaton): "
                 << std::endl;
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
    case 6: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.trieSearch(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Trie Search: " << std::endl;
      break;
    }
    case 7: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = deltaAlgos.deltaMaximalShift(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Delta Maximal Shift: " << std::endl;
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
    OccurrenceAlgos occurAlgos =
        numCase ? OccurrenceAlgos(numAlpBounds[0], numAlpBounds[1])
                : OccurrenceAlgos(alphPath);
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
    case 2: {
      std::wcout << "Approximate Delta-Gamma Matching. Insert Delta and Gamma: "
                 << std::endl;
      std::wcin >> delta >> gamma;
      auto start = std::chrono::high_resolution_clock::now();
      matchIndexes = occurAlgos.intervalSearch(y, x, delta, gamma);
      // Calculating total time taken by the program.
      auto end = std::chrono::high_resolution_clock::now();
      time_taken =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
      std::wcout << "Results with Interval Search: " << std::endl;
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
