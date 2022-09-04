#include "DubinsCarODE.hpp"

#include <iostream>

SubFunctions DubinsCarODE::getHi() const {
  SubFunction f0 = [](Time&, State& x){ return cos(x[2]); };
  SubFunction f1 = [](Time&, State& x){ return sin(x[2]); };
  SubFunction f2 = [](Time&, State&){ return 1; };

  SubFunctions fi(3);
  fi[0] = f0;
  fi[1] = f1;
  fi[2] = f2;

  return fi;
}

SubInputFunctions DubinsCarODE::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(3,[](Inputs &){return 0;}));

  /*
    u_1   0     0
     0   u_1    0
     0    0    u_2
   */

  gij[0][0] = [](Inputs& u){ return u[0]; };
  gij[1][1] = [](Inputs& u){ return u[0]; };
  gij[2][2] = [](Inputs& u){ return u[1]; };

  return gij;
}

std::vector<SetsLibrary::Interval> DubinsCarODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(3);
  x[0] = 0;
  x[1] = 0;
  x[2] = 0;
  return x;
}

InputsDomains DubinsCarODE::getInputsDomains() const {
  InputsDomains u(2);
  u[0] = SetsLibrary::Interval(0.9,1);
  u[1] = SetsLibrary::Interval(0,1);
  return u;
}

int DubinsCarODE::getDim() const {
  return 3;
}

std::vector<std::string> DubinsCarODE::getNames() const {
  std::vector<std::string> names(5);
  names[0] = "x_1";
  names[1] = "x_2";
  names[2] = "x_3";
  return names;
}

std::string DubinsCarODE::getNameODE() const {
  return "DubinsCar";
}
