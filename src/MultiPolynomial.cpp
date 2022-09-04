#include "MultiPolynomial.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>

#include "Enumeration.hpp"
#include "Binomial.hpp"

std::map<MultiPolynomial,SetsLibrary::Interval> MultiPolynomial::memEnclosures;

bool Monomial::operator<(Monomial const& other) const {
  int tDim = this->degrees.size();
  int oDim = other.degrees.size();

  int iT = 0;
  int iO = 0;

  while(iT < tDim && iO < oDim) {
    if(this->ids[iT] < other.ids[iO])
      return true;
    if(this->ids[iT] > other.ids[iO])
      return false;
    if(this->degrees[iT] < other.degrees[iO])
      return true;
    if(this->degrees[iT] > other.degrees[iO])
      return false;
    ++iT;
    ++iO;
  }

  return iO < oDim;
}

int Monomial::getDegree(int variable) const {
  std::vector<int>::const_iterator it = std::find(ids.begin(), ids.end(), variable);
  if(it == ids.end())
    return 0;

  return degrees[std::distance(ids.begin(), it)];
}

int Monomial::getDegree() const {
  int degree = 0;
  for(int d : degrees)
    degree += d;
  return degree;
}

void Monomial::incrementDegree(int variable) {
  std::vector<int>::iterator it = std::lower_bound(ids.begin(), ids.end(), variable);
  std::vector<int>::iterator itD = std::next(degrees.begin(), std::distance(ids.begin(), it));
  if(it == ids.end() || *it != variable) {
    ids.insert(it,variable);
    degrees.insert(itD,1);
  }
  else {
    ++(*itD);
  }
}

////////////////////////////////////////////////////////////////////////////////

MultiPolynomial::MultiPolynomial()
  : monomials()
{}

MultiPolynomial::MultiPolynomial(double const& d)
  : monomials()
{
  if(d != 0) {
    Monomial m;
    m.coef = d;
    monomials.push_back(m);
  }
}

MultiPolynomial::MultiPolynomial(Monomial const& m)
  : monomials()
{
  if(m.coef != 0)
    this->monomials.push_back(m);
}

MultiPolynomial::MultiPolynomial(std::list<Monomial> const& monomials)
  : monomials(monomials)
{}

MultiPolynomial::MultiPolynomial(MultiPolynomial const& other)
  : monomials(other.monomials)
{}

MultiPolynomial::~MultiPolynomial()
{}

bool MultiPolynomial::isZero() const {
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it)
    if(it->coef != 0.0)
      return false;
  return true;
}

void MultiPolynomial::setZero() {
  this->monomials.clear();
}

int MultiPolynomial::getDim() const {
  int dim = 0;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    if(!(it->ids.empty())) {
      int mDim = it->ids.back();
      if(dim < mDim)
	dim = mDim;
    }
  }
  return dim;
}

std::vector<int> MultiPolynomial::getDegrees(std::vector<int>& ids) const {
  std::vector<int> degrees;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    int size = it->ids.size();
    for(int i = 0; i < size; ++i) {
      int id = it->ids[i];
      int deg = it->degrees[i];
      auto itId = std::lower_bound(ids.begin(), ids.end(), id);
      auto itDeg = std::next(degrees.begin(), std::distance(ids.begin(), itId));
      if(itId == ids.end() || *itId != id) {
        ids.insert(itId,id);
        degrees.insert(itDeg,deg);
      }
      else if(*itDeg < deg) {
        *itDeg = deg;
      }
    }
  }
  return degrees;
}

MultiPolynomial& MultiPolynomial::operator=(MultiPolynomial const& other) {
  this->monomials = other.monomials;
  return *this;
}

bool MultiPolynomial::operator<(MultiPolynomial const& other) const {
  auto itT = this->monomials.begin();
  auto itO = other.monomials.begin();
  while(itT != this->monomials.end() && itO != other.monomials.end()) {
    if(*itT < *itO)
      return true;
    if(*itO < *itT)
      return false;
    if(itT->coef < itO->coef)
      return true;
    if(itO->coef < itT->coef)
      return false;
    ++itT;
    ++itO;
  }
  return itO != other.monomials.end();
}

bool MultiPolynomial::operator==(MultiPolynomial const& other) const {
  const int DEBUG = 0;

  if(this->monomials.size() != other.monomials.size()) {
    if(DEBUG)
      std::clog << "not same size : " << this->monomials.size() << " != " << other.monomials.size()  << std::endl;
    return false;
  }

  auto itT = this->monomials.begin();
  auto itO = other.monomials.begin();
  while(itT != this->monomials.end()) {
    if(itT->coef != itO->coef) {
      if(DEBUG)
        std::clog << "coef" << std::endl;
      return false;
    }
    if(itT->ids.size() != itO->ids.size()) {
      if(DEBUG)
        std::clog << "ids size" << std::endl;
      return false;
    }
    int size = itT->ids.size();
    for(int i = 0; i < size; ++i)
      if(itT->ids[i] != itO->ids[i] || itT->degrees[i] != itO->degrees[i]) {
        if(DEBUG)
          std::clog << "(" << itT->ids[i] << "," << itT->degrees[i] << ")"
                    << " != "
                    << "(" << itO->ids[i] << "," << itO->degrees[i] << ")"
                    << std::endl;
        return false;
      }

    ++itT;
    ++itO;
  }

  return true;
}

MultiPolynomial MultiPolynomial::operator-() const {
  MultiPolynomial result;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    Monomial m;
    m.coef = - it->coef;
    m.ids = it->ids;
    m.degrees = it->degrees;
    result.monomials.push_back(m);
  }
  return result;
}

MultiPolynomial& MultiPolynomial::operator+=(MultiPolynomial const& other) {
  *this = *this + other;
  return *this;
}

MultiPolynomial& MultiPolynomial::operator-=(MultiPolynomial const& other) {
  *this = *this - other;
  return *this;
}


MultiPolynomial& MultiPolynomial::operator*=(MultiPolynomial const& other) {
  *this = *this * other;
  return *this;
}

MultiPolynomial MultiPolynomial::operator+(MultiPolynomial const& other) const {
  MultiPolynomial result;

  auto tIt = this->monomials.begin();
  auto oIt = other.monomials.begin();

  while(tIt != this->monomials.end() && oIt != other.monomials.end()) {
    if(*tIt < *oIt) {
      result.monomials.push_back(*tIt);
      ++tIt;
    }
    else if(*oIt < *tIt) {
      result.monomials.push_back(*oIt);
      ++oIt;
    }
    else {
      Monomial m;
      m.coef = tIt->coef + oIt->coef;
      if(m.coef != 0) {
        m.ids = tIt->ids;
        m.degrees = tIt->degrees;
        result.monomials.push_back(m);
      }
      ++tIt;
      ++oIt;
    }
  }

  while(tIt != this->monomials.end()) {
    result.monomials.push_back(*tIt);
    ++tIt;
  }

  while(oIt != other.monomials.end()) {
    result.monomials.push_back(*oIt);
    ++oIt;
  }

  return result;
}



MultiPolynomial MultiPolynomial::operator-(MultiPolynomial const& other) const {
  MultiPolynomial result;

  auto tIt = this->monomials.begin();
  auto oIt = other.monomials.begin();

  while(tIt != this->monomials.end() && oIt != other.monomials.end()) {
    if(*tIt < *oIt) {
      result.monomials.push_back(*tIt);
      ++tIt;
    }
    else if(*oIt < *tIt) {
      Monomial m;
      m.coef = - oIt->coef;
      m.ids = oIt->ids;
      m.degrees = oIt->degrees;
      result.monomials.push_back(m);
      ++oIt;
    }
    else {
      Monomial m;
      m.coef = tIt->coef - oIt->coef;
      if(m.coef != 0) {
        m.ids = tIt->ids;
        m.degrees = tIt->degrees;
        result.monomials.push_back(m);
      }
      ++tIt;
      ++oIt;
    }
  }

  while(tIt != this->monomials.end()) {
    result.monomials.push_back(*tIt);
    ++tIt;
  }

  while(oIt != other.monomials.end()) {
    Monomial m;
    m.coef = - oIt->coef;
    m.ids = oIt->ids;
    m.degrees = oIt->degrees;
    result.monomials.push_back(m);
    ++oIt;
  }

  return result;
}

MultiPolynomial MultiPolynomial::operator*(MultiPolynomial const& other) const {
  MultiPolynomial result;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    result += other.mulMonomial(*it);
  }
  return result;
}

MultiPolynomial& MultiPolynomial::operator+=(double const& d) {
  return *this += MultiPolynomial(d);
}

MultiPolynomial& MultiPolynomial::operator-=(double const& d) {
  return *this += -d;
}

MultiPolynomial& MultiPolynomial::operator*=(double const& d) {
  if(d == 0)
    this->setZero();
  else
    for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
      it->coef *= d;
    }
  return *this;
}

MultiPolynomial& MultiPolynomial::operator/=(double const& d) {
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    it->coef /= d;
  }
  return *this;
}

MultiPolynomial MultiPolynomial::operator+(double const& d) const {
  return *this + MultiPolynomial(d);
}

MultiPolynomial MultiPolynomial::operator-(double const& d) const {
  return *this - MultiPolynomial(d);
}

MultiPolynomial MultiPolynomial::operator*(double const& d) const {
  MultiPolynomial result;
  if(d != 0)
    for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
      Monomial m;
      m.coef = it->coef * d;
      m.ids = it->ids;
      m.degrees = it->degrees;
      result.monomials.push_back(m);
    }
  return result;
}

MultiPolynomial MultiPolynomial::operator/(double const& d) const {
  MultiPolynomial result;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    Monomial m;
    m.coef = it->coef / d;
    m.ids = it->ids;
    m.degrees = it->degrees;
    result.monomials.push_back(m);
  }
  return result;
}

MultiPolynomial MultiPolynomial::primitive(int variable) const {
  MultiPolynomial result;

  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    if(it->coef != 0) {
      Monomial m;
      m.ids = it->ids;
      m.degrees = it->degrees;
      m.incrementDegree(variable);
      m.coef = it->coef / m.getDegree(variable);

      result.monomials.push_back(m);
    }
  }
  return result;
}

MultiPolynomial MultiPolynomial::truncate(int order, int variable, MultiPolynomial* remainder) const {
  MultiPolynomial result;

  if(remainder != nullptr)
    remainder->setZero();

  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    int degree = it->getDegree(variable);
    if(degree <= order) {
      result.monomials.push_back(*it);
    }
    else if(remainder != nullptr) {
      remainder->monomials.push_back(*it);
    }
  }

  return result;
}

MultiPolynomial MultiPolynomial::truncate(int order, MultiPolynomial* remainder) const {
  MultiPolynomial result;

  if(remainder != nullptr)
    remainder->setZero();

  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    int degree = it->getDegree();
    if(degree <= order) {
      result.monomials.push_back(*it);
    }
    else if(remainder != nullptr) {
      remainder->monomials.push_back(*it);
    }
  }

  return result;
}

SetsLibrary::Interval MultiPolynomial::fastUnitEnclosure() const {
  SetsLibrary::Interval r(0);
  for(Monomial const& m : monomials) {
    if(m.degrees.empty())
      r += m.coef;
    else if(m.coef > 0)
      r += SetsLibrary::Interval(0,m.coef);
    else
      r += SetsLibrary::Interval(m.coef,0);
  }
  return r;
}

// SetsLibrary::Interval MultiPolynomial::enclosure(std::vector<SetsLibrary::Interval> const& domain) const {
//   using namespace SetsLibrary;
//   Interval result;

//   enum Method{INTERVAL, BERNSTEIN};

//   const Method method = BERNSTEIN;

//   switch(method) {
//   case INTERVAL:
//     for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
//       Interval r(1.);

//       int dim = it->degrees.size();
//       for(int d = 0; d < dim; ++d)
//         r *= domain[d].pow(it->degrees[d]);

//       r *= it->coef;

//       result += r;
//     }
//     break;

//   case BERNSTEIN:
//     // std::vector<Interval> normedDomain(domain.size(), Interval(0.0, 1.0));
//     // MultiPolynomial normalized = this->changeDomain(domain, normedDomain);
//     MultiPolynomial const& normalized = *this; // already in [0,1]^n
//     std::list<double> bernstein = bernsteinCoefs(normalized);
//     double min = bernstein.front();
//     double max = min;
//     for(double const& v : bernstein) {
//       if(min > v)
//         min = v;
//       if(max < v)
//         max = v;
//     }
//     result = Interval(min,max);
//   }

//   return result;
// }

SetsLibrary::Interval MultiPolynomial::enclosure() const {
  using namespace SetsLibrary;

  auto it = memEnclosures.find(*this);
  if(it != memEnclosures.end())
    return it->second;

  std::list<double> bernstein = bernsteinCoefs(*this);
  double min = bernstein.front();
  double max = min;
  for(double const& v : bernstein) {
    if(min > v)
      min = v;
    if(max < v)
      max = v;
  }
  Interval result(min,max);

  memEnclosures.insert(std::pair<MultiPolynomial,Interval>(*this,result));
  return result;
}


MultiPolynomial MultiPolynomial::changeDomain(std::vector<SetsLibrary::Interval> const& from, std::vector<SetsLibrary::Interval> const& to) const {
  int dim = from.size();

  std::vector<MultiPolynomial> conversion(dim);
  for(int d = 0; d < dim; ++d) {
    double a1 = from[d].getMin();
    double b1 = from[d].getMax();
    double a2 = to[d].getMin();
    double b2 = to[d].getMax();

    double offset;
    double slope;
    if(b2 - a2 == 0) { // assume (b1 - a1 == 0)
      offset = a1 - a2;
      slope = 1;
    }
    else {
      offset = (a1 * b2 - a2 * b1) / (b2 - a2);
      slope = (b1 - a1) / (b2 - a2);
    }

    conversion[d] = offset + slope * MultiPolynomial::createIdentity(d);
  }

  MultiPolynomial result;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it)
    result += changeDomainMonomial(*it, std::vector<int>(), conversion);

  return result;
}

MultiPolynomial MultiPolynomial::partialUnitEval(std::vector<int> const& ids, SetsLibrary::Interval* remainder) const {
  MultiPolynomial result(0);
  MultiPolynomial pR(0);
  for(Monomial const& m : monomials) {
    bool changed = false;
    Monomial newM;
    newM.coef = m.coef;
    newM.ids = m.ids;
    newM.degrees = m.degrees;

    int iId = 0;
    for(int iDeg = 0; iDeg < m.ids.size(); ++iDeg) {
      while(iId < ids.size() && ids[iId] < m.ids[iDeg])
        ++iId;

      if(iId < ids.size() && ids[iId] == m.ids[iDeg]) { // partial eval
        changed = true;
        newM.degrees[iDeg] = 0;
      }
    }

    // remove null degrees
    auto itI = newM.ids.begin();
    auto itD = newM.degrees.begin();
    while(itI != newM.ids.end()) {
      if(*itD == 0) {
        itI = newM.ids.erase(itI);
        itD = newM.degrees.erase(itD);
      }
      else {
        ++itI;
        ++itD;
      }
    }

    if(changed) {
      double half = m.coef / 2;
      newM.coef = half;

      if(remainder != nullptr) {
        pR += MultiPolynomial(m);
        pR -= MultiPolynomial(newM);
      }
    }

    //result.monomials.push_back(newM);
    result += MultiPolynomial(std::list<Monomial>(1,newM));
  }
  if(remainder != nullptr)
    *remainder = pR.enclosure();
  return result;
}

MultiPolynomial MultiPolynomial::partialEval(int id, double value) const {
  MultiPolynomial result;

  for(Monomial const& m : monomials) {
    int deg = m.getDegree(id);
    if(deg == 0)
      result.monomials.push_back(m);
    else {
      Monomial newM;
      newM.coef = m.coef;
      newM.ids = m.ids;
      newM.degrees = m.degrees;
      auto itI = std::find(newM.ids.begin(), newM.ids.end(), id);
      newM.degrees.erase(std::next(newM.degrees.begin(),std::distance(newM.ids.begin(),itI)));
      newM.ids.erase(itI);
      newM.coef *= std::pow(value, deg);
      result += MultiPolynomial(std::list<Monomial>(1,newM));
    }
  }

  return result;
}

double MultiPolynomial::eval(std::vector<double> const& values) const {
  double result = 0;
  for(Monomial const& m : monomials) {
    double r = m.coef;
    int n = m.ids.size();
    for(int i = 0; i < n; ++i)
      r *= std::pow(values[m.ids[i]],m.degrees[i]);
    result += r;
  }
  return result;
}

MultiPolynomial MultiPolynomial::reduce(int nbMax, MultiPolynomial* remainder) const {
  if(this->monomials.size() <= nbMax)
    return *this;

  std::list<Monomial> sortedMonomials = this->monomials;
  sortedMonomials.sort([](Monomial const& m1, Monomial const& m2){
                         if(m2.degrees.empty())
                           return false;
                         if(m1.degrees.empty())
                           return true;
                         return std::abs(m1.coef) > std::abs(m2.coef);
                       }); // sort with bigger coef first
  MultiPolynomial result;
  for(int i = 0; i < nbMax; ++i) {
    result.monomials.push_back(sortedMonomials.front());
    sortedMonomials.pop_front();
  }
  result.monomials.sort(); // restore correct order

  if(remainder != nullptr) {
    remainder->monomials = sortedMonomials;
    remainder->monomials.sort(); // restore correct order
  }

  return result;
}

MultiPolynomial MultiPolynomial::reduceThreshold(double const& threshold, MultiPolynomial* remainder) const {
  MultiPolynomial result;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    if(std::abs(it->coef) < threshold) {
      if(remainder != nullptr)
        remainder->monomials.push_back(*it);
    }
    else {
      result.monomials.push_back(*it);
    }
  }
  return result;
}

std::ostream& MultiPolynomial::print(std::ostream& os) const {
  if(this->isZero())
    return os << "0";

  bool first = true;
  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    if(it->coef == 0)
      continue;

    if(!first)
      os << " + ";

    os << it->coef;

    int dim = it->degrees.size();
    for(int d = 0; d < dim; ++d) {
      int degree = it->degrees[d];
      if(degree > 0)
        os << " x_{" << it->ids[d] << "}^{" << degree << "}";
    }

    first = false;
  }

  return os;
}


std::ostream& MultiPolynomial::print(std::ostream& os, std::vector<std::string> const& names) const {
  if(this->isZero()) {
    os << 0;
    return os;
  }

  bool first = true;
  for(std::list<Monomial>::const_iterator it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    if(it->coef != 0.) {
      if(!first)
        os << " + ";

      os << it->coef;

      int dim = it->degrees.size();
      for(int d = 0; d < dim; ++d) {
        int degree = it->degrees[d];
        int id = it->ids[d];
        if(degree > 0) {
          os << " " << (id < names.size() ? names[id] : ("e_{" + std::to_string(id) + "}")) << "^{" << degree << "}";
        }
      }

      first = false;
    }
  }

  return os;
}

////////////////////////////////////////////////////////////////////////////////

MultiPolynomial MultiPolynomial::mulMonomial(Monomial const& m) const {
  MultiPolynomial result;

  int mSize = m.degrees.size();

  for(auto it = this->monomials.begin(); it != this->monomials.end(); ++it) {
    Monomial r;
    r.coef = it->coef * m.coef;

    int tSize = it->degrees.size();
    int sizeMin = std::min(mSize,tSize);
    int iT = 0;
    int iM = 0;
    while(iT < tSize && iM < mSize) {
      if(it->ids[iT] < m.ids[iM]) {
        r.ids.push_back(it->ids[iT]);
        r.degrees.push_back(it->degrees[iT]);
        ++iT;
      }
      else if(it->ids[iT] > m.ids[iM]) {
        r.ids.push_back(m.ids[iM]);
        r.degrees.push_back(m.degrees[iM]);
        ++iM;
      }
      else {
        r.ids.push_back(m.ids[iM]);
        r.degrees.push_back(it->degrees[iT] + m.degrees[iM]);
        ++iT;
        ++iM;
      }
    }

    for(; iM < mSize; ++iM) {
      r.ids.push_back(m.ids[iM]);
      r.degrees.push_back(m.degrees[iM]);
    }

    for(; iT < tSize; ++iT) {
      r.ids.push_back(it->ids[iT]);
      r.degrees.push_back(it->degrees[iT]);
    }

    result.monomials.push_back(r);
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

MultiPolynomial MultiPolynomial::createIdentity(int variableId) {
  Monomial m;
  m.coef = 1.;
  m.ids.push_back(variableId);
  m.degrees.push_back(1);

  // std::clog << "coef=" << m.coef << std::endl;
  // std::clog << "atoms:";
  // for(int i = 0; i < m.ids.size(); ++i)
  //   std::clog << " (" << m.ids[i] << "," << m.degrees[i] << ")";
  // std::clog << std::endl;

  MultiPolynomial result;
  result.monomials.push_back(m);

  return result;
}

MultiPolynomial MultiPolynomial::changeDomainMonomial(Monomial const& m, std::vector<int> const& ids, std::vector<MultiPolynomial> const& conversion) {
  MultiPolynomial result(1);

  int dim = m.degrees.size();
  for(int d = 0; d < dim; ++d) {
    MultiPolynomial r(1);
    int id = m.ids[d];
    int iConv = std::distance(ids.begin(), std::lower_bound(ids.begin(), ids.end(), id));
    for(int i = 0; i < m.degrees[d]; ++i)
      r *= conversion[iConv];

    result *= r;
  }

  return result * m.coef;
}

std::list<double> MultiPolynomial::bernsteinCoefs(MultiPolynomial const& poly) {
  std::vector<int> ids;
  std::vector<int> degrees = poly.getDegrees(ids);
  int dim = degrees.size();

  std::vector<int> zeros(dim,0);

  std::list<double> result;

  Enumeration enumeration(zeros, degrees);
  for(EnumerationIterator it = enumeration.begin(); it != enumeration.end(); ++it) {
    double coef = bernsteinCoef(poly, ids, *it, degrees);
    result.push_back(coef);
  }

  return result;
}

double MultiPolynomial::bernsteinCoef(MultiPolynomial const& poly, std::vector<int> const& ids, std::vector<int> const& indices, std::vector<int> const& indicesMax) {
  int size = indices.size();

  double result = 0;
  for(auto it = poly.monomials.begin(); it != poly.monomials.end(); ++it) {
    // check degrees
    bool valid = true;
    for(int d = 0; d < size; ++d) {
      int degree = it->getDegree(ids[d]);
      if(degree > indices[d] || degree > indicesMax[d]) {
        valid = false;
        break;
      }
    }
    if(!valid)
      continue; // monomial not part of the result

    double elt = 1;
    for(int d = 0; d < size; ++d) {
      int degree = it->getDegree(ids[d]);
      elt *= Binomial::getCoef(indices[d], degree);
      elt /= Binomial::getCoef(indicesMax[d], degree);
    }
    elt *= it->coef;

    result += elt;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

MultiPolynomial operator+(double const& d, MultiPolynomial const& poly) {
  return MultiPolynomial(d) + poly;
}

MultiPolynomial operator-(double const& d, MultiPolynomial const& poly) {
  return MultiPolynomial(d) - poly;
}

MultiPolynomial operator*(double const& d, MultiPolynomial const& poly) {
  return MultiPolynomial(d) * poly;
}

std::ostream& operator<<(std::ostream& os, MultiPolynomial const& poly) {
  poly.print(os);
  return os;
}
