#pragma once

#include "ODE.hpp"

class ConicalPendulumODE_PD : public ODE {
private:
  double m;
  double l;
  double g;
  double eta;
  double J0;
  double qi;

public:
  ConicalPendulumODE_PD(double m, double l, double g, double eta, double J0, double qi);

  virtual SubFunctions getHi() const override;
  virtual SubInputFunctions getGij() const override;

  virtual StateDomain getStateDomain() const override;
  virtual InputsDomains getInputsDomains() const override;

  virtual int getDim() const override;
  virtual std::vector<std::string> getNames() const override;
  virtual std::string getNameODE() const override;
};
