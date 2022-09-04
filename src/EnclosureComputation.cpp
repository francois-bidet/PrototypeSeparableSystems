#include "EnclosureComputation.hpp"

#include <iostream>
#include <assert.h>

#include "Interval.hpp"
#include "myExceptions.hpp"

const SetsLibrary::Interval absGrowth(-1e-6,1e-6);
const double relGrowth(2);

EnclosureComputation::EnclosureComputation()
  : integrator(nullptr)
{
  this->contractor = new Contractor();
}

EnclosureComputation::~EnclosureComputation()
{
  delete this->contractor;
}

void EnclosureComputation::setIntegrator(Integrator* i) {
  this->integrator = i;
  this->contractor->setIntegrator(i);
}

State EnclosureComputation::compute(State& s0, Time& dt) {
  int prevOrder = TaylorModel::getOrder();
  // TaylorModel::setOrder(-1); // old version
  TaylorModel::setOrder(0); // new version

  int dim = s0.size();
  State s0Int(dim);
  for(int d = 0; d < dim; ++d)
    //s0Int[d] = TaylorModel(MultiPolynomial(), s0[d].fastEnclosure());
    s0Int[d] = TaylorModel(MultiPolynomial(), s0[d].eval());

  // enlarge until contraction
  State enclosure = s0Int;
  State prev;
  Time dtInt(MultiPolynomial(), dt.eval());

  // std::clog << "s0" << std::endl;
  // s0Int[0].print(std::clog);
  // std::clog << std::endl;
  // std::clog << "dt" << std::endl;
  // dtInt.print(std::clog);
  // std::clog << std::endl;
  // do {
  //   prev = enlarge(enclosure);
  //   enclosure = integrator->integrate(s0Int,dtInt,prev);
  // }while(!isSubset(enclosure,prev));


  enclosure = this->contractor->contract(s0Int,dtInt,enclosure);

  TaylorModel::setOrder(prevOrder);

  return enclosure;
}

State EnclosureComputation::enlarge(State const& s) {
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

bool EnclosureComputation::isSubset(State const& s1, State const& s2) {
  int dim = s1.size();
  for(int d = 0; d < dim; ++d) {
    SetsLibrary::Interval r1 = s1[d].getRemainder();
    SetsLibrary::Interval r2 = s2[d].getRemainder();

    if(!r1.isSubsetOf(r2))
      return false;
  }

  return true;
}
