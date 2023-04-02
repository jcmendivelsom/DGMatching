#include <iostream>
#include <string>
#include <bitset>

std::string shiftAnd(std::string t, std::string p, unsigned int delta)
{
    int m = p.length();
    int n = t.length();

    if (m <= 0 || m > 64 || m > n)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }

    std::string answ = "";
    long DTable[256];
    long DState = 0;

    // Preprocessing
    for (int i = 0; i <= 255; ++i)
    {
        DTable[i] = 0;
        for (int j = 0; j < m; ++j)
            DTable[i] |= (std::abs(i - p[j]) <= delta) << j;
        // std::cout << t[i] << " = " << std::bitset<64>(DTable[t[i]]) << std::endl;
    }
    // Searching phase
    std::cout << " =========================== " << std::endl;
    for (int i = 0; i < n; ++i)
    {
        DState = ((DState << 1) | 1L) & DTable[t[i]];
        std::cout << t[i] << " = " << std::bitset<64>(DState) << std::endl;
        // std::cout << y[i] << " = " << std::bitset<16>(R & (1L << m-1))  << std::endl;
        if ((~DState & (1L << m - 1)) == 0)
            answ += std::to_string(i - m + 1) + " ";
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
    std::cout << shiftAnd(y, x, delta) << std::endl;
    return 0;
}