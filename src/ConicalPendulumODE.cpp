#include "ConicalPendulumODE.hpp"

#include <iostream>
#include <cmath>

ConicalPendulumODE::ConicalPendulumODE(double m, double l, double g, double eta, double J0, double qi)
  : m(m), l(l), g(g), eta(eta), J0(J0), qi(qi)
{}

SubFunctions ConicalPendulumODE::getHi() const {
  /*
  TaylorModel& p = x[0];
  TaylorModel& dp = x[1];
  TaylorModel& q = x[2];
  TaylorModel& dq = x[3];

  TaylorModel sinq = sin(q);
  TaylorModel cosq = cos(q);

  TaylorModel J = J0 + (m * l * l * sinq * sinq);
  */

  SubFunction f0 = [](Time&, State& x){ return x[1]; };
  SubFunction f1 = [](Time&, State& x){
		     TaylorModel& p = x[0];
		     TaylorModel& dp = x[1];
		     TaylorModel& q = x[2];
		     TaylorModel& dq = x[3];

		     TaylorModel sinq = sin(q);
		     TaylorModel cosq = cos(q);

		     TaylorModel taup = 0; // no control

		     TaylorModel J = J0 + (m * l * l * sinq * sinq);
		     TaylorModel dJ = 2 * m * l * l * sinq * cosq * dq;

		     return (taup - (dJ + (eta * dp * sinq)) * dp) / J; // assume tau_p = 0 (no control)
		   };
  SubFunction f2 = [](Time&, State& x){ return x[3]; };
  SubFunction f3 = [](Time&, State& x){
		     TaylorModel& p = x[0];
		     TaylorModel& dp = x[1];
		     TaylorModel& q = x[2];
		     TaylorModel& dq = x[3];

		     TaylorModel sinq = sin(q);
		     TaylorModel cosq = cos(q);

		     return sinq * ((dp * dp * cosq) - (g / l));
		   };
  SubFunction f4 = [](Time&, State& x){ return sin(x[0]); };
  SubFunction f5 = [](Time&, State&){ return 1; };

  SubFunctions fi(6);
  fi[0] = f0;
  fi[1] = f1;
  fi[2] = f2;
  fi[3] = f3;
  fi[4] = f4;
  fi[5] = f5;

  return fi;
}

SubInputFunctions ConicalPendulumODE::getGij() const {
  SubInputFunctions gij(this->getDim(),std::vector<SubInputFunction>(6,[](Inputs &){return 0;}));

  /*
     1    0    0    0    0    0
     0    1    0    0  deltap 0
     0    0    1    0    0    0
     0    0    0    1    0  deltaq
   */

  gij[0][0] = [](Inputs& u){ return 1; };
  gij[1][1] = [](Inputs& u){ return 1; };
  gij[2][2] = [](Inputs& u){ return 1; };
  gij[3][3] = [](Inputs& u){ return 1; };
  gij[1][4] = [](Inputs& u){ return u[0]; };
  gih[3][5] = [](Inputs& u){ return u[1]; };

  return gij;
}

std::vector<SetsLibrary::Interval> ConicalPendulumODE::getStateDomain() const {
  std::vector<SetsLibrary::Interval> x(4);
  x[0] = 0;
  x[1] = 0;
  x[2] = qi;
  x[3] = sqrt(g / (l * cos(qi)));
  return x;
}

InputsDomains ConicalPendulumODE::getInputsDomains() const {
  double ampDeltaP = 0.08;
  double ampDeltaQ = 0.4;
  InputsDomains u(2);
  u[0] = SetsLibrary::Interval(-ampDeltaP,ampDeltaP);
  u[1] = SetsLibrary::Interval(-ampDeltaQ,ampDeltaQ);
  return u;
}

int ConicalPendulumODE::getDim() const {
  return 4;
}

std::vector<std::string> ConicalPendulumODE::getNames() const {
  std::vector<std::string> names(4);
  names[0] = "p";
  names[1] = "dp";
  names[2] = "q";
  names[3] = "dq";
  return names;
}

std::string ConicalPendulumODE::getNameODE() const {
  return "ConicalPendulum";
}
