#pragma once

#include "ODE.hpp"

/*
 * \dot{x} = -x + x y u
 * \dot{y} = - y
 * x(0) = 1
 * y(0) = 2
 * u(t) \in [-1,1]
 */

class NonLinearODE : public ODE {
public:
  virtual SubFunctions getHi() const override;
  virtual SubInputFunctions getGij() const override;

  virtual StateDomain getStateDomain() const override;
  virtual InputsDomains getInputsDomains() const override;

  virtual int getDim() const override;
  virtual std::vector<std::string> getNames() const override;
  virtual std::string getNameODE() const override;
};
