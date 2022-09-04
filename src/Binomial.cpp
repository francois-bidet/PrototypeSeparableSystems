#include "Binomial.hpp"

std::vector<std::vector<int> > Binomial::coefs;

int Binomial::getCoef(int n, int k) {
  if(k > n || k < 0)
    return 0;

  if(k == 0 || k == n)
    return 1;

  if(coefs.size() > n && coefs[n].size() > k)
    return coefs[n][k];

  if(coefs.size() <= n)
    coefs.resize(n+1, std::vector<int>(1, 1));

  while(coefs[n].size() <= k) {
    int k0 = coefs[n].size();
    coefs[n].push_back(getCoef(n-1,k-1) + getCoef(n-1, k));
  }

  return coefs[n][k];
}
