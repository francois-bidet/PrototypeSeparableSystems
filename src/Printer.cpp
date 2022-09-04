#include "Printer.hpp"

#include <sstream>

Printer::Printer(std::ostream& os)
  : os(os), offset(0)
{}

void Printer::print(State& s) {
  int dim = s.size();

  applyOffset();
  os << "---\n";
  for(int d = 0; d < dim; ++d) {
    applyOffset();
    os << "| " << getName(d+1) << " -> ";
    s[d].print(os, names);
    os << "\n";
  }
  applyOffset();
  os << "---\n";
}

void Printer::print(OverApproximation& approx) {
  os << approx.t0 << ":\n";
  this->offset++;
  this->print(approx.state);
  this->offset--;
}

void Printer::print(OverApproximations& approxs) {
  for(OverApproximation& approx : approxs)
    this->print(approx);
}

void Printer::printDomain(std::vector<SetsLibrary::Interval> const& domain) {
  int dim = domain.size();

  applyOffset();
  os << "---\n";
  for(int d = 0; d < dim; ++d) {
    applyOffset();
    os << "| " << getName(d) << " -> ";
    domain[d].print(os);
    os << "\n";
  }
  applyOffset();
  os << "---\n";
}

void Printer::setNames(std::vector<std::string> const& names) {
  this->names = names;
}

std::string Printer::getName(int i) {
  if(i < names.size())
    return names[i];

  std::stringstream ss;
  ss << "x_{" << i << "}";
  return ss.str();
}

void Printer::flush() {
  os << std::flush;
}

void Printer::applyOffset() {
  for(int i = 0; i < offset; ++i)
    os << "\t";
}

void Printer::printNames() {}
