#include <iostream>
#include <vector>
#include <unordered_map>
#define MAX_CHAR 256
#define MIN_CHAR 0

// Naive check if two strings of the same length are equal
bool isDeltaMatch(std::string x, std::string y, unsigned int delta)
{
    if (x.length() != y.length())
        return false;
    for (int i = 0; i < x.length(); ++i)
        if (std::abs(x[i] - y[i]) > delta)
            return false;
    return true;
}
// Compute the hash of a string -> Just the sum of the characters
long int hash(std::string x)
{
    long int sum = 0L;
    for (int i = 0; i < x.length(); i++)
        sum += x[i];

    return sum;
}
// Get the delta hash table -> hash(x) : Posible positions for x
std::unordered_map<long int, std::vector<int>> computeIx(std::string x, int k, int delta)
{
    std::unordered_map<long int, std::vector<int>> deltaHashTable;
    long int begin, end;
    // Iterate over all factor of length k
    for (int pos = 0; pos <= x.length() - k; pos++)
    {
        std::string subx = x.substr(pos, k);
        // std::cout<< " # " << subx;
        begin = hash(subx) - k * delta > k * MIN_CHAR ? hash(subx) - k * delta : k * MIN_CHAR;
        end = hash(subx) + k * delta < k * MAX_CHAR ? hash(subx) + k * delta : k * MAX_CHAR;
        for (long int i = begin; i <= end; ++i)
        {
            // If i isn't presnt - Add this hash value with an array with position
            // Otherwise just add to the end the position
            deltaHashTable[i].push_back(pos);
        }
    }
    return deltaHashTable;
}
std::string hashDeltaBM(std::string t, std::string p, unsigned int delta, int k = 2)
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
    std::unordered_map<long int, std::vector<int>> deltaHashTable = computeIx(p, k, delta);
    std::cout << "---------------------------" << std::endl;
    // Print the entire delta hash table
    for (auto elem : deltaHashTable)
    {
        std::cout << elem.first << " ->";
        for (auto arrele : elem.second)
            std::cout << " " << arrele << ",";
        std::cout << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
    // Searching
    int j, i = m;
    long int h;
    while (i < t.length())
    {
        h = hash(t.substr(i - k, k));
        std::cout << t.substr(i - k, k) << "- H:" << h << '\n';
        if (deltaHashTable.count(h) == 0)
        {
            i += m - k + 1;
            continue;
        }
        for (int j = 0; j < deltaHashTable[h].size(); ++j)
        {
            if (i - k - deltaHashTable[h][j] + m > t.length())
                continue;
            std::cout << t.substr(i - k - deltaHashTable[h][j], m) << " " << '\n';
            if (isDeltaMatch(p, t.substr(i - k - deltaHashTable[h][j], m), delta))
            {
                answ += std::to_string(i - k - deltaHashTable[h][j]) + " ";
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
    unsigned int delta;
    std::cin >> y >> x;
    std::cin >> delta;
    std::cout << hashDeltaBM(y, x, delta) << std::endl;
    return 0;
}