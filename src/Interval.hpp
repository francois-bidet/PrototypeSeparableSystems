#ifndef DEF_SETLIBRARY_INTERVAL_HPP
#define DEF_SETLIBRARY_INTERVAL_HPP

#include <ostream>

namespace SetsLibrary
{
  class Interval
  {
  public:
    Interval(double v = 0);
    Interval(double min, double max);
    Interval(Interval const&);

    bool isZero() const;

    bool operator==(Interval const&) const;

    Interval operator-() const;

    Interval& operator+=(Interval const&);
    Interval& operator-=(Interval const&);
    Interval& operator*=(Interval const&);
    Interval& operator/=(Interval const&);

    Interval operator+(Interval const&) const;
    Interval operator-(Interval const&) const;
    Interval operator*(Interval const&) const;
    Interval operator/(Interval const&) const;

    Interval& operator+=(double const&);
    Interval& operator-=(double const&);
    Interval& operator*=(double const&);
    Interval& operator/=(double const&);

    Interval operator+(double const&) const;
    Interval operator-(double const&) const;
    Interval operator*(double const&) const;
    Interval operator/(double const&) const;

    double getMin() const;
    double getMax() const;
    double getMedian() const;

    bool isSubsetOf(Interval const&) const;
    bool isProperSubsetOf(Interval const&) const;

    bool contains(double const&) const;

    Interval sqrt() const;
    Interval abs() const;
    Interval pow(int) const;

    Interval cos() const;
    Interval sin() const;
    Interval inverse() const;

    std::ostream& print(std::ostream&) const;

  private:
    double min;
    double max;
  };
}

SetsLibrary::Interval operator+(double const&, SetsLibrary::Interval const&);
SetsLibrary::Interval operator-(double const&, SetsLibrary::Interval const&);
SetsLibrary::Interval operator*(double const&, SetsLibrary::Interval const&);
SetsLibrary::Interval operator/(double const&, SetsLibrary::Interval const&);

SetsLibrary::Interval sqrt(SetsLibrary::Interval const&);
SetsLibrary::Interval abs(SetsLibrary::Interval const&);
SetsLibrary::Interval cos(SetsLibrary::Interval const&);
SetsLibrary::Interval sin(SetsLibrary::Interval const&);

#endif
