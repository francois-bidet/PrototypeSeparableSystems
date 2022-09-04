#include "AffineDecomposer.hpp"
#include <iostream>

Decomposition AffineDecomposer::decompose(SubFunction const& f, FunctionImage const& img) {
  Decomposition result;
  result.setFunction(f);

  double a = img.getMin();
  double b = img.getMax();

  if(a >= 0) {
    result.setPos([](TaylorModel const& img){ return img; });
    result.setNeg([](TaylorModel const&){ return 0; });
  }
  else if(b <= 0) {
    result.setPos([](TaylorModel const&){ return 0; });
    result.setNeg([](TaylorModel const& img){ return -img; });
  }
  else {
    double alpha = b / (b - a);
    double beta = - a * b / (b - a);

    result.setPos([alpha,beta](TaylorModel const& img){ return alpha * img + beta; });
    result.setNeg([alpha,beta](TaylorModel const& img){ return (alpha - 1) * img + beta; });
  }

  return result;
}
