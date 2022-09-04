#include "SimpleODE.hpp"

#include <iostream>


SubFunctions SimpleODE::getHi() const {
  SubFunction f0 = [](Time& t, State&){ return TaylorModel(0.1 - t); };

  std::vector<SubFunction> fi(1);
  fi[0] = f0;

  return fi;
}

SubInputFunctions SimpleODE::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(1, [](Inputs&){return 0;}));

  gij[0][0] = [](Inputs& u){ return u[0]; };

  return gij;
}


StateDomain SimpleODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(1);
  x[0] = SetsLibrary::Interval(0);
  return x;
}

InputsDomains SimpleODE::getInputsDomains() const {
  InputsDomains u(1);
  u[0] = SetsLibrary::Interval(-1,1);
  return u;
}

int SimpleODE::getDim() const {
  return 1;
}

std::vector<std::string> SimpleODE::getNames() const {
  return std::vector<std::string>(1,"x");
}

std::string SimpleODE::getNameODE() const {
  return "Simple";
}
