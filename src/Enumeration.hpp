#pragma once

#include <vector>

/*
 * enumerate all possible multi-indices given min and max for each dimension
 */
class EnumerationIterator {
public:
  EnumerationIterator(std::vector<int> const& min, std::vector<int> const& max, bool end);
  EnumerationIterator(EnumerationIterator const&);

  EnumerationIterator& operator=(EnumerationIterator const&);

  ~EnumerationIterator();

  EnumerationIterator& operator++();

  std::vector<int> const& operator*() const;

  bool operator==(EnumerationIterator const& other) const;
  bool operator!=(EnumerationIterator const& other) const;

private:
  std::vector<int> min;
  std::vector<int> max;

  int dim;

  std::vector<int> current;

  bool isEnd;
};

class Enumeration {
public:
  Enumeration(std::vector<int> const& min, std::vector<int> const& max);

  EnumerationIterator const& begin() const;
  EnumerationIterator const& end() const;

private:
  std::vector<int> min;
  std::vector<int> max;

  EnumerationIterator itBegin;
  EnumerationIterator itEnd;
};
