#pragma once

#include "PlatoonODE.hpp"

class PlatoonNoLossODE : public PlatoonODE {
public:
  virtual SubFunctions getHi() const override;
  virtual IntSubFunctions getIntHi() const override;

  virtual std::vector<InputDomain> getInputsDomain() const override;

private:
  template<class Time, class State, class SubFunction>
  static std::vector<SubFunction> getGenericHi();
};
