#include <iostream>
#include <vector>
#include <cmath>

#define MAX_CHAR 256
#define MIN_CHAR 0
typedef std::pair<int, int> Interval;
/* Delta Node: [a, b] -> Interval of acceptance
               Children -> map of pointers to child nodes
*/
class DeltaNode
{
public:
    int a;
    int b;
    std::vector<Interval> childrenIntvls;
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

        for (int i = 0; i < childrenIntvls.size(); ++i)
        {
            if (!(childrenIntvls[i].second < inDeltaC.first || inDeltaC.second < childrenIntvls[i].first))
            {
                childrenIntvls[i].first = std::min(childrenIntvls[i].first, inDeltaC.first);
                childrenIntvls[i].second = std::max(childrenIntvls[i].second, inDeltaC.second);
                return childrens[i];
            }
        }
        // If get here means that we do not find a possible path
        childrenIntvls.push_back(inDeltaC);
        DeltaNode *newChild = new DeltaNode(inDeltaC.first, inDeltaC.second);
        childrens.push_back(newChild);
        return newChild;
    }
    DeltaNode *getTransition(int c)
    {
        for (int i = 0; i < childrenIntvls.size(); ++i)
        {
            if (childrenIntvls[i].first <= c && c <= childrenIntvls[i].second)
                return childrens[i];
        }
        return NULL;
    }
    void printNode()
    {
        for (auto intvl : childrenIntvls)
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
bool isDeltaMatch(std::string x, std::string y, unsigned int delta)
{
    if (x.length() != y.length())
        return false;
    for (int i = 0; i < x.length(); ++i)
        if (std::abs(x[i] - y[i]) > delta)
            return false;
    return true;
}
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
    std::vector<int> posiblePos;
    while (i < t.length())
    {
        if (trie->travelWith(t.substr(i - k, k)) == NULL)
        {
            i += m - k + 1;
            continue;
        }
        posiblePos = trie->travelWith(t.substr(i - k, k))->positions;
        for (int j = 0; j < posiblePos.size(); ++j)
        {
            if (i - k - posiblePos[j] + m > t.length())
                continue;
            std::cout << t.substr(i - k - posiblePos[j], m);
            if (isDeltaMatch(p, t.substr(i - k - posiblePos[j], m), delta))
            {
                std::cout<< " Ok " << '\n';
                answ += std::to_string(i - k - posiblePos[j]) + " ";
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