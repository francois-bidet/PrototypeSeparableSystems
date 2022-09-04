#pragma once

#include "ODE.hpp"

/*
  \dot{x} = - x u
  x(0) \in [-1,1]
  u(t) \in [1,2]
  t \in [0,0.1]
*/

class ExponentialODE  : public ODE {
public:
  virtual SubFunctions getHi() const override;
  virtual SubInputFunctions getGij() const override;

  virtual StateDomain getStateDomain() const override;
  virtual InputsDomains getInputsDomains() const override;

  virtual int getDim() const override;
  virtual std::vector<std::string> getNames() const override;
  virtual std::string getNameODE() const override;
};
