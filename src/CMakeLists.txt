set (CMAKE_CXX_STANDARD 11)

set(SRCS
  main.cpp
  experimentations.cpp

  Binomial.cpp

  Enumeration.cpp

  Interval.cpp

  ODE.cpp
  SimpleODE.cpp
  SimpleSwitchingODE.cpp
  ExponentialODE.cpp
  NonLinearODE.cpp
  DubinsCarODE.cpp
  DubinsCarODEbis.cpp
  DubinsCarODEnonlinear.cpp
  ConicalPendulumODE_PD.cpp
  sliding2d.cpp

  MultiPolynomial.cpp
  TaylorModel.cpp

  Decomposer.cpp
  AffineDecomposer.cpp

  EnclosureComputation.cpp

  PolynomialExpansion.cpp

  RemainderComputation.cpp

  Lifter.cpp

  Integrator.cpp
  DecomposedIntegrator.cpp
  BasicIntegrator.cpp
  Contractor.cpp

  Solver.cpp

  Printer.cpp
  NullPrinter.cpp
  PrinterCSV.cpp
  PrinterSamples.cpp
  )

set(HEADERS
  )


add_executable(run ${SRCS} ${HEADERS})
