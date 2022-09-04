#pragma once

#include "Interval.hpp"
#include "Integrator.hpp"

class Contractor {
public:
  Contractor();

  void setIntegrator(Integrator*);

  virtual State contract(State& s0, Time& dt, State& s); // contract s. Throw exception if empty state

protected:
  State intersection(State const& s1, State const& s2); // assume same polynomial part
  State enlarge(State const& s);
  bool isContracted(State const& s1, State const& s2); // Check if s2 is subset of s1. Assume same polynomial part and s2 subset of s1.
  bool isEnoughContracted(State const& s1, State const& s2, double const& threshold); // Check if s2 is proper subset of s1 with enough relative contraction. Assume same polynomial part and s2 subset of s1.

  SetsLibrary::Interval absGrowth;
  double relGrowth;

  Integrator* integrator;
};
