#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "ODE.hpp" // definition of types
#include "OverApproximation.hpp" // definition of OverApproximations

class Printer {
public:
  Printer(std::ostream& os = std::cout);

  virtual void print(State&);
  virtual void print(OverApproximation&);
  virtual void print(OverApproximations&);
  virtual void printDomain(std::vector<SetsLibrary::Interval> const& domain);

  void setNames(std::vector<std::string> const&);

  std::string getName(int);

  virtual void flush();

  virtual void printNames();

protected:
  void applyOffset();

  std::ostream& os;
  std::vector<std::string> names;
  int offset;
};
