#pragma once

#include <vector>
#include <list>
#include <ostream>
#include <string>
#include <map>

#include "Interval.hpp"

struct Monomial {
  double coef;
  std::vector<int> ids;
  std::vector<int> degrees;

  bool operator<(Monomial const&) const;
  int getDegree(int variable) const; // degree wrt a specific variable
  int getDegree() const; // degree of the monomial
  void incrementDegree(int variable);
};

class MultiPolynomial {
public:
  MultiPolynomial();
  MultiPolynomial(double const&);
  MultiPolynomial(Monomial const&);
  MultiPolynomial(MultiPolynomial const&);

  ~MultiPolynomial();

  static MultiPolynomial createIdentity(int variableId);

  bool isZero() const;
  void setZero(); // set to zero

  int getDim() const;
  std::vector<int> getDegrees(std::vector<int>& ids) const;

  MultiPolynomial& operator=(MultiPolynomial const&);

  bool operator<(MultiPolynomial const&) const;
  bool operator==(MultiPolynomial const&) const;

  MultiPolynomial operator-() const;

  MultiPolynomial& operator+=(MultiPolynomial const&);
  MultiPolynomial& operator-=(MultiPolynomial const&);
  MultiPolynomial& operator*=(MultiPolynomial const&);

  MultiPolynomial operator+(MultiPolynomial const&) const;
  MultiPolynomial operator-(MultiPolynomial const&) const;
  MultiPolynomial operator*(MultiPolynomial const&) const;

  MultiPolynomial& operator+=(double const&);
  MultiPolynomial& operator-=(double const&);
  MultiPolynomial& operator*=(double const&);
  MultiPolynomial& operator/=(double const&);

  MultiPolynomial operator+(double const&) const;
  MultiPolynomial operator-(double const&) const;
  MultiPolynomial operator*(double const&) const;
  MultiPolynomial operator/(double const&) const;

  MultiPolynomial primitive(int variable = 0) const;
  MultiPolynomial truncate(int order, int variable, MultiPolynomial* remainder = nullptr) const; // truncate polynomial with respect to variable "variable" up to the order "order"
  MultiPolynomial truncate(int order, MultiPolynomial* remainder = nullptr) const; // truncate polynomial up to the order "order"

  SetsLibrary::Interval fastUnitEnclosure() const; // fast enclosure with variables in [0,1]^n
  //SetsLibrary::Interval enclosure(std::vector<SetsLibrary::Interval> const& domain) const;
  SetsLibrary::Interval enclosure() const; // enclosure with variables in [0,1]^n
  MultiPolynomial changeDomain(std::vector<SetsLibrary::Interval> const& from, std::vector<SetsLibrary::Interval> const& to) const;

  MultiPolynomial partialUnitEval(std::vector<int> const& ids, SetsLibrary::Interval* remainder) const; // partial evaluation with variables ids in [0,1]^d
  MultiPolynomial partialEval(int id, double value) const;
  double eval(std::vector<double> const& values) const;

  MultiPolynomial reduce(int nbMax, MultiPolynomial* remainder) const; // limit the number of monomials to nbMax. Keep the ones with higher coefficient
  MultiPolynomial reduceThreshold(double const& threshold, MultiPolynomial* remainder) const;

  std::ostream& print(std::ostream&) const;
  std::ostream& print(std::ostream&, std::vector<std::string> const&) const;

private:
  MultiPolynomial(std::list<Monomial> const&);

  MultiPolynomial mulMonomial(Monomial const&) const;

  static MultiPolynomial changeDomainMonomial(Monomial const& m, std::vector<int> const& ids, std::vector<MultiPolynomial> const& conversion);
  static std::list<double> bernsteinCoefs(MultiPolynomial const& poly); // assume domain of poly is [0,1]^d
  static double bernsteinCoef(MultiPolynomial const& poly, std::vector<int> const& ids, std::vector<int> const& indices, std::vector<int> const& indicesMax);

  static std::map<MultiPolynomial,SetsLibrary::Interval> memEnclosures;

  std::list<Monomial> monomials;
};

MultiPolynomial operator+(double const&, MultiPolynomial const&);
MultiPolynomial operator-(double const&, MultiPolynomial const&);
MultiPolynomial operator*(double const&, MultiPolynomial const&);

std::ostream& operator<<(std::ostream&, MultiPolynomial const&);
