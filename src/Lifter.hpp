#pragma once

#include <vector>

#include "Interval.hpp"
#include "ODE.hpp" // define some types
#include "TaylorModel.hpp"
#include "DecomposedIntegrator.hpp" // InputRepresentatives

class Lifter {
public:
  virtual TaylorModel lift(SetsLibrary::Interval const& domain, int id); // create TaylorModel with variable id in [0,1]
  virtual State liftState(StateDomain const& domain, int id0 = 1);
  virtual InputRepresentatives liftInputs(SubInputFunctions const& gij, InputsDomains const& domain, int id0);
};
