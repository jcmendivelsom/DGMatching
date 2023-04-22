#include "DeltaBMAlgos.h"

////////////////// ALGORITHM ////////////////////////

std::vector<int> DeltaBMAlgos::deltaBM2(std::wstring_view t,
                                        std::wstring_view p, int delta) {
  int m = p.length();
  int n = t.length();
  if (m <= 0 || m > n || delta < 0) {
    throw std::invalid_argument("Invalid parameters! ");
  }

  // PREPROCESSING PHASE
  std::vector<int> answ;
  // Delta period.
  int deltaPer = m;
  for (int k = 1; k < m; ++k) {
    for (int i = 1; i < m - k; ++i) {
      if (std::abs(alph.getValue(p[i]) - alph.getValue(p[i + k])) > delta)
        goto OUTERGO;
    }
    deltaPer = k;
    break;
  OUTERGO:
    continue;
  }
  std::wcout << "Delta period " << deltaPer << "\n";
  // Make reverse of pattern
  std::wstring rev;
  for (int i = m-1; i >= 0; --i)
    rev += p[i];
  std::wstring_view pReverse(rev);
  std::wcout << pReverse << "\n";
  // Compute the Delta Factor Trie for all the k-factors.
  DeltaSuffixAutomaton sa = DeltaSuffixAutomaton(alph, pReverse, delta);
  // Print the entire Delta Factor Trie
  //sa.printAutomaton();

  // SEARCHING PHASE
  int i = m - 1, q, j, b;
  while (i < n) {
    q = 0;
    j = i;
    b = 0;
    while (sa.getState(q).travel(alph.getValue(t[j])) != -1) {
      q = sa.getState(q).travel(alph.getValue(t[j]));
      j = j - 1;
      if (sa.getState(q).terminal) {
        b = i - j;
      }
    }
    // std::wcout << "b:  " << b << "\n";
    // std::wcout<< "Salgo del while " << t[j]  << "\n";
    if (i - j >= m) {
      answ.push_back(i - m + 1);
      // std::wcout << "Match en " << i - m + 1 << "\n";
      i = i + deltaPer;
    } else {
      i = i + m - b;
    }
    // std::wcout << "i:  " << i << "\n";
  }

  if (answ.empty())
    return {-1};
  return answ;
}

////////////////// INTERVAL STATE /////////////////////

// A single state in our DFA, which represents an equivalence class.
void DeltaBMAlgos::IntervalState::addTransition(Interval in, int s) {
  boundVals.first = std::min(boundVals.first, in.first);
  boundVals.second = std::max(boundVals.second, in.second);
  transitions[in] = s;
}
// Returns the index of a state or -1 if no transition exists for c
int DeltaBMAlgos::IntervalState::getTransition(Interval in) {
  for (auto const &t : transitions) {
    if (t.first.first == in.first && t.first.second == in.second) {
      return t.second;
    }
  }
  return -1;
}
// Returns the index of a state or -1 if no transition exists for c
std::vector<int> DeltaBMAlgos::IntervalState::travelWith(int c) {
  std::vector<int> states;
  for (auto const &t : transitions) {
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

// Updates the transition through c to a new index i
void DeltaBMAlgos::IntervalState::updateTransition(Interval in, int s) {
  for (auto &t : transitions) {
    if (t.first.first == in.first && t.first.second == in.second) {
      t.second = s;
      return;
    }
  }
}

DeltaBMAlgos::IntervalState::IntervalState() {
  this->len = 0;
  this->link = -1;
  this->first = 0;
  this->index = 0;
  this->clone = false;
  this->terminal = false;
  // std::unordered_map<Interval, int> transitions;
  // std::vector<int> suffixReferences;
  this->boundVals = Interval(INT32_MAX, INT32_MIN);
}

////////////////// DELTA SUFFIX AUTOMATON /////////////////////

// Returns the state at index i
DeltaBMAlgos::IntervalState
DeltaBMAlgos::DeltaSuffixAutomaton::getState(int i) {
  return states[i];
}
// Create a new state and return its index (requires t0 already initialized)
int DeltaBMAlgos::DeltaSuffixAutomaton::addState(int len) {
  IntervalState a;
  a.len = len;
  a.index = states.back().index + 1;
  states.push_back(a);
  return a.index;
}
// Populate each state with a vector of its children in the link tree
void DeltaBMAlgos::DeltaSuffixAutomaton::computeSuffixReferences() {
  for (int i = 1; i < states.size(); i++) {
    states[states[i].link].suffixReferences.push_back(i);
  }
  suffixReferences = true;
}

DeltaBMAlgos::DeltaSuffixAutomaton::DeltaSuffixAutomaton(Alphabet alpha,
                                                         std::wstring_view s,
                                                         int delta) {
  // Initial state t0 will be initialized as last
  this->delta = delta;
  IntervalState l;
  l.len = 0;
  l.link = -1;
  l.index = 0;
  states.push_back(l);

  int last = 0;
  Interval in;
  for (auto &c : s) {
    in = Interval(std::max(alpha.getMinValue(), alpha.getValue(c) - delta),
                  std::min(alpha.getMaxValue(), alpha.getValue(c) + delta));
    // Create a new state for a new equivalence class
    int cur = addState(states[last].len + 1);
    // Mark the ending position of the first occurrence of this state
    states[cur].first = states[last].len;
    // Keep following links until we find a transition through c
    int linked = last;
    int t = states[linked].getTransition(in);
    while (t == -1) {
      states[linked].addTransition(in, states[cur].index);
      if (states[linked].link != -1) {
        linked = states[linked].link;
        t = states[linked].getTransition(in);
      } else // We have climbed the link tree to the root
      {
        // Add cur as a child of the root in the link tree and
        // process the next character
        states[cur].link = 0;
        last = cur;
        goto OUTER;
      }
    }
  OUTER:
    continue;
    // If we have reached here, we have found a state p
    // such that p transitions through c to some state q at index t
    int p = linked;
    int q = t;
    if (states[q].len == states[p].len + 1) {
      // Cur is a child of q in the link tree, process next character
      states[cur].link = states[q].index;
      last = cur;
      continue;
    }
    // Cur is not a child of q in the link tree, we must create a new
    // state that will be the parent of both q and cur in the link tree
    int clone = addState(states[p].len + 1);
    states[clone].link = states[q].link;
    states[clone].transitions = states[q].transitions;
    states[clone].first = states[q].first;
    states[clone].clone = true;
    states[cur].link = states[clone].index;
    states[q].link = states[clone].index;

    // Updates transitions through c to q to match our new state
    // TODO: Double check that p needs to be updated as well
    linked = p;
    while (t == q) {
      states[linked].updateTransition(in, clone);
      linked = states[linked].link;
      if (linked != -1) {
        t = states[linked].getTransition(in);
      } else {
        break;
      }
    }
    // We are finished, advance last to the new state and continue
    last = cur;
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
  fillTransitions();
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
        for (int state : states[nfaState].travelWith(input)) {
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
        aux.index = newDfaState;
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
    auto it = states[indexCurrState].transitions.cbegin();
    while (it != states[indexCurrState].transitions.cend()) {
      if (it->first.first != it->first.second) {
        // supported in C++11
        it = states[indexCurrState].transitions.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void DeltaBMAlgos::DeltaSuffixAutomaton::fillTransitions() {
  for (auto &st : states){
    for (auto &tr : st.transitions){
      for(int i = tr.first.first; i <= tr.first.second; ++i){
        st.travelTransitions[i] = tr.second;
      }
    }
  }
}

void DeltaBMAlgos::DeltaSuffixAutomaton::printAutomaton() {
  std::wstring aux, aux2;
  for (auto const &st : states)
    for (auto const &tr : st.transitions) {
      aux = st.terminal ? L"%" : L" ";
      aux2 = states[tr.second].terminal ? L"%" : L" ";
      std::wcout << "( " << st.index << " )" << aux << "-> "
                << "[" << tr.first.first << "," << tr.first.second
                << "] "
                << "-> "
                << "( " << tr.second << " )" << aux2 << std::endl;
    }
}
// O(s) query to see if our source text contains a substring s
/*
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