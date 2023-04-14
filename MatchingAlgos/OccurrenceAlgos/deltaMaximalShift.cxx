#include "OccurrenceAlgos.h"

typedef struct patternScanOrder {
  int loc;
  char c;
} pattern;

bool isDeltaMatch(std::string x, std::string y, unsigned int delta) {
  if (x.length() != y.length())
    return false;
  for (int i = 0; i < x.length(); ++i)
    if (std::abs(x[i] - y[i]) > delta)
      return false;
  return true;
}
int computeMinShift(std::string p, int pos, unsigned int delta) {
  int j, m = p.length();
  int minShift[m];
  for (int i = 0; i < m; ++i) {
    for (j = i - 1; j >= 0; --j)
      if (std::abs(p[i] - p[j]) <= delta)
        break;
    minShift[i] = i - j;
  }
  return minShift[1]; // Esta mal esto
}
/* Maximal Shift pattern comparison function. */
int maxShiftPcmp(pattern *pat1, pattern *pat2, int *minShift) {
  int dsh;

  dsh = minShift[pat2->loc] - minShift[pat1->loc];
  return (dsh ? dsh : (pat2->loc - pat1->loc));
}
/* Construct an ordered pattern from a string. */
void orderPattern(char *x, int m, int (*pcmp)(), pattern *pat) {
  int i;

  for (i = 0; i <= m; ++i) {
    pat[i].loc = i;
    pat[i].c = x[i];
  }
  qsort(pat, m, sizeof(pattern), (int (*)(const void *, const void *))pcmp);
}
/* Find the next leftward matching shift for
   the first ploc pattern elements after a
   current shift or lshift. */
int matchShift(char *x, int m, int ploc, int lshift, pattern *pat) {
  int i, j;

  for (; lshift < m; ++lshift) {
    i = ploc;
    while (--i >= 0) {
      if ((j = (pat[i].loc - lshift)) < 0)
        continue;
      if (pat[i].c != x[j])
        break;
    }
    if (i < 0)
      break;
  }
  return (lshift);
}

/* Constructs the good-suffix shift table
   from an ordered string. */
void preAdaptedGs(char *x, int m, int adaptedGs[], pattern *pat) {
  int lshift, i, ploc;

  adaptedGs[0] = lshift = 1;
  for (ploc = 1; ploc <= m; ++ploc) {
    lshift = matchShift(x, m, ploc, lshift, pat);
    adaptedGs[ploc] = lshift;
  }
  for (ploc = 0; ploc <= m; ++ploc) {
    lshift = adaptedGs[ploc];
    while (lshift < m) {
      i = pat[ploc].loc - lshift;
      if (i < 0 || pat[ploc].c != x[i])
        break;
      ++lshift;
      lshift = matchShift(x, m, ploc, lshift, pat);
    }
    adaptedGs[ploc] = lshift;
  }
}

std::string deltaMaximalShift(std::string t, std::string p,
                              unsigned int delta) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n) {
    std::cout << "Invalid parameters! " << std::endl;
    return "-1";
  }
  std::string answ = "";
  // Preprocessing
  if (answ != "")
    return answ;
  return "-1";
}

int main() {
  std::string y, x;
  unsigned int delta;
  std::cin >> y >> x;
  std::cin >> delta;
  std::cout << deltaMaximalShift(y, x, delta) << std::endl;
  return 0;
}