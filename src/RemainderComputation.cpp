#include "RemainderComputation.hpp"

#include <iostream>
#include <assert.h>

#include "Interval.hpp"
#include "myExceptions.hpp"

const SetsLibrary::Interval absGrowth(-1e-6,1e-6);
const double relGrowth(1.01);

RemainderComputation::RemainderComputation()
  : contractor(nullptr)
{}

void RemainderComputation::setContractor(Contractor* c) {
  this->contractor = c;
}

void RemainderComputation::setIntegrator(Integrator* i) {
  this->integrator = i;
}

double relDiff(State& s1, State& s2) {
  int dim = s1.size();
  double relMax = 0;
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();

    double w1 = r1.getMax() - r1.getMin();
    double w2 = r2.getMax() - r2.getMin();
    if(w2 > 0) {
      double rel = std::abs(w2 - w1) / w2;
      //std::clog << "\t\trel: " << rel << "\n";
      if(rel > relMax)
        relMax = rel;
    }
  }
  //std::clog << "\trelMax: " << relMax << std::endl;
  return relMax;
}

bool sameRemainder(State& s1, State& s2) {
  int dim = s1.size();
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();
    if(!(r1 == r2))
      return false;
  }
  return true;
}

bool isSubsetOf(State& s1, State& s2) {
  int dim = s1.size();
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();
    if(r1.getMin() < r2.getMin() || r2.getMax() < r1.getMax())
      return false;
  }
  return true;
}

State RemainderComputation::enlarge(State const& s) {
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

State setPolynomialPart(State const& enclosure, State const& prev) {
  int dim = prev.size();
  State result(dim);
  for(int d = 0; d < dim; ++d) {
    auto poly = prev[d].getPoly();
    auto pR = enclosure[d].getPoly() - poly;
    auto r = enclosure[d].getRemainder();
    if(!pR.isZero()) {
      std::clog << "\t" << d << " -> " << pR << std::endl;
      r += pR.enclosure();
    }
    result[d] = TaylorModel(poly,r);
  }
  return result;
}

State RemainderComputation::compute(State& s0, Time& dt, State& s) {
  State enclosure = s;

  State prev;
  int n = 0;
  do {
    prev = enclosure;
    enclosure = integrator->integrate(s0, dt, prev);
    enclosure = setPolynomialPart(enclosure, prev);
    ++n;
  } while(relDiff(enclosure, prev) > 1e-2);
  std::clog << "\tAlmost fixed point: " << n << std::endl;

  n = 0;
  while(!isSubsetOf(enclosure,prev)) {
    prev = enlarge(enclosure);
    enclosure = integrator->integrate(s0, dt, prev);
    enclosure = setPolynomialPart(enclosure,prev);
    ++n;
  }
  std::clog << "\tTo contraction: " << n << std::endl;

  n = 0;
  while(!sameRemainder(enclosure, prev)) {
    prev = enclosure;
    enclosure = integrator->integrate(s0, dt, prev);
    enclosure = setPolynomialPart(enclosure,prev);
    ++n;
  }
  std::clog << "\tTo almort fixed point: " << n << std::endl;

  return enclosure;
}
