#include <iostream>
#include <string>
#include <cmath>

bool deltaGammaM(std::string x, std::string y, unsigned int delta, unsigned int gamma)
{
    if (x.length() != y.length())
    {
        std::cout << "Different string sizes!" << std::endl;
        return false;
    }
    unsigned int max = 0;
    unsigned int sum = 0;
    for (int i = 0; i < x.length(); ++i)
    {
        max = (std::abs(x[i] - y[i]) > max) ? std::abs(x[i] - y[i]) : max;
        sum += std::abs(x[i] - y[i]);
    }
    return max <= delta && sum <= gamma;
}

int main()
{
    std::string y, x;
    unsigned int delta, gamma;
    std::cin >> x >> y;
    std::cin >> delta >> gamma;
    std::cout << deltaGammaM(x, y, delta, gamma) << std::endl;
    return 0;
}