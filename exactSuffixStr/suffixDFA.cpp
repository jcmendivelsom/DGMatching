#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
typedef std::pair<char, int> tr;

// A single state in our DFA, which represents an equivalence class.
struct State
{
    int len;
    int link;
    int first;
    bool clone = false;
    std::vector<tr> transitions;
    std::vector<int> suffixreferences;
    bool terminal = false;
    int index;
    void addTransition(char c, int i)
    {
        transitions.push_back(tr(c, i));
    }
    // Returns the index of a state or -1 if no transition exists for c
    int getTransition(char c)
    {
        for (auto &t : transitions)
        {
            if (t.first == c)
            {
                return t.second;
            }
        }
        return -1;
    }
    // Updates the transition through c to a new index i
    void updateTransition(char c, int i)
    {
        for (auto &t : transitions)
        {
            if (t.first == c)
            {
                t.second = i;
                return;
            }
        }
    }
};

struct SuffixAutomaton
{
    bool suffixreferences = false;
    std::vector<State> states;
    // Returns the state at index i
    State getState(int i)
    {
        return states[i];
    }
    // Create a new state and return its index (requires t0 already initialized)
    int addState(int len)
    {
        State a;
        a.len = len;
        a.index = states.back().index + 1;
        states.push_back(a);
        return a.index;
    }
    // Populate each state with a vector of its children in the link tree
    void computeSuffixReferences()
    {
        for (int i = 1; i < states.size(); i++)
        {
            states[states[i].link].suffixreferences.push_back(i);
        }
        suffixreferences = true;
    }

    SuffixAutomaton(std::string s)
    {
        // Initial state t0 will be initialized as last
        State l;
        l.len = 0;
        l.link = -1;
        l.index = 0;
        states.push_back(l);
        int last = 0;
        for (auto &c : s)
        {
            bool done = false;
            // Create a new state for a new equivalence class
            int cur = addState(states[last].len + 1);
            // Mark the ending position of the first occurrence of this state
            states[cur].first = states[last].len;
            // Keep following links until we find a transition through c
            int linked = last;
            int t = states[linked].getTransition(c);
            while (t == -1)
            {
                states[linked].addTransition(c, states[cur].index);
                if (states[linked].link != -1)
                {
                    linked = states[linked].link;
                    t = states[linked].getTransition(c);
                }
                else // We have climbed the link tree to the root
                {
                    // Add cur as a child of the root in the link tree and
                    // process the next character
                    states[cur].link = 0;
                    last = cur;
                    done = true;
                    break;
                }
            }
            if (done)
                continue;
            // If we have reached here, we have found a state p
            // such that p transitions through c to some state q at index t
            int p = linked;
            int q = t;
            if (states[q].len == states[p].len + 1)
            {
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
            while (t == q)
            {
                states[linked].updateTransition(c, clone);
                linked = states[linked].link;
                if (linked != -1)
                {
                    t = states[linked].getTransition(c);
                }
                else
                {
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
        while (link != -1)
        {
            int linked = link;
            states[linked].terminal = true;
            link = states[linked].link;
        }
    }

    // O(s) query to see if our source text contains a substring s
    bool contains(std::string s)
    {
        int i = 0;
        for (auto &c : s)
        {
            i = states[i].getTransition(c);
            if (i == -1)
            {
                return false;
            }
        }
        return true;
    }
    // Returns the position of the first occurrence of a non-empty string s,
    // or -1 if it does not occur
    int first(std::string s)
    {
        int next = 0;
        for (int i = 0; i < s.size(); i++)
        {
            next = states[next].getTransition(s[i]);
            if (next == -1)
                return -1;
        }
        return states[next].first - s.size() + 1;
    }
    // Return a vector of positions where a non-empty string s occurs
    std::vector<int> positions(std::string s)
    {
        std::vector<int> p;
        int sz = s.size();
        if (!suffixreferences)
            computeSuffixReferences();
        int next = 0;
        for (int i = 0; i < sz; i++)
        {
            next = states[next].getTransition(s[i]);
            if (next == -1)
                return {};
        }
        // Traverse link tree down from first occurrence to find all others
        std::vector<int> stack = {next};
        while (stack.size() > 0)
        {
            next = stack.back();
            stack.pop_back();
            if (!states[next].clone)
                p.push_back(states[next].first - sz + 1);
            for (auto &i : states[next].suffixreferences)
            {
                stack.push_back(i);
            }
        }
        // sort(p.begin(), p.end());
        return p;
    }
};

int main()
{
    std::string s;
    std::cin >> s;
    std::cout << "Constructing automaton..." << std::endl;
    SuffixAutomaton sa = SuffixAutomaton(s);
    std::cout << "String: \"" << s << "\" is of size " << s.size() << " and its automaton has " << sa.states.size() << " states" << std::endl;
    std::cout << "Enter a substring to see if it occurs:" << std::endl;

    std::string p;
    std::cin >> p;
    bool occurs = sa.contains(p);
    if (occurs)
    {
        std::cout << "YES, \"" << s << "\" contains the substring "
                  << "\"" << p << "\"" << std::endl;
    }
    else
    {
        std::cout << "No, \"" << s << "\" does not contain the substring "
                  << "\"" << p << "\"" << std::endl;
    }
}