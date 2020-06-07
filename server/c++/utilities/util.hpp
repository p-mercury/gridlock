#include <stdexcept>
#include <vector>

#include "util.hpp"

int randomNumberFromInterval(int intervalStart, int intervalEnd);

template <class T>
T randomElement(const std::vector<T> v);

template <class T>
T weightedDistribution(std::vector<T, int> probabilityPairs);