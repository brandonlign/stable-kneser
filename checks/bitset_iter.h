// Portable replacements for libstdc++'s std::bitset::_Find_first/_Find_next.
#pragma once
#include <bitset>
#include <cstddef>
template <std::size_t N> std::size_t find_first(const std::bitset<N>& b) {
  for (std::size_t i = 0; i < N; ++i) if (b[i]) return i;
  return N;
}
template <std::size_t N> std::size_t find_next(const std::bitset<N>& b, std::size_t x) {
  for (std::size_t i = x + 1; i < N; ++i) if (b[i]) return i;
  return N;
}
