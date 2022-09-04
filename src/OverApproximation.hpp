#pragma once

#include <vector>

#include "ODE.hpp"

struct OverApproximation {
  State state;
  double t0;
  double dt;
};

using OverApproximations = std::vector<OverApproximation>;
