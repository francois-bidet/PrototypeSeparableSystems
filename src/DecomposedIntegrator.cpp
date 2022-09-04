#include "DecomposedIntegrator.hpp"

#include <iostream>
#include <assert.h>

DecomposedIntegrator::DecomposedIntegrator(int nbMax)
  : nbMax(nbMax)
{}

void DecomposedIntegrator::setDecompositions(Decompositions const& d) {
  this->decompositions = d;
}

void DecomposedIntegrator::setInputs(InputRepresentatives const& u) {
  this->u = u;
}

void DecomposedIntegrator::setNbMonomialsMax(int nbMax) {
  this->nbMax = nbMax;
}

State DecomposedIntegrator::integrate(State& s0,
                            Time& dt,
                            State& s) {
  // evaluation of decompositions
  for(Decomposition& dec : decompositions)
    dec.eval(dt,s);

  int dim = u.size();
  State result = s0;

  int dimJ = u[0].size();
  for(int i = 0; i < dim; ++i) {
    for(int j = 0; j < dimJ; ++j) {
      if(!u[i][j].uPos.isZero()) {
        TaylorModel pos = dtCoef * decompositions[j].getPos().primitive(0,unitDomain);
        result[i] += u[i][j].uPos * pos;

        TaylorModel neg = dtCoef * decompositions[j].getNeg().primitive(0,unitDomain);
        result[i] -= u[i][j].uNeg * neg;
      }
    }
    result[i] = result[i].reduce(this->nbMax);
  }

  return result;
}
