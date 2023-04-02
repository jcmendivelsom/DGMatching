#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
std::string forwardScan(std::string t, std::string p, unsigned int delta, unsigned int gamma)
{
    int m = p.length();
    int n = t.length();
    int l = 1 + std::ceil(std::log2(gamma + 1));

    if (m <= 0 || m * l > 64 || m > n)
    {
        std::cout << "Invalid parameters! " << std::endl;
        return "-1";
    }

    std::string answ = "";
    long BTable[256];
    long DState, auxHighBits, H;

    // Preproccessing
    for (int i = 0; i <= 255; ++i)
    {
        BTable[i] = 0;
        for (int j = m - 1; j >= 0; --j)
        {
            if (std::abs(i - p[j]) <= delta)
                BTable[i] = (BTable[i] << l) | (long)std::abs(i - p[j]);
            else
                BTable[i] = (BTable[i] << l) | (long)gamma + 1;
        }
    }
    // Serch Phase
    std::cout << " =========================== " << std::endl;
    for (int i = 0; i < m; i++)
        auxHighBits = (auxHighBits << l) | (1L << l - 1);
    DState = auxHighBits;
    std::cout << " l: " << l << std::endl;
    for (int i = 0; i < n; ++i)
    {
        DState = (DState << l) | ((1L << l - 1) - (gamma + 1));
        // DState &= ~(~1L << m * l - 1); // Mask just for DState Visualization!
        H = DState & auxHighBits;
        DState = ((DState & ~H) + BTable[t[i]]) | H;
        std::cout << "D" << t[i] << " = " << std::bitset<64>(DState & ~(~1L << m * l - 1)) << std::endl;

        if ((DState & (1L << m * l - 1)) == 0)
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
    std::cout << forwardScan(y, x, delta, gamma) << std::endl;
    return 0;
}