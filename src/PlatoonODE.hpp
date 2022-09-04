#pragma once

#include "InputAffineODE.hpp"

class PlatoonODE : public InputAffineODE {
public:
  virtual SubFunctions getHi() const override;
  virtual IntSubFunctions getIntHi() const override;

  virtual std::vector<SetsLibrary::Interval> getInitialStateDomain() const override;
  virtual std::vector<InputDomain> getInputsDomain() const override;

  virtual int getDim() const override;
  virtual std::vector<std::string> getNames() const override;

private:
  template<class Time, class State, class SubFunction>
  static std::vector<SubFunction> getGenericHi();
};
