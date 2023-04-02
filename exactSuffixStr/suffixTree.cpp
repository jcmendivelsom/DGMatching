#include <iostream>
#include <unordered_map>
#define MAX_CHAR 256

class SuffixTreeNode
{
public:
    std::unordered_map<char, SuffixTreeNode *> children;
    SuffixTreeNode *suffixLink;
    int suffixIndex;
    int start;
    int *end;

    SuffixTreeNode(int start, int *end)
    {
        suffixLink = NULL;
        this->start = start;
        this->end = end;
        suffixIndex = -1;
    }
};
class SuffixTree
{
    SuffixTreeNode *root;
    std::string text;

    // Active Point
    SuffixTreeNode *activeNode;
    char activeEdge;
    int activeLength;

    SuffixTreeNode *lastNewNode;
    int remainingSuffixCount;
    int leafEnd;
    int *splitEnd;

    int edgeLength(SuffixTreeNode *n)
    {
        return *(n->end) - (n->start) + 1;
    }

    int walkDown(SuffixTreeNode *currNode)
    {
        /*activePoint change for walk down (APCFWD) using
        Skip/Count Trick (Trick 1). If activeLength is greater
        than current edge length, set next internal node as
        activeNode and adjust activeEdge and activeLength
        accordingly to represent same activePoint*/
        if (activeLength >= edgeLength(currNode))
        {
            activeEdge =
                (int)text[activeEdge + edgeLength(currNode)] - (int)' ';
            activeLength -= edgeLength(currNode);
            activeNode = currNode;
            return 1;
        }
        return 0;
    }

    void extendSuffixTree(int pos)
    {
        /*Extension Rule 1, this takes care of extending all
        leaves created so far in tree*/
        leafEnd = pos;

        /*Increment remainingSuffixCount indicating that a
        new suffix added to the list of suffixes yet to be
        added in tree*/
        remainingSuffixCount++;

        /*set lastNewNode to NULL while starting a new phase,
        indicating there is no internal node waiting for
        it's suffix link reset in current phase*/
        lastNewNode = NULL;

        // Add all suffixes (yet to be added) one by one in tree
        while (remainingSuffixCount > 0)
        {

            if (activeLength == 0)
            {
                // APCFALZ
                activeEdge = (int)text[pos] - (int)' ';
            }
            // There is no outgoing edge starting with
            // activeEdge from activeNode
            if (activeNode->children.count(activeEdge) == 0)
            {
                // Extension Rule 2 (A new leaf edge gets created)
                activeNode->children[activeEdge] = new SuffixTreeNode(pos, &leafEnd);

                /*A new leaf edge is created in above line starting
                from an existing node (the current activeNode), and
                if there is any internal node waiting for it's suffix
                link get reset, point the suffix link from that last
                internal node to current activeNode. Then set lastNewNode
                to NULL indicating no more node waiting for suffix link
                reset.*/
                if (lastNewNode != NULL)
                {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = NULL;
                }
            }
            // There is an outgoing edge starting with activeEdge
            // from activeNode
            else
            {
                // Get the next node at the end of edge starting
                // with activeEdge
                SuffixTreeNode *next = activeNode->children[activeEdge];
                if (walkDown(next)) // Do walkdown
                {
                    // Start from next node (the new activeNode)
                    continue;
                }
                /*Extension Rule 3 (current character being processed
                is already on the edge)*/
                if (text[next->start + activeLength] == text[pos])
                {
                    // If a newly created node waiting for it's
                    // suffix link to be set, then set suffix link
                    // of that waiting node to current active node
                    if (lastNewNode != NULL && activeNode != root)
                    {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = NULL;
                    }

                    // APCFER3
                    activeLength++;
                    /*STOP all further processing in this phase
                    and move on to next phase*/
                    break;
                }

                /*We will be here when activePoint is in middle of
                the edge being traversed and current character
                being processed is not on the edge (we fall off
                the tree). In this case, we add a new internal node
                and a new leaf edge going out of that new node. This
                is Extension Rule 2, where a new leaf edge and a new
                internal node get created*/
                int posEnd = next->start + activeLength - 1;
                splitEnd = &posEnd;
                // New internal node
                SuffixTreeNode *split = new SuffixTreeNode(next->start, splitEnd);
                activeNode->children[activeEdge] = split;

                // New leaf coming out of new internal node
                split->children[text[pos]] = new SuffixTreeNode(pos, &leafEnd);
                next->start += activeLength;
                split->children[activeEdge] = next;

                /*We got a new internal node here. If there is any
                internal node created in last extensions of same
                phase which is still waiting for it's suffix link
                reset, do it now.*/
                if (lastNewNode != NULL)
                {
                    /*suffixLink of lastNewNode points to current newly
                    created internal node*/
                    lastNewNode->suffixLink = split;
                }

                /*Make the current newly created internal node waiting
                for it's suffix link reset (which is pointing to root
                at present). If we come across any other internal node
                (existing or newly created) in next extension of same
                phase, when a new leaf edge gets added (i.e. when
                Extension Rule 2 applies is any of the next extension
                of same phase) at that point, suffixLink of this node
                will point to that internal node.*/
                lastNewNode = split;
            }

            /* One suffix got added in tree, decrement the count of
            suffixes yet to be added.*/
            remainingSuffixCount--;
            if (activeNode == root && activeLength > 0) // APCFER2C1
            {
                activeLength--;
                activeEdge = (int)text[pos -
                                       remainingSuffixCount + 1] -
                             (int)' ';
            }

            // APCFER2C2
            else if (activeNode != root)
            {
                activeNode = activeNode->suffixLink;
            }
        }
    }
    void print(int i, int j)
    {
        int k;
        for (k = i; k <= j; k++)
            std::cout << text[k];
    }

    // Print the suffix tree as well along with setting suffix index
    // So tree will be printed in DFS manner
    // Each edge along with it's suffix index will be printed
    void setSuffixIndexByDFS(SuffixTreeNode *n, int labelHeight)
    {
        if (n == NULL)
            return;

        if (n->start != -1) // A non-root node
        {
            // Print the label on edge from parent to current node
            print(n->start, *(n->end));
        }
        int leaf = 1;
        int i;
        for (auto elem : n->children)
        {
            if (leaf == 1 && n->start != -1)
                std::cout << " [" << n->suffixIndex << "]\n";

            // Current node is not a leaf as it has outgoing
            // edges from it.
            leaf = 0;
            setSuffixIndexByDFS(elem.second,
                                labelHeight + edgeLength(elem.second));
        }
        if (leaf == 1)
        {
            n->suffixIndex = text.length() - labelHeight;
            std::cout << " [" << n->suffixIndex << "]\n";
        }
    }

public:
    /*Build the suffix tree and print the edge labels along with
    suffixIndex. suffixIndex for leaf edges will be >= 0 and
    for non-leaf edges will be -1*/
    SuffixTree(std::string text)
    {
        /*Root is a special node with start and end indices as -1,
        as it has no parent from where an edge comes to root*/
        int *rootEnd = (int *)-1;
        this->root = new SuffixTreeNode(-1, rootEnd);
        this->text = text;

        // Active Point
        activeNode = root;
        activeEdge = -1;
        activeLength = 0;

        lastNewNode = NULL;
        remainingSuffixCount = 0;
        leafEnd = -1;
        splitEnd = NULL;

        for (int i = 0; i < text.length(); i++)
            extendSuffixTree(i);
        setSuffixIndexByDFS(root, 0);
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

std::string suffTreeDeltaBM(std::string t, std::string p, unsigned int delta, int k = 2)
{
    int m = p.length();
    int n = t.length();
    if (m <= 0 || m > n || k > m)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }
    std::string answ = "";
    // Preprocessing
    SuffixTree *sfTree = new SuffixTree(p);
    std::cout << "---------------------------" << std::endl;

    // Searching

    if (answ != "")
        return answ;
    return "-1";
}
int main()
{
    SuffixTree *tree = new SuffixTree("152165265");

    return 0;
}