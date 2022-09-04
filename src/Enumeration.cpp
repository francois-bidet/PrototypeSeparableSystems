#include "Enumeration.hpp"

EnumerationIterator::EnumerationIterator(std::vector<int> const& min, std::vector<int> const& max, bool end)
  : min(min), max(max), dim(min.size()), current(min), isEnd(end)
{}

EnumerationIterator::EnumerationIterator(EnumerationIterator const& other)
  : min(other.min), max(other.max), dim(other.dim), current(other.current), isEnd(false)
{}

EnumerationIterator::~EnumerationIterator() {}

EnumerationIterator& EnumerationIterator::operator=(EnumerationIterator const& other) {
  this->min = other.min;
  this->max = other.max;

  this->dim = other.dim;

  this->current = other.current;

  this->isEnd = other.isEnd;

  return *this;
}

EnumerationIterator& EnumerationIterator::operator++() {
  if(isEnd)
    return *this;

  int d = 0;

  while(d < dim) {
    ++current[d];
    if(current[d] <= max[d])
      break;

    current[d] = min[d];
    ++d;
  }

  if(d >= dim) {
    isEnd = true;
  }

  return *this;
}

std::vector<int> const& EnumerationIterator::operator*() const {
  return this->current;
}

bool EnumerationIterator::operator==(EnumerationIterator const& other) const {
  if(isEnd != other.isEnd)
    return false;

  if(this->isEnd && other.isEnd)
    return true;

  return
    this->dim == other.dim
    && this->current == other.current
    && this->min == other.min
    && this->max == other.max
    ;
}

bool EnumerationIterator::operator!=(EnumerationIterator const& other) const {
  return !(*this == other);
}

////////////////////////////////////////////////////////////////////////////////

Enumeration::Enumeration(std::vector<int> const& min, std::vector<int> const& max)
  : min(min), max(max), itBegin(min,max,false), itEnd(min,max,true)
{}

EnumerationIterator const& Enumeration::begin() const {
  return this->itBegin;
}

EnumerationIterator const& Enumeration::end() const {
  return this->itEnd;
}
