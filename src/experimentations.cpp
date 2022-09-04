#include "experimentations.hpp"

#include <chrono>

#include "Solver.hpp"

#include "SimpleODE.hpp"
#include "SimpleSwitchingODE.hpp"
#include "ExponentialODE.hpp"
#include "NonLinearODE.hpp"
#include "DubinsCarODE.hpp"
#include "DubinsCarODEbis.hpp"
#include "DubinsCarODEnonlinear.hpp"
#include "ConicalPendulumODE_PD.hpp"
#include "sliding2d.hpp"

#include <fstream>
#include <exception>

#include "Printer.hpp"
#include "PrinterCSV.hpp"
#include "PrinterSamples.hpp"

void experimentationGeneric(std::string const& msg,
                            ODE* ode,
                            double dt,
                            double tEnd,
                            int order
                            ) {
  std::clog << msg << std::endl;

  std::string filename = "outputs/" + ode->getNameODE() + ".dat";
  std::ofstream outputFile(filename, std::ios_base::out | std::ios_base::trunc);
  if(!outputFile.is_open()) {
    throw std::runtime_error("Unable to open \"" + filename + "\"");
  }

  Solver* solver = new Solver(dt);
  solver->setPrinter(new PrinterCSV(outputFile));
  solver->init(ode, tEnd, order);

  // save computation data
  std::string infoFilename = "outputs/" + ode->getNameODE() + ".info";
  std::ofstream infoFile(infoFilename, std::ios_base::out | std::ios_base::trunc);
  infoFile << "name: " << ode->getNameODE() << "\n";
  infoFile << "order: " << order << "\n";
  infoFile << "fixed dt: " << dt << "\n";
  infoFile << "tEnd: " << tEnd << "\n";
  infoFile << "between steps monomials numbers: " << NB_MONOMIALS_MAX << "\n";
  infoFile << "intermediate monomials numbers: " << NB_INTERMEDIATE_MONOMIALS_MAX << "\n";
  infoFile << "between steps order: " << BETWEEN_STEPS_ORDER << "\n";

  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  OverApproximations overapproxs = solver->compute();
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

  std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
  infoFile << "computation time: " << timeSpan.count() << " seconds\n";

  // add samples
  if(false) {
    std::string samplesFilename = "outputs/" + ode->getNameODE() + ".samples";
    std::ofstream samplesFile(samplesFilename, std::ios_base::out | std::ios_base::trunc);
    PrinterSamples printerSamples(samplesFile, 1000000);
    printerSamples.setNames(ode->getNames());
    printerSamples.print(overapproxs);
  }
}

void experimentationSimple() {
  std::string msg = "Experimentation: SimpleODE";
  ODE* ode = new SimpleODE();
  double dt = 0.2;
  double tEnd = dt;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationSimpleSwitching() {
  std::string msg = "Experimentation: SimpleSwitchingODE";
  ODE* ode = new SimpleSwitchingODE();
  double dt = 1e-1;
  double tEnd = 20;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}


void experimentationExponential() {
  std::string msg = "Experimentation: ExponentialODE";
  ODE* ode = new ExponentialODE();
  double dt = 5e-2;
  double tEnd = 5;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationNonLinear() { // need threshold 1e-8
  std::string msg = "Experimentation: NonLinearODE";
  ODE* ode = new NonLinearODE();
  double dt = 5e-2;
  double tEnd = 5.0;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationDubinsCar() {
  std::string msg = "Experimentation: DubinsCarODE";
  ODE* ode = new DubinsCarODE();
  double dt = 1e-2;
  double tEnd = 1.0;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationDubinsCarBis() {
  std::string msg = "Experimentation: DubinsCarODEbis";
  ODE* ode = new DubinsCarODEbis();
  double dt = 1e-2;
  double tEnd = 1.0;
  int order = 3;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationDubinsCarNonlinear() {
  std::string msg = "Experimentation: DubinsCarODEnonlinear";
  ODE* ode = new DubinsCarODEnonlinear();
  double dt = 1e-2;
  double tEnd = 1.0;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationConicalPendulumPD() {
  std::string msg = "Experimentation: ConicalPendulumPD";
  ODE* ode = new ConicalPendulumODE_PD(1,1,9.81,0.1,2e-7,0.021);
  double dt = 1e-2;
  double tEnd = 15;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}

void experimentationSliding2d() {
  std::string msg = "Experimentation: Siliding2D";
  ODE* ode = new Sliding2D();
  double dt = 0.5;
  double tEnd = 0.5;
  int order = 5;

  experimentationGeneric(msg, ode, dt, tEnd, order);

  delete ode;
}
