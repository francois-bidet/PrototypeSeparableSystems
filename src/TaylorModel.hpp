#pragma once

#include <vector>
#include <ostream>

#include "Interval.hpp"
#include "MultiPolynomial.hpp"

class TaylorModel {
public:
  TaylorModel();
  TaylorModel(double const&);
  TaylorModel(MultiPolynomial const&, SetsLibrary::Interval const& remainder = SetsLibrary::Interval());
  TaylorModel(TaylorModel const&);

  static void setOrder(int); // order with respect to first variable
  static int getOrder();

  bool isZero() const;

  MultiPolynomial getPoly() const;
  bool samePoly(TaylorModel const&) const;

  SetsLibrary::Interval getRemainder() const;
  void setRemainder(SetsLibrary::Interval const&);

  TaylorModel& operator=(TaylorModel const&);

  TaylorModel operator-() const;

  TaylorModel& operator+=(TaylorModel const&);
  TaylorModel& operator-=(TaylorModel const&);
  TaylorModel& operator*=(TaylorModel const&);

  TaylorModel operator+(TaylorModel const&) const;
  TaylorModel operator-(TaylorModel const&) const;
  TaylorModel operator*(TaylorModel const&) const;
  TaylorModel operator/(TaylorModel const&) const;

  TaylorModel& operator+=(double const&);
  TaylorModel& operator-=(double const&);
  TaylorModel& operator*=(double const&);
  TaylorModel& operator/=(double const&);

  TaylorModel operator+(double const&) const;
  TaylorModel operator-(double const&) const;
  TaylorModel operator*(double const&) const;
  TaylorModel operator/(double const&) const;

  TaylorModel cos() const;
  TaylorModel sin() const;
  TaylorModel inverse() const; // should not contain zero // TODO

  TaylorModel primitive(int variable, SetsLibrary::Interval const& intDomain) const; // intDomain shoul be [0,t] with t > 0

  SetsLibrary::Interval fastEnclosure() const;
  //SetsLibrary::Interval eval(std::vector<SetsLibrary::Interval> const& domain) const;
  SetsLibrary::Interval eval() const; // eval on [0,1]^d
  TaylorModel partialUnitEval(std::vector<int> const& ids) const;
  TaylorModel partialEval(int id, double value) const;


  TaylorModel truncate(int order) const; // add all monomials with higher order to the remainder
  TaylorModel reduce(int nbMax) const; // limit the number of monomials to nbMax. Keep the ones with higher coefficient
  TaylorModel reduceThreshold(double const& threshold) const; // add smaller monomials to remainder

  std::ostream& print(std::ostream&) const;
  std::ostream& print(std::ostream& os, std::vector<std::string> const& names) const;

private:
  MultiPolynomial poly;
  SetsLibrary::Interval remainder;

  static int order;
};

TaylorModel operator+(double const&, TaylorModel const&);
TaylorModel operator-(double const&, TaylorModel const&);
TaylorModel operator*(double const&, TaylorModel const&);

TaylorModel cos(TaylorModel const&);
TaylorModel sin(TaylorModel const&);

std::ostream& operator<<(std::ostream& os, TaylorModel const&);
