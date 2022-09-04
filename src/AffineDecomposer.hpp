#pragma once

#include "Decomposer.hpp"

class AffineDecomposer : public Decomposer {
  virtual Decomposition decompose(SubFunction const&, FunctionImage const&);
};
