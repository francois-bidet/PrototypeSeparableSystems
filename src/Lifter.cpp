#include "Lifter.hpp"

#include <iostream>

TaylorModel Lifter::lift(SetsLibrary::Interval const& domain, int id) {
  double a = domain.getMin();
  double b = domain.getMax();

  TaylorModel result(a);
  if(a < b) {
    result += (b-a) * TaylorModel(MultiPolynomial::createIdentity(id));
  }

  return result;
}

State Lifter::liftState(StateDomain const& domain, int id0) {
  int dim = domain.size();

  State result(dim);
  for(int d = 0; d < dim; ++d) {
    double a = domain[d].getMin();
    double b = domain[d].getMax();

    result[d] = a;
    if(a < b) {
      TaylorModel var(MultiPolynomial::createIdentity(id0 + d)); // var \in [0,1]
      result[d] += (b-a) * var;
    }
  }

  return result;
}

InputRepresentatives Lifter::liftInputs(SubInputFunctions const& gij, InputsDomains const& domain, int id0) {
  std::vector<TaylorModel> inputs = liftState(domain, 1); // temporary variables
  //std::vector<SetsLibrary::Interval> unitDomain(domain.size(), SetsLibrary::Interval(0,1));

  int dimI = gij.size();
  int dimJ = gij[0].size();

  InputRepresentatives result(dimI, std::vector<InputRepresentative>(dimJ));

  int id = id0;
  for(int i = 0; i < dimI; ++i)
    for(int j = 0; j < dimJ; ++j) {
      TaylorModel img = gij[i][j](inputs);
      //std::clog << "img = "<< img << std::endl;
      SetsLibrary::Interval enclosure = img.eval();
      //std::clog << "enclosure = "; enclosure.print(std::clog); std::clog << std::endl;

      double a = enclosure.getMin();
      double b = enclosure.getMax();

      result[i][j].uPos = a;
      result[i][j].uNeg = a;

      if(a < b) {
        result[i][j].uPos += (b-a) * TaylorModel(MultiPolynomial::createIdentity(id));
        result[i][j].uNeg += (b-a) * TaylorModel(MultiPolynomial::createIdentity(id+1));
      }

      std::clog << "u[" << i << "][" << j << "].pos = "; result[i][j].uPos.print(std::clog); std::clog << std::endl;
      std::clog << "u[" << i << "][" << j << "].neg = "; result[i][j].uNeg.print(std::clog); std::clog << std::endl;

      id += 2;
    }

  return result;
}
