#include "PlatoonODE.hpp"

template<class Time, class State, class SubFunction>
std::vector<SubFunction> PlatoonODE::getGenericHi() {
  SubFunction f0 = [](Time &, State & x) { // An * x
                     State dx(9);

                     dx[0] = x[1];
                     dx[1] = -x[2];
                     dx[2] = 1.6050 * x[0] + 4.8680 * x[1] - 3.5754 * x[2];

                     dx[3] = x[4];
                     dx[4] = -x[5];
                     dx[5] = 1.1936 * x[3] + 3.6258 * x[4] - 3.2396 * x[5];

                     dx[6] = x[7];
                     dx[7] = -x[8];
                     dx[8] = 1.2726 * x[6] + 3.0720 * x[7] - 3.1356 * x[8];

                     return dx;
                   };
  SubFunction f1 = [](Time &, State & x) { // (Ac - An) * x
                     State dx(9,0);

                     dx[2] = -0.8198 * x[3] + 0.4270 * x[4] - 0.0450 * x[5] - 0.1942 * x[6] + 0.3626 * x[7] - 0.0946 * x[8];

                     dx[5] = 0.8718 * x[0] + 3.8140 * x[1] - 0.0754 * x[2] -0.5950 * x[6] + 0.1294 * x[7] - 0.0796 * x[8];

                     dx[8] = 0.7132 * x[0] + 3.5730 * x[1] - 0.0964 * x[2] + 0.8472 * x[3] + 3.2568 * x[4] - 0.0876 * x[5];

                     return dx;
                   };
  SubFunction f2 = [](Time &, State & x) { // B
                     State b(9,0);
                     b[1] = 1;
                     return b;
                   };

  std::vector<SubFunction> fi(3);
  fi[0] = f0;
  fi[1] = f1;
  fi[2] = f2;

  return fi;
}

SubFunctions PlatoonODE::getHi() const {
  return getGenericHi<Time,State,SubFunction>();
}

IntSubFunctions PlatoonODE::getIntHi() const {
  return getGenericHi<IntTime,IntState,IntSubFunction>();
}

std::vector<SetsLibrary::Interval> PlatoonODE::getInitialStateDomain() const {
  std::vector<SetsLibrary::Interval> x(9,0); // initial motionless state
  return x;
}

std::vector<InputDomain> PlatoonODE::getInputsDomain() const {
  std::vector<InputDomain> u(2);
  u[0] = SetsLibrary::Interval(0,1); // alpha
  u[1] = SetsLibrary::Interval(-9,1); // acceleration
  return u;
}

int PlatoonODE::getDim() const {
  return 9;
}

std::vector<std::string> PlatoonODE::getNames() const {
  std::vector<std::string> names(9);

  names[0] = "e_1";
  names[1] = "de_1";
  names[2] = "a_1";

  names[3] = "e_2";
  names[4] = "de_2";
  names[5] = "a_2";

  names[6] = "e_3";
  names[7] = "de_3";
  names[8] = "a_3";

  return names;
}

