#pragma once

#include "ODE.hpp"
#include "Integrator.hpp"
#include "Contractor.hpp"

class EnclosureComputation {
public:
  EnclosureComputation();
  ~EnclosureComputation();

  void setIntegrator(Integrator*);

  virtual State compute(State& s0, Time& dt);

protected:
  State enlarge(State const& s);
  bool isSubset(State const& s1, State const& s2); // return true if s1 subset of s2
  Integrator* integrator;
  Contractor* contractor;
};
