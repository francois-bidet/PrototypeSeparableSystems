#include <iostream>
#include <vector>
#include <functional>

#include "experimentations.hpp"

int main(int argc, char** argv) {
  //*
  std::ios_base::sync_with_stdio(false); // speedup iostreams (stdio not allowed)
  std::cin.tie(NULL); // speedup iostreams (stdio not allowed)
  //*/

  if(argc == 1) {
    std::clog << "Usage: " << argv[0] << " experiment"
	      << "\nwhere experiment = "
	      << "\n\t0 : all experiments"
	      << "\n\t1 : Simple"
	      << "\n\t2 : SimpleSwitching"
	      << "\n\t3 : Exponential"
	      << "\n\t4 : NonLinear"
	      << "\n\t5 : DubinsCar"
	      << std::endl;
  }
  else {
    std::vector<std::function<void()> > experiments;
    experiments.push_back(experimentationSimple);
    experiments.push_back(experimentationSimpleSwitching);
    experiments.push_back(experimentationExponential);
    experiments.push_back(experimentationNonLinear);
    experiments.push_back(experimentationDubinsCar);

    int selector = std::stoi(argv[1]);
    if(selector <= 0) {
      for(std::function<void()>& f : experiments)
	f();
    }
    else {
      experiments[selector-1]();
    }
  }

  return 0;
}
