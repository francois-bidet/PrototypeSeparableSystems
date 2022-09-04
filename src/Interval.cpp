#include "Interval.hpp"

#include <cmath>

#include <cassert>

namespace SetsLibrary
{
  Interval::Interval(double v)
    : min(v), max(v)
  {}

  Interval::Interval(double min, double max)
    : min(min), max(max)
  {
    assert(min <= max);
  }

  Interval::Interval(Interval const& other)
    : min(other.min), max(other.max)
  {}

  bool Interval::isZero() const
  {
    return (this->min == 0.) && (this->max == 0.);
  }

  bool Interval::operator==(Interval const& other) const
  {
    return this->min == other.min && this->max == other.max;
  }

  Interval Interval::operator-() const
  {
    return Interval(-this->max, -this->min);
  }

  Interval& Interval::operator+=(Interval const& other)
  {
    this->min += other.min;
    this->max += other.max;
    return *this;
  }

  Interval& Interval::operator-=(Interval const& other)
  {
    this->min -= other.max;
    this->max -= other.min;
    return *this;
  }

  Interval& Interval::operator*=(Interval const& other)
  {
    *this = *this * other;
    return *this;
  }

  Interval& Interval::operator/=(Interval const& other)
  {
    *this = *this / other;
    return *this;
  }

  Interval Interval::operator+(Interval const& other) const
  {
    double min = this->min + other.min;
    double max = this->max + other.max;
    return Interval(min,max);
  }

  Interval Interval::operator-(Interval const& other) const
  {
    double min = this->min - other.max;
    double max = this->max - other.min;
    return Interval(min,max);
  }

  Interval Interval::operator*(Interval const& other) const
  {
    double a = this->min * other.min;
    double b = this->min * other.max;
    double c = this->max * other.min;
    double d = this->max * other.max;

    double min = std::min(a, std::min(b, std::min(c,d)));
    double max = std::max(a, std::max(b, std::max(c,d)));

    return Interval(min,max);
  }

  Interval Interval::operator/(Interval const& other) const
  {
    return *this * (other.inverse());
  }

  Interval& Interval::operator+=(double const& d)
  {
    this->min += d;
    this->max += d;
    return *this;
  }

  Interval& Interval::operator-=(double const& d)
  {
    this->min -= d;
    this->max -= d;
    return *this;
  }

  Interval& Interval::operator*=(double const& d)
  {
    if(d < 0) {
      double tmp = this->min;
      this->min = this->max * d;
      this->max = tmp * d;
    }
    else {
      this->min *= d;
      this->max *= d;
    }
    return *this;
  }

  Interval& Interval::operator/=(double const& d)
  {
    if(d < 0) {
      double tmp = this->min;
      this->min = this->max / d;
      this->max = tmp / d;
    }
    else {
      this->min /= d;
      this->max /= d;
    }
    return *this;
  }

  Interval Interval::operator+(double const& d) const
  {
    double min = this->min + d;
    double max = this->max + d;
    return Interval(min, max);
  }

  Interval Interval::operator-(double const& d) const
  {
    double min = this->min - d;
    double max = this->max - d;
    return Interval(min, max);
  }

  Interval Interval::operator*(double const& d) const
  {
    if(d < 0) {
      double min = this->max * d;
      double max = this->min * d;
      return Interval(min,max);
    }
    else {
      double min = this->min * d;
      double max = this->max * d;
      return Interval(min,max);
    }
  }

  Interval Interval::operator/(double const& d) const
  {
    if(d < 0) {
      double min = this->max / d;
      double max = this->min / d;
      return Interval(min,max);
    }
    else {
      double min = this->min / d;
      double max = this->max / d;
      return Interval(min,max);
    }
  }

  double Interval::getMin() const
  { return this->min; }

  double Interval::getMax() const
  { return this->max; }

  double Interval::getMedian() const
  { return (this->min + this->max)/2; }

  bool Interval::isSubsetOf(Interval const& other) const
  {
    return (other.min <= this->min) && (this->max <= other.max);
  }

  bool Interval::isProperSubsetOf(Interval const& other) const
  {
    return this->isSubsetOf(other)
      && ((other.min < this->min) || (this->max < other.max));
  }

  bool Interval::contains(double const& p) const
  {
    return this->getMin() <= p && p <= this->getMax();
  }

  Interval Interval::sqrt() const
  {
    if(this->min < 0)
      throw std::runtime_error("Interval::sqrt() on negative set");

    return Interval(std::sqrt(this->min), std::sqrt(this->max));
  }

  Interval Interval::abs() const
  {
    if(this->min >= 0)
      return *this;
    else if(this->max < 0)
      return -*this;
    else
      return Interval(0, std::max(-this->min, this->max));
  }

  Interval Interval::pow(int degree) const
  {
    if(this->min >= 0) {
      return Interval(std::pow(this->min, degree), std::pow(this->max, degree));
    }
    else if(this->max < 0) {
      double a = std::pow(this->min, degree);
      double b = std::pow(this->max, degree);
      if(a < b)
        return Interval(a, b);
      else
        return Interval(b, a);
    }
    else { // contains 0
      double a = std::pow(this->min, degree);
      double b = std::pow(this->max, degree);
      if(degree % 2 == 0)
        return Interval(std::max(a,b));
      else
        return Interval(a,b);
    }
  }

  Interval Interval::cos() const { // implementation presented in "Implementation of Interval Arithmetic in CORA 2016"
    double xmin = min;
    double xmax = max;
    double pi = M_PI;
    double twopi = 2*pi;
    if(xmax - xmin >= twopi)
      return Interval(-1,1);

    double ymin = std::fmod(xmin,twopi);
    if(ymin < 0)
      ymin += twopi;
    double ymax = std::fmod(xmax,twopi);
    if(ymax < 0)
      ymax += twopi;

    if(ymin < pi) { // ymin in [0,pi[
      if(ymax < pi) { // ymax in [0,pi[
	if(ymin > ymax) {
	  return Interval(-1,1);
	}
	else {
	  return Interval(std::cos(ymax),std::cos(ymin));
	}
      }
      else { // ymax in [pi,2*pi[
	return Interval(-1, std::max(std::cos(ymin),std::cos(ymax)));
      }
    }
    else { // ymin in [pi,2*pi[
      if(ymax < pi) { // ymax in [0,pi[
	return Interval(std::min(std::cos(ymin),std::cos(ymax)),1);
      }
      else { // ymax in [pi,2*pi[
	if(ymin > ymax) {
	  return Interval(-1,1);
	}
	else {
	  return Interval(std::cos(ymin),std::cos(ymax));
	}
      }
    }
  }

  Interval Interval::sin() const { // implementation depending on the one of Interval::cos
    double halfpi = M_PI / 2;
    return (halfpi - *this).cos();
  }

  Interval Interval::inverse() const {
    if(this->min <= 0 && this->max >= 0)
      throw std::runtime_error("Division by zero");
    return Interval(1/(this->max), 1/(this->min));
  }

  std::ostream& Interval::print(std::ostream& os) const
  {
    os << "[" << this->min << "," << this->max << "]";
    return os;
  }
}

SetsLibrary::Interval sqrt(SetsLibrary::Interval const& i)
{ return i.sqrt(); }

SetsLibrary::Interval abs(SetsLibrary::Interval const& i)
{ return i.abs(); }

SetsLibrary::Interval cos(SetsLibrary::Interval const& i)
{ return i.cos(); }

SetsLibrary::Interval sin(SetsLibrary::Interval const& i)
{ return i.sin(); }

SetsLibrary::Interval operator+(double const& d, SetsLibrary::Interval const& i) {
  return i + d;
}

SetsLibrary::Interval operator-(double const& d, SetsLibrary::Interval const& i) {
  return d + (-i);
}

SetsLibrary::Interval operator*(double const& d, SetsLibrary::Interval const& i) {
  return i * d;
}

SetsLibrary::Interval operator/(double const& d, SetsLibrary::Interval const& i) {
  return SetsLibrary::Interval(d) / i;
}
