#pragma once

#include "ODE.hpp"

class Integrator {
public:
  virtual State integrate(State& s0, Time& dt, State& s) = 0;
  void setDtDomain(SetsLibrary::Interval const& domain);

protected:
  SetsLibrary::Interval dtDomain;
  SetsLibrary::Interval unitDomain;
  double dtCoef; // coef used to correct integration on unitDomain
};
