#pragma once

#include "ODE.hpp"
#include "Contractor.hpp"
#include "Integrator.hpp"

class RemainderComputation {
public:
  RemainderComputation();

  void setContractor(Contractor*);
  void setIntegrator(Integrator*);

  virtual State compute(State& s0, Time& dt, State& s);

protected:
  State enlarge(State const& s);
  Contractor* contractor;
  Integrator* integrator;
};
