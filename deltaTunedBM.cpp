#include <iostream>
#include <string>
bool isDeltaMatch(std::string x, std::string y, unsigned int delta)
{
    if (x.length() != y.length())
        return false;
    for (int i = 0; i < x.length(); ++i)
        if (std::abs(x[i] - y[i]) > delta)
            return false;
    return true;
}
int minDeltaMatch(std::string p, char a, unsigned int delta)
{
    int m = p.length();
    for (int i = m - 1; i >= 0; --i)
        if (std::abs(p[i] - a) <= delta)
            return m - (i + 1);
    return m;
}
std::string deltaTunedBM(std::string t, std::string p, unsigned int delta)
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
    int shift[256];
    for (int i = 0; i < 256; ++i)
        shift[i] = minDeltaMatch(p, (char)i, delta);
    int s = minDeltaMatch(p.substr(0, m - 1), p[m - 1], 2 * delta) + 1;
    std::cout << "s " << s << std::endl;
    for (int i = 0; i < m; ++i)
        std::cout << p[i] << " " << shift[p[i]] << std::endl;
    t += std::string(m, p[m - 1]);
    int k, j = m;
    while (j <= n)
    {
        k = shift[t[j - 1]];
        while (k != 0)
        {
            std::cout << "J" << j << " ";
            j += k;
            k = shift[t[j - 1]];
        }
        if (isDeltaMatch(p, t.substr(j - m, m), delta) && j <= n)
            answ += std::to_string(j - m) + " ";
        j += s;
    }
    std::cout << std::endl;
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
    std::cout << deltaTunedBM(y, x, delta) << std::endl;
    return 0;
}