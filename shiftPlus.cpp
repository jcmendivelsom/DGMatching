#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
std::string shiftPlus(std::string t, std::string p, unsigned int delta, unsigned int gamma)
{
    int m = p.length();
    int n = t.length();
    int d = std::ceil(std::log2(delta * m));

    if (m <= 0 || m * d > 64 || m > n)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }

    std::string answ = "";
    long DTable[256];
    long DState = 0;
    long GTable[256];
    long GState = 0;

    // Preproccessing
    bool deltaComp;
    for (int i = 0; i <= 255; ++i)
    {
        DTable[i] = 0;
        GTable[i] = 0;
        for (int j = 0; j < m; ++j)
        {
            deltaComp = std::abs(i - p[j]) <= delta;
            DTable[i] |= deltaComp << j;
            GTable[i] = GTable[i] << d;
            if (deltaComp)
            {
                GTable[i] |= (long)std::abs(i - p[j]);
            }
        }
    }
    // Serch Phase
    std::cout << " =========================== " << std::endl;
    std::cout << " d " << d << std::endl;
    for (int i = 0; i < n; ++i)
    {
        DState = ((DState << 1) | 1L) & DTable[t[i]];
        std::cout << "D" << t[i] << " = " << std::bitset<64>(DState) << std::endl;
        GState = (GState >> d) + GTable[t[i]];
        std::cout << "G" << t[i] << " = " << std::bitset<64>(GState) << std::endl;
        // std::cout << " = " << std::bitset<64>(GState & ~(~1L<<d-1)) << std::endl;
        if ((~DState & (1L << m - 1)) == 0 && (GState & ~(~1L << d - 1)) <= gamma)
            answ += std::to_string(i - m + 1) + " ";
    }
    if (answ != "")
        return answ;
    return "-1";
}
int main()
{
    std::string y, x;
    unsigned int delta, gamma;
    std::cin >> y >> x;
    std::cin >> delta >> gamma;
    std::cout << shiftPlus(y, x, delta, gamma) << std::endl;
    return 0;
}