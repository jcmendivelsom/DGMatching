#include <iostream>
#include <string>
#include <vector>
#define MAX_CHAR 256
bool isDeltaSuffix(std::string psub, std::string p, unsigned int delta)
{
    int pLen = p.length();
    int psubLen = psub.length();
    if (psubLen > pLen)
        return false;
    if (psubLen == 0)
        return true;
    for (int i = 0; i < psubLen; ++i)
        if (std::abs(psub[i] - p[pLen - psubLen + i]) > delta)
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

std::string deltaFastSearch(std::string t, std::string p, unsigned int delta)
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
    int dBCshift[MAX_CHAR];
    int auxMinDelta;
    // Delta Bad Character
    for (int i = 0; i < MAX_CHAR; ++i)
        dBCshift[i] = m;
    for (int i = m - 1; i >= 0; --i)
        for (int j = 0; j <= delta; ++j)
        {
            if (dBCshift[p[i] + j] == m)
                dBCshift[p[i] + j] = minDeltaMatch(p, (char)(p[i] + j), delta);
            if (dBCshift[p[i] - j] == m)
                dBCshift[p[i] - j] = minDeltaMatch(p, (char)(p[i] - j), delta);
        }
    // Delta Forward Good Suffix
    int dForwardGSshift[m][MAX_CHAR], posAux;
    for (int j = m - 1; j >= 0; --j)
        for (int i = 0; i < MAX_CHAR; ++i)
        {
            dForwardGSshift[j][i] = m + 1;
            if (dBCshift[i] != m)
                for (int k = 1; k <= m; ++k)
                {
                    if (k <= j - 1)
                        if (p[j - 1] == p[j - 1 - k] || std::abs(p[m - k] - i) > delta) // p[j-1-kc -> this could be negative]
                            continue;
                    posAux = j - k >= 0 ? j - k : 0;
                    // std::cout << p.substr(posAux, m - k - posAux) << " " << p.substr(j) << std::endl;
                    if (isDeltaSuffix(p.substr(posAux, m - k - posAux), p.substr(j), 2 * delta))
                    {
                        // std::cout << " + ";
                        dForwardGSshift[j][i] = k;
                        break;
                    }
                    // std::cout << std::endl;
                }
        }
    std::cout << "-------------------" << '\n';
    for (int j = 0; j < m; ++j)
        std::cout << p[j] << " dBC: " << dBCshift[p[j]] << std::endl;
    std::cout << "*******************" << '\n';
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            std::cout << i << " - " << p[j] << " dGS: " << dForwardGSshift[i][p[j]] << std::endl;
    // Searching phase.
    t += std::string(m + 1, p[m - 1]);
    int s = 0;
    int k, j;
    while (s <= n - m)
    {
        while (dBCshift[t[s + m - 1]] > 0)
            s += dBCshift[t[s + m - 1]];
        j = m - 2;
        while (j >= 0 && std::abs(p[j] - t[s + j]) <= delta)
            j -= 1;
        if (j < 0)
            answ += std::to_string(s) + " ";
        s += dForwardGSshift[j + 1][t[s + m]];
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
    std::cout << deltaFastSearch(y, x, delta) << std::endl;
    return 0;
}