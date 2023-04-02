#include <iostream>
#include <string>
#include <list>
bool isDeltaMatch(std::string x, std::string y, unsigned int delta)
{
    if (x.length() != y.length())
        return false;
    for (int i = 0; i < x.length(); ++i)
        if (std::abs(x[i] - y[i]) > delta)
            return false;
    return true;
}
std::string deltaSkipSearch(std::string t, std::string p, unsigned int delta)
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
    std::list<int> z[256];
    for (int i = 0; i < 256; ++i)
    {
        z[i] = {};
        for (int j = 0; j < m; ++j)
        {
            if (abs(i - p[j]) <= delta)
                z[i].push_back(j);
        }
    }
    // Searching
    int j = m;
    while (j <= n)
    {
        for (auto i : z[t[j - 1]])
        {
            std::cout << "J" << j - i - 1 << " " << t.substr(j - i - 1, m) << std::endl;
            if (isDeltaMatch(p, t.substr(j - i - 1, m), delta))
            {
                answ += std::to_string(j - i - 1) + " ";
            }
        }
        j += m;
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
    std::cout << deltaSkipSearch(y, x, delta) << std::endl;
    return 0;
}