#pragma once

#include <vector>

class Binomial {
public:
  static int getCoef(int n, int k);

private:
  static std::vector<std::vector<int> > coefs;
};
