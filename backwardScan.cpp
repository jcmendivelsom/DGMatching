#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
std::string backwardScan(std::string t, std::string p, unsigned int delta, unsigned int gamma)
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
    long DState = 0, auxHighBits = 0, H = 0;

    // Preproccessing
    for (int i = 0; i <= 255; ++i)
    {
        BTable[i] = 0;
        for (int j = 0; j < m; ++j)
        {
            if (std::abs(i - p[j]) <= delta)
                BTable[i] = (BTable[i] << l) | (long)std::abs(i - p[j]);
            else
                BTable[i] = (BTable[i] << l) | (long)gamma + 1;
        }
    }
    // Serch Phase
    std::cout << " =========================== " << std::endl;
    std::cout << "l = " << l << std::endl;
    for (int i = 0; i < m; i++)
        auxHighBits = (auxHighBits << l) | (1L << l - 1);
    int pos = 0, j, last;
    while (pos <= n - m)
    {
        std::cout << "POS " << pos << std::endl;
        j = m;
        last = m;
        for (int i = 0; i < m; i++)
            DState = (DState << l) | ((1L << l - 1) - (gamma + 1));
        while ((DState & auxHighBits) != auxHighBits)
        {
            std::cout << "J" << j - 1 << std::endl;
            H = DState & auxHighBits;
            DState = ((DState & ~H) + BTable[t[pos + j - 1]]) | H;
            std::cout << "D " << t[pos + j - 1] << " = " << std::bitset<64>(DState & ~(~1L << m * l - 1)) << std::endl;
            j -= 1;
            if ((DState & (1L << m * l - 1)) == 0)
            {
                // std::cout << "Cambio Last" << std::endl;
                if (j > 0)
                {
                    last = j;
                }
                else
                {
                    answ += std::to_string(pos) + " ";
                    // std::cout << pos << std::endl;
                }
            }
            DState = (DState << l) | (1L << l - 1);
            // DState &= ~(~1L << m * l - 1); // Mask just for DState Visualization!
        }
        pos += last;
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
    std::cout << backwardScan(y, x, delta, gamma) << std::endl;
    return 0;
}