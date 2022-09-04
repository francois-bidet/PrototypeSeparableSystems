#include "PrinterCSV.hpp"

PrinterCSV::PrinterCSV(std::ostream& os)
  : Printer(os)
{}

void PrinterCSV::print(State& s) {
  int dim = s.size();
  //std::vector<SetsLibrary::Interval> domain(dim+1,SetsLibrary::Interval(0,1));
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval interval = s[d].eval();
    os << "\t" << interval.getMin() << "\t" << interval.getMax();
  }
}

void PrinterCSV::print(OverApproximation& approx) {
  double tMin = approx.t0;
  double tMax = approx.t0 + approx.dt;

  os << tMin << "\t" << tMax;
  print(approx.state);
  os << "\n";
}

void PrinterCSV::print(OverApproximations& approxs) {
  os.precision(10);

  printNames();

  for(OverApproximation& approx : approxs) {
    print(approx);
  }
}

void PrinterCSV::printNames() {
  bool first = true;
  int dim = names.size();
  for(int d = 0; d < dim; ++d) {
    if(!first)
      os << "\t";
    os << names[d] << "\t" << names[d] << "2";
    first = false;
  }
  os << "\n";
}
