#include "PolynomialExpansion.hpp"

#include <iostream>

PolynomialExpansion::PolynomialExpansion()
  : integrator(nullptr)
{}

void PolynomialExpansion::setIntegrator(Integrator* integrator) {
  this->integrator = integrator;
}

Integrator* PolynomialExpansion::getIntegratorPointer() {
  return this->integrator;
}

bool samePoly(State const& s1, State const& s2) {
  int size = s1.size();
  for(int i = 0; i < size; ++i)
    if(!(s1[i].samePoly(s2[i])))
      return false;
  return true;
}

State PolynomialExpansion::compute(State& s0, Time& dt, int order) {

  State result = s0;

  /*
  for(int i = 0; i < order; ++i)
    result = integrator->integrate(s0, dt, result);
  */

  State prev;
  int n = 0;
  do {
    prev = result;
    //std::clog << "\tprev[0] = " << prev[0] << std::endl;
    result = integrator->integrate(s0,dt,prev);
    ++n;
    if(n>= 2*order)
      std::clog << "\t" << n << " -> " << result[0] << std::endl;
  }while(!samePoly(prev,result));
  std::clog << "\t#iter poly expansion: " << n << std::endl;

  return result;
}
