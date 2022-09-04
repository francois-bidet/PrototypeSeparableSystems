#include "ExponentialODE.hpp"

#include <iostream>

SubFunctions ExponentialODE::getHi() const {
  SubFunction f0 = [](Time &, State & x){ return  -x[0]; };

  std::vector<SubFunction> fi(1);
  fi[0] = f0;

  return fi;
}

SubInputFunctions ExponentialODE::getGij() const {
  SubInputFunctions gij(1,std::vector<SubInputFunction>(1, [](Inputs &){return 0;}));

  gij[0][0] = [](Inputs & u){ return u[0]; };

  return gij;
}


StateDomain ExponentialODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(1);
  //x[0] = SetsLibrary::Interval(-1,1);
  x[0] = SetsLibrary::Interval(1,1.1);
  return x;
}

InputsDomains ExponentialODE::getInputsDomains() const {
  InputsDomains u(1);
  u[0] = SetsLibrary::Interval(1,2);
  return u;
}

int ExponentialODE::getDim() const {
  return 1;
}

std::vector<std::string> ExponentialODE::getNames() const {
  return std::vector<std::string>(1,"x");
}

std::string ExponentialODE::getNameODE() const {
  return "Exponential";
}
