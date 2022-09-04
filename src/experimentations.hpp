#pragma once

#include <string>

#include "ODE.hpp"

void experimentationSimple();
void experimentationSimpleSwitching();
void experimentationExponential();
void experimentationNonLinear();
void experimentationDubinsCar();
void experimentationDubinsCarBis();
void experimentationDubinsCarNonlinear();
void experimentationConicalPendulumPD();
void experimentationSliding2d();

void experimentationGeneric(std::string const& msg,
                            ODE* ode,
                            double dt,
                            double tEnd,
                            int order
                            );
