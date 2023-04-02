#include <iostream>
#include <string>
#include <vector>
#include <math.h>
int deltaMisFind(char i, std::string p, unsigned int delta)
{
    int pLen = p.length();
    for (int i = 0; i < pLen; ++i)
        if (std::abs(p[i] - (int)i) <= delta)
            return i;
    return -1;
}
std::string backEqvClass(std::string x, int delta)
{
    int m = x.length();
    std::vector<char> eqClass = {x[m - 1]};
    std::string ans = "";
    for (int i = m - 1; i >= 0; --i)
    {
        for (auto const &eq : eqClass)
        {
            if (std::abs(eq - x[i]) <= delta)
            {
                ans = eq + ans;
                break;
            }
        }
        if (ans.length() != m - i + 1)
        {
            ans = x[i] + ans;
            eqClass.push_back(x[i]);
        }
    }
    return ans.substr(0, m);
}
std::vector<int> dsuffixes(std::string x, int delta)
{
    int f, g, i, m = x.length();
    std::vector<int> suff(m);
    std::vector<int> aux(m);
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i)
    {
        // if (i > g && suff[i + m - 1 - f] < i - g && deltaMisFind(x[i], x.substr(i + m - 1 - f), delta/2) != -1)
        // if (i > g && suff[i + m - 1 - f] < i - g && std::abs(x[i] - x[m - 1]) > 2 * delta)
        // if (i > g && suff[i + m - 1 - f] < i - g && aux[i] > 0)
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else
        {
            // if (i < g || aux[i] <= 0)
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && std::abs(x[g] - x[g + m - 1 - f]) <= delta)
            {
                --g;
            }
            if (g >= 0 && std::abs(x[g] - x[g + m - 1 - f]) > 2 * delta)
                ++aux[g];
            else if (g >= 0)
                --aux[g];
            suff[i] = f - g;
        }
    }
    return suff;
}
std::vector<int> dsuffiMax(std::string x, int delta)
{
    std::vector<int> suff = dsuffixes(x, delta);
    std::vector<int> aux = dsuffixes(backEqvClass(x, delta), delta);
    for (int i = 0; i < suff.size(); i++)
    {
        if (aux[i] > suff[i])
            suff[i] = aux[i];
    }
    return suff;
}
std::vector<int> dNaiveSuffixes(std::string x, int delta)
{
    int g, i, m = x.length();
    std::vector<int> suff(m);
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i)
    {
        g = i;
        while (g >= 0 && std::abs(x[g] - x[g + m - 1 - i]) <= delta)
            --g;
        suff[i] = i - g;
    }
    return suff;
}
std::vector<int> preDGS(std::string x, int delta)
{
    int i, j, m = x.length();
    // std::vector<int> suff = dsuffixes(x, delta), dGS(m);
    std::vector<int> suff = dNaiveSuffixes(x, delta), dGS(m);
    for (i = 0; i < m; ++i)
        dGS[i] = m;
    j = 0;
    for (i = m - 1; i >= 0; --i)
        if (suff[i] == i + 1)
            for (; j < m - 1 - i; ++j)
                if (dGS[j] == m)
                    dGS[j] = m - 1 - i;
    for (i = 0; i <= m - 2; ++i)
        dGS[m - 1 - suff[i]] = m - 1 - i;

    return dGS;
}
int main()
{
    std::string s;
    int delta;
    std::cin >> s;
    std::cin >> delta;
    std::vector<int> dGS = dsuffixes(s, delta);
    std::vector<int> dNaiveGS = dNaiveSuffixes(s, delta);
    std::vector<int> dGSN = dsuffixes(backEqvClass(s, delta), delta);
    std::cout << backEqvClass(s, delta) << std::endl;
    for (int i = 0; i < s.length(); ++i)
        std::cout << "( " << i << " ) " << s[i] << " " << dGS[i] << " " << dNaiveGS[i] << " " << dGSN[i] << std::endl;
    // std::cout << "( " << i << " ) " << s[i] << " " << dGS[i] << " " << dNaiveGS[i] << std::endl;

    return 0;
}