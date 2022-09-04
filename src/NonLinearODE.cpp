#include "NonLinearODE.hpp"

SubFunctions NonLinearODE::getHi() const {
  SubFunction f0 = [](Time &, State & x){ return -x[0]; };
  SubFunction f1 = [](Time &, State & x){ return -x[1]; };
  SubFunction f2 = [](Time &, State & x){ return x[0] * x[1]; };

  SubFunctions fi(3);
  fi[0] = f0;
  fi[1] = f1;
  fi[2] = f2;

  return fi;
}

SubInputFunctions NonLinearODE::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(3, [](Inputs &){return 0;}));

  gij[0][0] = [](Inputs &){ return 1; };
  gij[0][2] = [](Inputs & u){ return u[0]; };
  gij[1][1] = [](Inputs &){ return 1; };

  return gij;
}

std::vector<SetsLibrary::Interval> NonLinearODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(2);
  x[0] = SetsLibrary::Interval(1);
  x[1] = SetsLibrary::Interval(2);
  return x;
}

InputsDomains NonLinearODE::getInputsDomains() const {
  InputsDomains u(1);
  u[0] =  SetsLibrary::Interval(-1,1);
  return u;
}

int NonLinearODE::getDim() const {
  return 2;
}

std::vector<std::string> NonLinearODE::getNames() const {
  std::vector<std::string> names(2);
  names[0] = "x";
  names[1] = "y";
  return names;
}

std::string NonLinearODE::getNameODE() const {
  return "NonLinear";
}
