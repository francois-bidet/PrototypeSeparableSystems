#include "PrinterSamples.hpp"



PrinterSamples::PrinterSamples(std::ostream& os, int nb)
  : Printer(os), nb(nb), rengine()
{}

void PrinterSamples::print(State& s) {
  int dim = s.size();

  // print Taylor models in standard logging output
  for(int i = 0; i < dim; ++i) {
    std::clog << i << " -> " << s[i] << std::endl;
  }

  // compute the number of variables in
  int nbVar = 0;
  for(TaylorModel const& tm : s) {
    int n = tm.getPoly().getDim();
    if(n > nbVar)
      nbVar = n;
  }

  // samples
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  std::bernoulli_distribution binDist(0.5);
  for(int i = 0; i < this->nb; ++i) {
    // pick random point
    std::vector<double> point;
    for(int k = 0; k < nbVar; ++k)
      point.push_back(distribution(rengine));

    // get image of the random point
    std::vector<double> image;
    for(int k = 0; k < dim; ++k) {
      double v = s[k].getPoly().eval(point); // image of the polynomial
      // add bound of the remainder
      if(binDist(rengine))
	v += s[k].getRemainder().getMax();
      else
	v += s[k].getRemainder().getMin();
      image.push_back(v);
    }

    // print image
    os << image[0];
    for(int k = 1; k < dim; ++k) {
      os << "\t" << image[k];
    }
    os << "\n";
  }
}

void PrinterSamples::print(OverApproximation& approx) {
  print(approx.state);
}

void PrinterSamples::print(OverApproximations& approxs) {
  os.precision(10);

  printNames();

  print(approxs.back()); // print only last approximation
}

void PrinterSamples::printNames() {
  bool first = true;
  int dim = names.size();
  for(int d = 0; d < dim; ++d) {
    if(!first)
      os << "\t";
    os << names[d];
    first = false;
  }
  os << "\n";
}
