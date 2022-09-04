#pragma once

#include "Printer.hpp"

class NullPrinter : public Printer {
public:
  NullPrinter(std::ostream& os = std::clog); // used in case of non-overrided methods

  virtual void print(State&) override;
  virtual void print(OverApproximation&) override;
  virtual void print(OverApproximations&) override;

  virtual void flush() override;
};
