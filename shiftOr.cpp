#include <iostream>
#include <string>
#include <bitset>

std::string shiftOr(std::string y, std::string x)
{
   int m = x.length();

   if (m <= 0 || m > 64 || m > y.length())
   {
      std::cout << "Invalid parameters! " << std::endl;
      return "-1";
   }

   std::string answ = "";
   long S[256];
   long R = ~0;

   for (int i = 0; i <= 255; ++i)
      S[i] = ~0;
   for (int i = 0; i < m; ++i)
   {
      S[x[i]] &= ~(1L << i);
      // std::cout << x[i] << " = " << std::bitset<64>(S[x[i]]) << std::endl;
   }
   std::cout << " =========================== " << std::endl;
   for (int i = 0; i < y.length(); ++i)
   {
      R = (R << 1) | S[y[i]];
      std::cout << y[i] << " = " << std::bitset<64>(R) << std::endl;
      // std::cout << y[i] << " = " << std::bitset<16>(R & (1L << m-1))  << std::endl;
      if ((R & (1L << m - 1)) == 0)
         answ += std::to_string(i - m + 1) + " ";
   }
   return (answ != "") ? answ : "-1";
}

int main()
{
   std::string y, x;
   std::cin >> y >> x;
   std::cout << shiftOr(y, x) << std::endl;
   return 0;
}