#include "DubinsCarODEbis.hpp"

#include <iostream>

SubFunctions DubinsCarODEbis::getHi() const {
  SubFunction f0 = [](Time&, State& x){ return x[3]; }; // y_1
  SubFunction f1 = [](Time&, State& x){ return x[4]; }; // y_2
  SubFunction f2 = [](Time&, State&){ return 1; };

  SubFunctions fi(3);
  fi[0] = f0;
  fi[1] = f1;
  fi[2] = f2;

  return fi;
}

SubInputFunctions DubinsCarODEbis::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(3,[](Inputs &){return 0;}));

  /*
    u_1   0     0
     0   u_1    0
     0    0    u_2
     0   -u_2   0
    u_2   0     0
   */

  gij[0][0] = [](Inputs& u){ return 1; };
  gij[1][1] = [](Inputs& u){ return 1+u[0]; };
  gij[2][2] = [](Inputs& u){ return u[1]; };
  gij[3][1] = [](Inputs& u){ return -u[1]; };
  gij[4][0] = [](Inputs& u){ return u[1]; };

  return gij;
}

std::vector<SetsLibrary::Interval> DubinsCarODEbis::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(5);
  x[0] = 0;
  x[1] = 0;
  x[2] = 0;
  x[3] = 1;
  x[4] = 0;
  return x;
}

InputsDomains DubinsCarODEbis::getInputsDomains() const {
  InputsDomains u(2);
  u[0] = SetsLibrary::Interval(-5e-2,5e-2);
  u[1] = SetsLibrary::Interval(0,1);
  return u;
}

int DubinsCarODEbis::getDim() const {
  return 5;
}

std::vector<std::string> DubinsCarODEbis::getNames() const {
  std::vector<std::string> names(5);
  names[0] = "x_1";
  names[1] = "x_2";
  names[2] = "x_3";
  names[3] = "y_1";
  names[4] = "y_2";
  return names;
}

std::string DubinsCarODEbis::getNameODE() const {
  return "DubinsCarBis";
}
