#include "TaylorModel.hpp"

#include <iostream>
#include <limits>
#include <cmath>

int TaylorModel::order = std::numeric_limits<int>::max();

TaylorModel::TaylorModel()
  : poly(), remainder()
{}

TaylorModel::TaylorModel(double const& d)
  : poly(d), remainder()
{
  if(order < 0) {
    this->poly.setZero();
    remainder = d;
  }

}

TaylorModel::TaylorModel(MultiPolynomial const& poly, SetsLibrary::Interval const& remainder)
  : poly(poly), remainder(remainder)
{}

TaylorModel::TaylorModel(TaylorModel const& other)
  : poly(other.poly), remainder(other.remainder)
{}

void TaylorModel::setOrder(int order) {
  TaylorModel::order = order;
}

int TaylorModel::getOrder() {
  return order;
}

bool TaylorModel::isZero() const {
  return poly.isZero() && remainder.isZero();
}

MultiPolynomial TaylorModel::getPoly() const {
  return this->poly;
}

bool TaylorModel::samePoly(TaylorModel const& other) const {
  return this->poly == other.poly;
}

SetsLibrary::Interval TaylorModel::getRemainder() const {
  return this->remainder;
}

void TaylorModel::setRemainder(SetsLibrary::Interval const& r) {
  this->remainder = r;
}

TaylorModel& TaylorModel::operator=(TaylorModel const& other) {
  this->poly = other.poly;
  this->remainder = other.remainder;
  return *this;
}

TaylorModel TaylorModel::operator-() const {
  TaylorModel result;
  result.poly = - this->poly;
  result.remainder = - this->remainder;
  return result;
}

TaylorModel& TaylorModel::operator+=(TaylorModel const& other) {
  this->poly += other.poly;
  this->remainder += other.remainder;

  return *this;
}

TaylorModel& TaylorModel::operator-=(TaylorModel const& other) {
  this->poly -= other.poly;
  this->remainder -= other.remainder;

  return *this;
}

TaylorModel& TaylorModel::operator*=(TaylorModel const& other) {
  *this = *this * other;
  return *this;
}

TaylorModel TaylorModel::operator+(TaylorModel const& other) const {
  TaylorModel result = *this;
  result += other;
  return result;
}

TaylorModel TaylorModel::operator-(TaylorModel const& other) const {
  TaylorModel result = *this;
  result -= other;
  return result;
}

TaylorModel TaylorModel::operator*(TaylorModel const& other) const {
  TaylorModel result;

  result.poly = this->poly * other.poly;

  result.remainder += this->poly.enclosure() * other.remainder;
  result.remainder += this->remainder * other.poly.enclosure();
  result.remainder += this->remainder * other.remainder;

  if(order >= -1) {
    MultiPolynomial pR;
    result.poly = result.poly.truncate(order, &pR);
    result.remainder += pR.enclosure();
  }

  return result;
}

TaylorModel TaylorModel::operator/(TaylorModel const& other) const {
  return *this * (other.inverse());
}

TaylorModel& TaylorModel::operator+=(double const& d) {
  if(order >= 0)
    this->poly += d;
  else
    this->remainder += d;
  return *this;
}

TaylorModel& TaylorModel::operator-=(double const& d) {
  if(order >= 0)
    this->poly -= d;
  else
    this->remainder -= d;
  return *this;
}

TaylorModel& TaylorModel::operator*=(double const& d) {
  this->poly *= d;
  this->remainder *= d;
  return *this;
}

TaylorModel& TaylorModel::operator/=(double const& d) {
  this->poly /= d;
  this->remainder /= d;
  return *this;
}

TaylorModel TaylorModel::operator+(double const& d) const {
  TaylorModel result = *this;
  result += d;
  return result;
}

TaylorModel TaylorModel::operator-(double const& d) const {
  TaylorModel result = *this;
  result -= d;
  return result;
}

TaylorModel TaylorModel::operator*(double const& d) const {
  TaylorModel result = *this;
  result *= d;
  return result;
}

TaylorModel TaylorModel::operator/(double const& d) const {
  TaylorModel result = *this;
  result /= d;
  return result;
}

TaylorModel TaylorModel::cos() const { // naive implementation
  SetsLibrary::Interval enclosure = this->fastEnclosure();
  double mid = enclosure.getMedian();
  enclosure -= mid;

  std::vector<double> derivatives(4);
  derivatives[0] = std::cos(mid);
  derivatives[1] = -std::sin(mid);
  derivatives[2] = -derivatives[0];
  derivatives[3] = -derivatives[1];


  MultiPolynomial p0 = this->poly - mid;

  MultiPolynomial p = 0;
  int denum = 1;
  MultiPolynomial p1 = 1;
  for(int i = 0; i <= TaylorModel::order; ++i) {
    p += derivatives[i % 4] * p1 / denum;
    denum *= (i+1);
    p1 *= p0;
  }


  //MultiPolynomial p = std::cos(mid) - std::sin(mid) * p0 - std::cos(mid) * p0 * p0 / 2;

  //SetsLibrary::Interval r = std::sin(mid) * (enclosure.pow(3)) / 6;

  std::vector<SetsLibrary::Interval> intDeriv(4);
  intDeriv[0] = enclosure.cos();
  intDeriv[1] = -enclosure.sin();
  intDeriv[2] = -intDeriv[0];
  intDeriv[3] = -intDeriv[1];
  SetsLibrary::Interval r = intDeriv[(TaylorModel::order+1) % 4] * (enclosure.pow(TaylorModel::order+1)) / denum;
  return TaylorModel(p,r).truncate(order);
}

TaylorModel TaylorModel::sin() const { // implementation depending on the one of cos
  double halfpi = M_PI / 2;
  return (halfpi - *this).cos();
}

TaylorModel TaylorModel::inverse() const { // cf. "Implementation of Taylor models in CORA 2018"
  SetsLibrary::Interval box0 = this->eval();
  double c = box0.getMedian();
  TaylorModel tmCentered = *this - c;
  TaylorModel coef = - tmCentered / c;

  // compute expansion up to order using Horner scheme
  TaylorModel r = 0;
  for(int i = 0; i <= order; ++i) {
    r *= coef;
    r += 1;
  }
  r /= c;

  // Lagrange error
  SetsLibrary::Interval boxScaled = (box0 - c) / c; // B(tilde{T}) / c
  SetsLibrary::Interval boxCoef = boxScaled.pow(order+1);
  SetsLibrary::Interval boxDenum = (1 + (SetsLibrary::Interval(0,1) * boxScaled));
  boxDenum = boxDenum.pow(order+2);
  boxDenum *= c;
  SetsLibrary::Interval boxRem = boxCoef / boxDenum;
  if(order % 2 == 0)
    boxRem *= -1;
  TaylorModel rem = TaylorModel(MultiPolynomial(),boxRem);

  return r+rem;
}

TaylorModel TaylorModel::primitive(int variable, SetsLibrary::Interval const& intDomain) const {
  MultiPolynomial pE;
  MultiPolynomial p = this->poly.truncate(order-1, &pE);

  TaylorModel result;
  // result.poly = p.primitive(variable); // wrong
  result.poly = p.primitive(variable) * intDomain.getMax(); // correct
  result.remainder = (pE.enclosure() + this->remainder) * intDomain;

  return result;
}

SetsLibrary::Interval TaylorModel::fastEnclosure() const {
  SetsLibrary::Interval result = this->remainder;
  result += this->poly.fastUnitEnclosure();
  return result;
}

// SetsLibrary::Interval TaylorModel::eval(std::vector<SetsLibrary::Interval> const& domain) const {
//   SetsLibrary::Interval result = this->remainder;
//   result += this->poly.enclosure(domain);
//   return result;
// }

SetsLibrary::Interval TaylorModel::eval() const {
  SetsLibrary::Interval result = this->remainder;
  result += this->poly.enclosure();
  return result;
}

TaylorModel TaylorModel::partialUnitEval(std::vector<int> const& ids) const {
  SetsLibrary::Interval r;
  MultiPolynomial p = this->poly.partialUnitEval(ids, &r);
  r += this->remainder;
  return TaylorModel(p,r);
}

TaylorModel TaylorModel::partialEval(int id, double value) const {
  MultiPolynomial p = this->poly.partialEval(id,value);
  return TaylorModel(p, this->remainder);
}

TaylorModel TaylorModel::truncate(int order) const {
  MultiPolynomial pR;
  MultiPolynomial p = this->poly.truncate(order, &pR);
  SetsLibrary::Interval r = this->remainder + pR.enclosure();
  return TaylorModel(p,r);
}

TaylorModel TaylorModel::reduce(int nbMax) const {
  MultiPolynomial pR;
  MultiPolynomial p = this->poly.reduce(nbMax, &pR);
  SetsLibrary::Interval r = this->remainder + pR.enclosure();
  return TaylorModel(p,r);
}

TaylorModel TaylorModel::reduceThreshold(double const& threshold) const {
  MultiPolynomial pR;
  MultiPolynomial p = this->poly.reduceThreshold(threshold, &pR);
  SetsLibrary::Interval r = this->remainder + pR.enclosure();
  return TaylorModel(p,r);
}

std::ostream& TaylorModel::print(std::ostream& os) const {
  this->poly.print(os);
  os << " + ";
  this->remainder.print(os);
  return os;
}

std::ostream& TaylorModel::print(std::ostream& os, std::vector<std::string> const& names) const {
  this->poly.print(os, names);
  os << " + ";
  this->remainder.print(os);
  return os;
}

////////////////////////////////////////////////////////////////////////////////

TaylorModel operator+(double const& d, TaylorModel const& tm) {
  return tm + d;
}

TaylorModel operator-(double const& d, TaylorModel const& tm) {
  return d + (-tm);
}

TaylorModel operator*(double const& d, TaylorModel const& tm) {
  return tm * d;
}

TaylorModel cos(TaylorModel const& tm) {
  return tm.cos();
}

TaylorModel sin(TaylorModel const& tm) {
  return tm.sin();
}

std::ostream& operator<<(std::ostream& os, TaylorModel const& tm) {
  return tm.print(os);
}
