#include "SimpleSwitchingODE.hpp"

#include <iostream>

SubFunctions SimpleSwitchingODE::getHi() const {
  SubFunction f0 = [](Time &, State & x){
                     return -x[0];
                   };
  SubFunction f1 = [](Time &, State &){ return TaylorModel(1); };

  SubFunctions fi(2);
  fi[0] = f0;
  fi[1] = f1;

  return fi;
}

SubInputFunctions SimpleSwitchingODE::getGij() const {
  SubInputFunctions gij(1,std::vector<SubInputFunction>(2, [](Inputs &){return 0;}));

  gij[0][0] = [](Inputs & u){ return 1; };
  gij[0][1] = [](Inputs & u){ return u[0]; };

  return gij;
}

StateDomain SimpleSwitchingODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(1);
  x[0] = SetsLibrary::Interval(3);
  return x;
}

InputsDomains SimpleSwitchingODE::getInputsDomains() const {
  InputsDomains u(1);
  u[0] = SetsLibrary::Interval(0,1);
  return u;
}

int SimpleSwitchingODE::getDim() const {
  return 1;
}

std::vector<std::string> SimpleSwitchingODE::getNames() const {
  return std::vector<std::string>(1,"x");
}

std::string SimpleSwitchingODE::getNameODE() const {
  return "SimpleSwitching";
}
