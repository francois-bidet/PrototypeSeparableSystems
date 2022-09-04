#pragma once

#include "Interval.hpp"
#include "Integrator.hpp"
#include "ODE.hpp"
#include "Decomposer.hpp"

struct InputRepresentative {
  TaylorModel uPos;
  TaylorModel uNeg;
};

using InputRepresentatives = std::vector<std::vector<InputRepresentative> >;

class DecomposedIntegrator : public Integrator {
public:
  DecomposedIntegrator(int nbMax = 100);

  void setDecompositions(Decompositions const&);
  void setInputs(InputRepresentatives const&);
  void setNbMonomialsMax(int nbMax);

  virtual State integrate(State& s0, Time& dt, State& s) override;

protected:
  Decompositions decompositions;
  InputRepresentatives u;
  double threshold;
  int nbMax;
};
