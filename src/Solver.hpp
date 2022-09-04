#pragma once

#include "Interval.hpp"
#include "ODE.hpp"
#include "Decomposer.hpp"
#include "EnclosureComputation.hpp"
#include "PolynomialExpansion.hpp"
#include "Lifter.hpp"
#include "Contractor.hpp"
#include "RemainderComputation.hpp"

#include "Integrator.hpp"
#include "BasicIntegrator.hpp"
#include "DecomposedIntegrator.hpp"

#include "OverApproximation.hpp"

#include "Printer.hpp"

// const int NB_MONOMIALS_MAX = 5; // truncate to this number of monomials before creating an extra one to handle the rest
const int NB_MONOMIALS_MAX = 3; // truncate to this number of monomials before creating an extra one to handle the rest
const int BETWEEN_STEPS_ORDER = 1;
const int NB_INTERMEDIATE_MONOMIALS_MAX = 50;

class Solver {
public:
  Solver(double dt);
  virtual ~Solver();

  void setEnclosureComputation(EnclosureComputation*);
  void setPrinter(Printer*);

  virtual void setODE(ODE* ode);
  void setDt(double);
  void setTEnd(double);
  void setOrder(int); // order of the expansion in time

  double getDt() const;

  void init(ODE* ode,
            double tEnd,
            int order,
            double t0 = 0
            );

  void setT0(double t0 = 0);

  void computeRawEnclosure();
  void computeDecompositions();
  void computePolynomialExpansion();
  void computeValidRemainder();

  virtual State step();

  OverApproximations compute();

  void setNamesForPrinter();

protected:
  EnclosureComputation* enclosureComputation;
  Decomposer* decomposer;
  PolynomialExpansion* polyExpansion;
  RemainderComputation* remainderComputation;
  Lifter* lifter;
  BasicIntegrator* bIntegrator;
  DecomposedIntegrator* dIntegrator;
  Contractor* contractorTM;
  Printer* printer;

  ODE* ode;

  double dt;
  double tEnd;
  int order;

  double t0;

  State s0;
  InputRepresentatives u;
  Time dtTM;

  std::vector<std::string> names;

  State rawEnclosure;
  Decompositions decompositions;
  State approximation;
};
