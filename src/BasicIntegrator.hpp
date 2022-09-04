#pragma once

#include "Integrator.hpp"
#include "ODE.hpp"

class BasicIntegrator : public Integrator {
public:
  void setODE(ODE* ode);

  virtual State integrate(State& s0, Time& dt, State& s) override;

private:
  int dimI;
  int dimJ;
  SubFunctions hi;
  std::vector<std::vector<TaylorModel> > gijInt;
};
