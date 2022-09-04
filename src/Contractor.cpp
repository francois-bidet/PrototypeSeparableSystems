#include "Contractor.hpp"

#include <iostream>

#include "myExceptions.hpp"

const int DEBUG = 0;

Contractor::Contractor()
  : absGrowth(-1e-6,1e-6), relGrowth(2),integrator(nullptr)
{}

void Contractor::setIntegrator(Integrator* i) {
  this->integrator = i;
}

State Contractor::contract(State& s0, Time& dt, State& s) {
  State rPrev;
  State rNext = s;

  /*
  // remove remainder
  for(TaylorModel& tm : rNext) {
    tm.setRemainder(SetsLibrary::Interval(0));
  }
  //*/

  if(DEBUG >= 1)
    std::clog << "\tfind valid" << std::endl;

  do {
    rPrev = enlarge(rNext);
    rNext = integrator->integrate(s0, dt, rPrev);
  } while(!isContracted(rPrev, rNext));

  if(DEBUG >= 1)
    std::clog << "\tcontract" << std::endl;

  int n = 0;
  do {
    n += 1;
    rPrev = rNext;
    if(DEBUG >= 10) {
      std::clog << "\tprev:" << std::endl;;
      for(TaylorModel const& tm : rPrev)
        std::clog << "\t[" << tm.getRemainder().getMin() << "," << tm.getRemainder().getMax() << "]" << std::endl;
    }
    rNext = integrator->integrate(s0, dt, rPrev);
    //std::clog << "\ttmp: [" << rNext[0].getRemainder().getMin() << "," << rNext[0].getRemainder().getMax() << "]" << std::endl;
    //rNext = intersection(rPrev,rNext);
    if(DEBUG >= 10) {
      std::clog << "\tnext:" << std::endl;;
      for(TaylorModel const& tm : rNext)
        std::clog << "\t[" << tm.getRemainder().getMin() << "," << tm.getRemainder().getMax() << "]" << std::endl;
    }
  } while(isEnoughContracted(rPrev, rNext,1e-2));
  //std::clog << n << " contractions" << std::endl;

  return rNext;
}

State Contractor::intersection(State const& s1, State const& s2) {
  int dim = s1.size();
  State result = s1;

  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();

    double a = std::max(r1.getMin(), r2.getMin());
    double b = std::min(r1.getMax(), r2.getMax());

    if(a > b)
      throw InvalidContraction();

    SetsLibrary::Interval r(a,b);

    result[d].setRemainder(r);
  }

  return result;
}

State Contractor::enlarge(State const& s) {
  int dim = s.size();
  State result = s;

  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r = result[d].getRemainder();

    double rad = (r.getMax() - r.getMin()) / 2;
    r += absGrowth;
    r += relGrowth * SetsLibrary::Interval(-rad,rad);

    result[d].setRemainder(r);
  }

  return result;
}

bool Contractor::isContracted(State const& s1, State const& s2) {
  int dim = s1.size();

  // check polynomials for fixed point
  for(int d = 0; d < dim; ++d)
    if(!(s1[d].getPoly() == s2[d].getPoly()))
      return false;

  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();

    if(!r2.isSubsetOf(r1))
      return false;
  }

  return true;
}

bool Contractor::isEnoughContracted(State const& s1, State const& s2, double const& threshold) {
  int dim = s1.size();

  // check polynomials for fixed point
  for(int d = 0; d < dim; ++d)
    if(!(s1[d].getPoly() == s2[d].getPoly()))
      return false;

  // check remainders
  double contractMax = 0;
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();

    if(!r2.isSubsetOf(r1))
      return false;

    double w1 = r1.getMax() - r1.getMin();
    if(w1 > 0) {
      double w2 = r2.getMax() - r2.getMin();
      double coef = (w1 - w2) / w1;
      if(coef < contractMax)
        contractMax = coef;
    }
  }

  return contractMax > threshold;
}
