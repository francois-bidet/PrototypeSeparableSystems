#pragma once

#include <vector>

#include "Interval.hpp"
#include "ODE.hpp"

using FunctionImage = SetsLibrary::Interval;
using FunctionsImages = std::vector<FunctionImage>;

class Decomposition {
public:
  using InternalFunction = std::function<TaylorModel(TaylorModel const&)>;

  void eval(Time&, State&); // to be called before getPos() or getNeg()
  TaylorModel getPos();
  TaylorModel getNeg();

  void setFunction(SubFunction const&);
  void setPos(InternalFunction const&);
  void setNeg(InternalFunction const&);

private:
  SubFunction f;
  InternalFunction pos;
  InternalFunction neg;

  TaylorModel imgPos;
  TaylorModel imgNeg;
};

using Decompositions = std::vector<Decomposition>;

class Decomposer {
public:
  virtual Decomposition decompose(SubFunction const&, FunctionImage const&) = 0;

  Decompositions decompose(SubFunctions const&, FunctionsImages const&);
};
