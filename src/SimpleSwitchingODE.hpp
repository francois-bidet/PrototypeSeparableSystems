#pragma once

#include "ODE.hpp"

class SimpleSwitchingODE : public ODE {
public:
  virtual SubFunctions getHi() const override;
  virtual SubInputFunctions getGij() const override;

  virtual StateDomain getStateDomain() const override;
  virtual InputsDomains getInputsDomains() const override;

  virtual int getDim() const override;
  virtual std::vector<std::string> getNames() const override;
  virtual std::string getNameODE() const override;
};
