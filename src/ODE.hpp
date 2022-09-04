#pragma once

#include <vector>
#include <string>
#include <functional>

#include "Interval.hpp"
#include "TaylorModel.hpp"

using State = std::vector<TaylorModel>;
using Input = TaylorModel;
using Inputs = std::vector<Input>;
using Time = TaylorModel;
using SubFunction = std::function<TaylorModel(Time&, State&)>;
using SubFunctions = std::vector<SubFunction>;

using SubInputFunction = std::function<Input(Inputs&)>;
using SubInputFunctions = std::vector<std::vector<SubInputFunction> >;

using StateDomain = std::vector<SetsLibrary::Interval>;
using InputsDomains = std::vector<SetsLibrary::Interval>;

class ODE {
public:
  virtual SubFunctions getHi() const = 0;
  virtual SubInputFunctions getGij() const = 0;
  virtual int getDim() const = 0; // state dimension

  virtual StateDomain getStateDomain() const = 0;
  virtual InputsDomains getInputsDomains() const = 0;

  virtual std::vector<std::string> getNames() const;
  virtual std::string getNameODE() const;
};
