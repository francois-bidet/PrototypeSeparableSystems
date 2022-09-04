#include "BasicIntegrator.hpp"

#include <iostream>

void BasicIntegrator::setODE(ODE* ode) {
  this->hi = ode->getHi();

  SubInputFunctions gij = ode->getGij();

  InputsDomains domains = ode->getInputsDomains();
  int dimU = domains.size();
  Inputs u(dimU);
  for(int d = 0; d < dimU; ++d)
    u[d] = TaylorModel(MultiPolynomial(), domains[d]);

  dimI = gij.size();
  dimJ = (dimI > 0) ? gij[0].size() : 0;
  this->gijInt.resize(dimI, std::vector<TaylorModel>(dimJ));
  for(int i = 0; i < dimI; ++i)
    for(int j = 0; j < dimJ; ++j) {
      this->gijInt[i][j] = gij[i][j](u);
      // std::clog << "g(" << i << "," << j << ") = ";
      // gijInt[i][j].print(std::clog);
      // std::clog << std::endl;
    }
}

State BasicIntegrator::integrate(State& s0, Time& dt, State& s) {
  State imgHi(dimJ);
  for(int d = 0; d < dimJ; ++d)
    imgHi[d] = hi[d](dt,s);

  State result = s0;
  for(int i = 0; i < dimI; ++i)
    for(int j = 0; j < dimJ; ++j) {
      //std::clog << "img = "; imgHi[j].print(std::clog); std::clog << std::endl;
      TaylorModel tm = gijInt[i][j] * imgHi[j];
      //std::clog << "tm = "; tm.print(std::clog); std::clog << std::endl;
      //std::clog << "dtDomain = [" << this->dtDomain.getMin() << "," << this->dtDomain.getMax() << "]" << std::endl;
      TaylorModel prim = tm.primitive(0, this->dtDomain);
      //std::clog << "tm.primitive = "; prim.print(std::clog); std::clog << std::endl;
      result[i] += prim;
    }

  // std::clog << "---Basic-Integration---" << std::endl;
  // std::clog << "s0 = "; s0[0].print(std::clog); std::clog << std::endl;
  // std::clog << "dt = "; dt.print(std::clog); std::clog << std::endl;
  // std::clog << "s = "; s[0].print(std::clog); std::clog << std::endl;
  // std::clog << "result = "; result[0].print(std::clog); std::clog << std::endl;

  return result;
}
