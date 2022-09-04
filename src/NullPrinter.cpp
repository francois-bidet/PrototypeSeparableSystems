#include "NullPrinter.hpp"

NullPrinter::NullPrinter(std::ostream& os)
  : Printer(os)
{}

void NullPrinter::print(State&) {}
void NullPrinter::print(OverApproximation&) {}
void NullPrinter::print(OverApproximations&) {}

void NullPrinter::flush() {}
