#pragma once

#include "Printer.hpp"

class PrinterCSV : public Printer {
public:
  PrinterCSV(std::ostream& os = std::cout);

  virtual void print(State&) override;
  virtual void print(OverApproximation&) override;
  virtual void print(OverApproximations&) override;

  virtual void printNames() override;
};
