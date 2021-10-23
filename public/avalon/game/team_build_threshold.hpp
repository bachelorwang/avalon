#pragma once
#include <cstdint>

namespace avalon {

template <int8_t>
struct team_build_threshold;

template <>
struct team_build_threshold<5> {
  static constexpr int8_t value[5] = {2, 3, 2, 3, 3};
};

template <>
struct team_build_threshold<6> {
  static constexpr int8_t value[5] = {2, 3, 4, 3, 4};
};

template <>
struct team_build_threshold<7> {
  static constexpr int8_t value[5] = {2, 3, 3, 4, 4};
};

template <>
struct team_build_threshold<8> {
  static constexpr int8_t value[5] = {3, 4, 4, 5, 5};
};

template <>
struct team_build_threshold<9> {
  static constexpr int8_t value[5] = {3, 4, 4, 5, 5};
};

template <>
struct team_build_threshold<10> {
  static constexpr int8_t value[5] = {3, 4, 4, 5, 5};
};

}  // namespace avalon
