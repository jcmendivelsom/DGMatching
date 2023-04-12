#include "DeltaBMAlgos.h"


/* Delta Node: [a, b] -> Interval of acceptance
               Children -> map of pointers to child nodes

class DeltaNode
{
public:
    int a;
    int b;
    std::vector<Interval> childrenIntervals;
    std::vector<DeltaNode *> childrens;
    std::vector<int> positions;
    DeltaNode *link;
    DeltaNode(int a, int b)
    {
        link = NULL;
        this->a = a;
        this->b = b;
    }
    DeltaNode *buildTransition(int c, int delta)
    {
        Interval inDeltaC = Interval(std::max(MIN_CHAR, c - delta), std::min(MAX_CHAR, c + delta));

        for (int i = 0; i < childrenIntervals.size(); ++i)
        {
            if (!(childrenIntervals[i].second < inDeltaC.first || inDeltaC.second < childrenIntervals[i].first))
            {
                childrenIntervals[i].first = std::min(childrenIntervals[i].first, inDeltaC.first);
                childrenIntervals[i].second = std::max(childrenIntervals[i].second, inDeltaC.second);
                return childrens[i];
            }
        }
        // If get here means that we do not find a possible path
        childrenIntervals.push_back(inDeltaC);
        DeltaNode *newChild = new DeltaNode(inDeltaC.first, inDeltaC.second);
        childrens.push_back(newChild);
        return newChild;
    }
    DeltaNode *getTransition(int c)
    {
        for (int i = 0; i < childrenIntervals.size(); ++i)
        {
            if (childrenIntervals[i].first <= c && c <= childrenIntervals[i].second)
                return childrens[i];
        }
        return NULL;
    }
    void printNode()
    {
        for (auto intvl : childrenIntervals)
            std::cout << "[" << intvl.first - (int)' ' << ", " << intvl.second - (int)' ' << "] ";
        std::cout << "{ ";
        for (auto pos : positions)
            std::cout << pos << ", ";
        std::cout << " }\n";
    }
};
class DeltaFactorTrie
{
    DeltaNode *root;
    std::string text;
    int delta;
    void insertFactor(int pos)
    {
        std::string currFactor = text.substr(pos, k);
        DeltaNode *traveler = root;
        for (int i = 0; i < currFactor.length(); ++i)
            traveler = traveler->buildTransition(currFactor[i], delta);
        traveler->positions.push_back(pos);
    }
    void print(DeltaNode *inputNode)
    {
        if (inputNode == NULL)
            return;
        inputNode->printNode();
        for (auto node : inputNode->childrens)
        {
            print(node);
        }
    }

public:
    int k;
    DeltaFactorTrie(std::string text, int delta)
    {
        this->text = text;
        this->delta = delta;
        root = new DeltaNode(MIN_CHAR - 1, MAX_CHAR);
        k = std::floor(std::log2(text.length()));
        for (int i = 0; i <= text.length() - k; ++i)
        {
            insertFactor(i);
        }
    }

    void printTrie()
    {
        print(root);
    }

    DeltaNode *travelWith(std::string p)
    {
        DeltaNode *traveler = root;
        int i = 0;
        while (traveler != NULL && i < p.length())
        {
            traveler = traveler->getTransition(p[i]);
            ++i;
        }
        return traveler;
    }
};

std::string deltaBM1SkipSearch(std::string t, std::string p, unsigned int delta)
{
    int m = p.length();
    int n = t.length();
    if (m <= 0 || m > n)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }
    std::string answ = "";
    // Preprocessing
    DeltaFactorTrie *trie = new DeltaFactorTrie(p, delta);
    std::cout << "---------------------------" << std::endl;
    // Print the entire delta hash table
    trie->printTrie();
    std::cout << "---------------------------" << std::endl;
    // Searching
    int i = m, k = trie->k;
    std::vector<int> possiblePos;
    while (i < t.length())
    {
        if (trie->travelWith(t.substr(i - k, k)) == NULL)
        {
            i += m - k + 1;
            continue;
        }
        possiblePos = trie->travelWith(t.substr(i - k, k))->positions;
        for (int j = 0; j < possiblePos.size(); ++j)
        {
            if (i - k - possiblePos[j] + m > t.length())
                continue;
            std::cout << t.substr(i - k - possiblePos[j], m);
            if (isDeltaMatch(p, t.substr(i - k - possiblePos[j], m), delta))
            {
                std::cout<< " Ok " << '\n';
                answ += std::to_string(i - k - possiblePos[j]) + " ";
            } else {
                std::cout<< " X " << '\n';
            }
        }
        i += m - k + 1;
    }
    if (answ != "")
        return answ;
    return "-1";
}

int main()
{
    std::string y, x;
    int delta;
    std::cin >> y >> x;
    std::cin >> delta;
    std::cout << deltaBM1SkipSearch(y, x, delta) << std::endl;
}
*/