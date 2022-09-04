#include "Integrator.hpp"

void Integrator::setDtDomain(SetsLibrary::Interval const& domain) {
  this->dtDomain = domain;
  this->unitDomain = SetsLibrary::Interval(0,1);
  this->dtCoef = domain.getMax() - domain.getMin();
}
