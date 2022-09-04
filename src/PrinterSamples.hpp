#pragma once

#include <random>

#include "Printer.hpp"

class PrinterSamples : public Printer {
public:
  PrinterSamples(std::ostream& os = std::cout, int nb = 100);

  virtual void print(State&) override;
  virtual void print(OverApproximation&) override;
  virtual void print(OverApproximations&) override;

  virtual void printNames() override;

private:
  int nb;
  std::default_random_engine rengine;
};
