#pragma once

#include "ODE.hpp"
#include "Integrator.hpp"

class PolynomialExpansion {
public:
  PolynomialExpansion();

  virtual State compute(State& s0, Time& dt, int order);

  void setIntegrator(Integrator*);
  Integrator* getIntegratorPointer();

private:
  Integrator* integrator;
};
