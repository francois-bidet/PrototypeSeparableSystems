#include "Decomposer.hpp"

void Decomposition::eval(Time& t, State& s) {
  TaylorModel img = this->f(t,s);
  this->imgPos = this->pos(img);
  this->imgNeg = this->neg(img);
}

TaylorModel Decomposition::getPos() {
  return imgPos;
}

TaylorModel Decomposition::getNeg() {
  return imgNeg;
}

void Decomposition::setFunction(SubFunction const& fun) {
  this->f = fun;
}

void Decomposition::setPos(InternalFunction const& fun) {
  this->pos = fun;
}

void Decomposition::setNeg(InternalFunction const& fun) {
  this->neg = fun;
}

////////////////////////////////////////////////////////////////////////////////

Decompositions Decomposer::decompose(SubFunctions const& fi, FunctionsImages const& imgs) {
  int m = fi.size();

  Decompositions result(m);

  for(int i = 0; i < m; ++i)
    result[i] = this->decompose(fi[i], imgs[i]);

  return result;
}

