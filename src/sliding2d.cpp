#include "sliding2d.hpp"

SubFunctions Sliding2D::getHi() const {
  SubFunction f0 = [](Time &, State &){ return -1; };
  SubFunction f1 = [](Time &, State &){ return 1; };

  SubFunctions fi(2);
  fi[0] = f0;
  fi[1] = f1;

  return fi;
}

SubInputFunctions Sliding2D::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(2, [](Inputs &){return 0;}));

  gij[1][0] = [](Inputs &){ return 1; };
  gij[0][1] = [](Inputs & u){ return u[0]; };
  gij[1][1] = [](Inputs & u){ return u[0]; };

  return gij;
}

std::vector<SetsLibrary::Interval> Sliding2D::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(2);
  x[0] = SetsLibrary::Interval(-0.7,-0.5);
  x[1] = SetsLibrary::Interval(0.3,0.5);
  return x;
}

InputsDomains Sliding2D::getInputsDomains() const {
  InputsDomains u(1);
  u[0] =  SetsLibrary::Interval(0,1);
  return u;
}

int Sliding2D::getDim() const {
  return 2;
}

std::vector<std::string> Sliding2D::getNames() const {
  std::vector<std::string> names(2);
  names[0] = "x";
  names[1] = "y";
  return names;
}

std::string Sliding2D::getNameODE() const {
  return "Sliding2D";
}
