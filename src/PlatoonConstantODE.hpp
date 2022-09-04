#pragma once

#include "PlatoonODE.hpp"

// Variation with a constant acceleration
class PlatoonConstantODE : public PlatoonODE {
public:
  PlatoonConstantODE(double acceleration = 1.0);

  virtual SubFunctions getHi() const override;
  virtual IntSubFunctions getIntHi() const override;

  virtual std::vector<InputDomain> getInputsDomain() const override;

private:
  template<class Time, class State, class SubFunction>
  std::vector<SubFunction> getGenericHi() const;

  double acc;
};
