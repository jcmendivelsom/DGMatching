#include "DeltaBMAlgos.h"
#include <chrono>
#include <iomanip>

////////////////// ALGORITHM ////////////////////////

std::vector<int> DeltaBMAlgos::deltaBM2(std::wstring_view t,
                                        std::wstring_view p, int delta,
                                        int gamma) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }
  if (gamma < 0)
    gamma = delta * m;
  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Make reverse of pattern
  std::wstring rev;
  for (int i = m - 1; i >= 0; --i)
    rev += p[i];
  std::wstring_view pReverse(rev);
  auto start = std::chrono::high_resolution_clock::now();
  // Compute the Delta Factor Trie for all the k-factors.
  DeltaSuffixAutomaton dsa = DeltaSuffixAutomaton(alph, pReverse, delta);
  // Print the entire Delta Factor Trie
  // dsa.printAutomaton();
  auto end = std::chrono::high_resolution_clock::now();
  double time_taken =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  time_taken *= 1e-9;
  std::wcout << "Time building the automaton : " << std::fixed << time_taken
             << std::setprecision(9);
  std::wcout << " sec" << std::endl;

  // SEARCHING PHASE
  start = std::chrono::high_resolution_clock::now();
  int period = m, sum;
  int j = 0, i, state, shift;
  while (j <= n - m) {
    i = m - 1;
    state = 0;
    shift = m;
    sum = 0;
    while (i + j >= 0 && dsa.travelWith(state, alph.getValue(t[i + j])) != -1) {
      state = dsa.travelWith(state, alph.getValue(t[i + j]));
      sum += std::abs(alph.getValue(t[i + j]) - alph.getValue(p[i]));
      if (dsa.isTerminal(state) && state != 0) {
        period = shift;
        shift = i;
      }
      --i;
    }
    if (i < 0) {
      if (sum <= gamma)
        answ.push_back(j);
      shift = period;
    }
    j += shift;
  }
  end = std::chrono::high_resolution_clock::now();
  time_taken =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  time_taken *= 1e-9;
  std::wcout << "Time searching : " << std::fixed << time_taken
             << std::setprecision(9);
  std::wcout << " sec" << std::endl;

  if (answ.empty())
    return {-1};
  return answ;
}

////////////////// INTERVAL STATE /////////////////////

// A single state in our DFA, which represents an equivalence class.
void DeltaBMAlgos::IntervalState::addTransition(Interval in, int s) {
  boundVals.first = std::min(boundVals.first, in.first);
  boundVals.second = std::max(boundVals.second, in.second);
  intervalTransitions[in] = s;
}
// Returns the index of a state or -1 if no transition exists for c
std::vector<int> DeltaBMAlgos::IntervalState::intervalTravelWith(int c) {
  std::vector<int> states;
  for (auto const &t : intervalTransitions) {
    if (t.first.first <= c && c <= t.first.second) {
      states.push_back(t.second);
    }
  }
  return states;
}

// Returns the index of a state or -1 if no transition exists for c
int DeltaBMAlgos::IntervalState::travel(int c) {
  try {
    return travelTransitions.at(c);
  } catch (const std::exception &e) {
    return -1;
  }
}

DeltaBMAlgos::IntervalState::IntervalState() {
  this->len = 0;
  this->link = -1;
  this->terminal = false;
  //this->intervalTransitions = std::unordered_map<Interval, int>();
  // std::unordered_map<Interval, int> transitions;
  // std::vector<int> suffixReferences;
  this->boundVals = Interval(INT32_MAX, INT32_MIN);
}

////////////////// DELTA SUFFIX AUTOMATON /////////////////////

// Returns the state at index i
DeltaBMAlgos::IntervalState
DeltaBMAlgos::DeltaSuffixAutomaton::getState(int i) {
  return states.at(i);
}

void DeltaBMAlgos::DeltaSuffixAutomaton::dSuffixAExtend(Interval in) {
  int cur = size++;
  states.push_back(IntervalState());
  states[cur].len = states[last].len + 1;
  int p = last;
  while (p != -1 && !states[p].intervalTransitions.count(in)) {
    states[p].addTransition(in, cur);
    p = states[p].link;
  }
  if (p == -1) {
    states[cur].link = 0;
  } else {
    int q = states[p].intervalTransitions[in];
    if (states[p].len + 1 == states[q].len) {
      states[cur].link = q;
    } else {
      int clone = size++;
      states.push_back(IntervalState());
      states[clone].len = states[p].len + 1;
      states[clone].intervalTransitions = states[q].intervalTransitions;
      states[clone].boundVals = states[q].boundVals;
      states[clone].link = states[q].link;
      while (p != -1 && states[p].intervalTransitions[in] == q) {
        states[p].addTransition(in, clone);
        p = states[p].link;
      }
      states[q].link = states[cur].link = clone;
    }
  }
  last = cur;
}
DeltaBMAlgos::DeltaSuffixAutomaton::DeltaSuffixAutomaton(Alphabet alpha,
                                                         std::wstring_view s,
                                                         int delta) {
  // Initial state t0 will be initialized as last
  this->delta = delta;
  IntervalState l;
  l.len = 0;
  l.link = -1;
  states.push_back(l);
  this->last = 0;
  this->size = 1;

  Interval in;
  for (auto const &c : s) {
    in = Interval(std::max(alpha.getMinValue(), alpha.getValue(c) - delta),
                  std::min(alpha.getMaxValue(), alpha.getValue(c) + delta));
    dSuffixAExtend(in);
  }
  // We now want to mark every terminal state. We start with last, as
  // it is obviously a terminal state. By climbing the suffix links, we
  // find the state that corresponds to the next largest suffix that
  // is of a different equivalence class. This will be a terminal state
  // as well. So on and so forth until we hit the root of the link tree.
  states[last].terminal = true;
  int link = states[last].link;
  while (link != -1) {
    int linked = link;
    states[linked].terminal = true;
    link = states[linked].link;
  }
  makeDeterministic();
  fillTravelTransitions();
}

// MAKE IT DETERMINISTIC FINITE AUTOMATON
void DeltaBMAlgos::DeltaSuffixAutomaton::makeDeterministic() {
  std::map<std::set<int>, int> dfaStateMap;
  std::queue<std::set<int>> dfaStateQueue;
  for (int i = 0; i < states.size(); ++i)
    dfaStateMap[{i}] = i;
  dfaStateQueue.push({0});
  while (!dfaStateQueue.empty()) {
    std::set<int> currentNfaStates = dfaStateQueue.front();
    int indexCurrState = dfaStateMap[currentNfaStates];
    dfaStateQueue.pop();
    IntervalState current = states[indexCurrState];
    // std::cout << "Entro" << indexCurrState << "\n";
    bool isTerminal = false;
    // Procesar cada posible entrada del alfabeto
    for (int input = current.boundVals.first; input <= current.boundVals.second;
         ++input) {
      // std::cout << "Estoy en for" << (char) input << "\n";
      std::set<int> nextNfaStates;
      // Encontrar todos los estados NFA accesibles por esta entrada
      for (int nfaState : currentNfaStates) {
        for (int state : states[nfaState].intervalTravelWith(input)) {
          nextNfaStates.insert(state);
          if (states[state].terminal)
            isTerminal = true;
        }
      }
      // Si el conjunto de estados NFA es vacío, no hay transición para esta
      // entrada
      if (nextNfaStates.empty()) {
        // std::cout << "Entro a continue \n";
        continue;
      }
      // Verificar si ya existe un estado DFA correspondiente a este conjunto
      // de estados NFA
      auto sts = dfaStateMap.find(nextNfaStates);
      if (sts == dfaStateMap.end()) {
        // Si no existe, crear un nuevo estado DFA
        int newDfaState = states.size();
        dfaStateMap[nextNfaStates] = newDfaState;
        IntervalState aux;
        aux.len = 0;
        aux.link = -1;
        aux.terminal = isTerminal;
        for (auto const &i : nextNfaStates) {
          aux.boundVals.first =
              std::min(aux.boundVals.first, states[i].boundVals.first);
          aux.boundVals.second =
              std::max(aux.boundVals.second, states[i].boundVals.second);
        }
        states.push_back(aux);
        // states[newDfaState].nfaStates = nextNfaStates;
        // std::cout << " nuevo estado \n";
        dfaStateQueue.push(nextNfaStates);
        sts = dfaStateMap.find(nextNfaStates);
      }
      states[indexCurrState].addTransition(Interval(input, input),
                                           dfaStateMap[nextNfaStates]);
    }
    auto it = states[indexCurrState].intervalTransitions.cbegin();
    while (it != states[indexCurrState].intervalTransitions.cend()) {
      if (it->first.first != it->first.second) {
        // supported in C++11
        it = states[indexCurrState].intervalTransitions.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void DeltaBMAlgos::DeltaSuffixAutomaton::fillTravelTransitions() {
  for (auto &st : states) {
    for (auto &tr : st.intervalTransitions) {
      for (int i = tr.first.first; i <= tr.first.second; ++i) {
        st.travelTransitions[i] = tr.second;
      }
    }
  }
}

void DeltaBMAlgos::DeltaSuffixAutomaton::printAutomaton() {
  std::wstring aux, aux2;
  for (int index = 0; index < states.size(); ++index)
    for (auto const &tr : getState(index).travelTransitions) {
      aux = getState(index).terminal ? L"%" : L" ";
      aux2 = states[tr.second].terminal ? L"%" : L" ";
      std::wcout << "( " << index << " )" << aux << "-> "
                 << "[ " << tr.first << " ] "
                 << "-> "
                 << "( " << tr.second << " )" << aux2 << std::endl;
    }
  /*
for (auto const &st : states)
  for (auto const &tr : st.transitions) {
    aux = st.terminal ? L"%" : L" ";
    aux2 = states[tr.second].terminal ? L"%" : L" ";
    std::wcout << "( " << st.index << " )" << aux << "-> "
               << "[ " << tr.first.first << "," << tr.first.second << " ] "
               << "-> "
               << "( " << tr.second << " )" << aux2 << std::endl;
  }
  */
}
int DeltaBMAlgos::DeltaSuffixAutomaton::travelWith(int indexState, int val) {
  return states[indexState].travel(val);
}

bool DeltaBMAlgos::DeltaSuffixAutomaton::isTerminal(int indexState) {
  return states[indexState].terminal;
}
// O(s) query to see if our source text contains a substring s
/*
// Populate each state with a vector of its children in the link tree
void DeltaBMAlgos::DeltaSuffixAutomaton::computeSuffixReferences() {
  for (int i = 1; i < states.size(); i++) {
    states[states[i].link].suffixReferences.push_back(i);
  }
  suffixReferences = true;
}
bool contains(std::string s) {
  int i = 0;
  Interval in;
  for (auto &c : s) {
    in = Interval(std::max(MIN_CHAR, (int)c - delta),
                  std::min(MAX_CHAR, (int)c + delta));
    i = states[i].getTransition(in);
    if (i == -1) {
      return false;
    }
  }
  return true;
}
// Returns the position of the first occurrence of a non-empty string s,
// or -1 if it does not occur
int first(std::string s) {
  int next = 0;
  for (int i = 0; i < s.size(); i++) {
    next = states[next].getTransition(
        Interval(std::max(MIN_CHAR, (int)s[i] - delta),
                 std::min(MAX_CHAR, (int)s[i] + delta)));
    if (next == -1)
      return -1;
  }
  return states[next].first - s.size() + 1;
}
// Return a vector of positions where a non-empty string s occurs
std::vector<int> positions(std::string s) {
  std::vector<int> p;
  int sz = s.size();
  if (!suffixReferences)
    computeSuffixReferences();
  int next = 0;
  for (int i = 0; i < sz; i++) {
    next = states[next].getTransition(
        Interval(std::max(MIN_CHAR, (int)s[i] - delta),
                 std::min(MAX_CHAR, (int)s[i] + delta)));
    if (next == -1)
      return {};
  }
  // Traverse link tree down from first occurrence to find all others
  std::vector<int> stack = {next};
  while (stack.size() > 0) {
    next = stack.back();
    stack.pop_back();
    if (!states[next].clone)
      p.push_back(states[next].first - sz + 1);
    for (auto &i : states[next].suffixReferences) {
      stack.push_back(i);
    }
  }
  // sort(p.begin(), p.end());
  return p;
}
*/